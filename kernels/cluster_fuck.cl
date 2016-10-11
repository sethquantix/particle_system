#include "particle.hl"

// #pragma OPENCL EXTENSION cl_khr_fp64 : disable

/* Dist Estimator
	float2	dist_est = (float2)0.0f;
	mad_el= (float4)(offs.x, offs.y, 0.0f,0.0f);
	mad_el.z = 1.0f;itera=-1.0f;
	while (++itera < MAX_ITER)
	{
		dist_est.x = 2.0f * (mad_el.x*mad_el.z - mad_el.y*mad_el.w);
		dist_est.y = 2.0f * (mad_el.y*mad_el.z - mad_el.x*mad_el.w);
		mad_el.z = dist_est.x;
		dist_est.x = mad_el.x * mad_el.x - mad_el.y * mad_el.y + cos(3.14f+3.14f*timer[0]/180.0f)/2.0f;
		mad_el.y = 2.0f * mad_el.x * mad_el.y + sin(3.14f+3.14f*timer[0]/180.0f)/2.0f;
		mad_el.x = dist_est.x;
		dist_est.x = mad_el.x * mad_el.x + mad_el.y * mad_el.y;
		dist_est.y = mad_el.z * mad_el.z + mad_el.w * mad_el.w;
		if (dist_est.x > exp(60.0f) || dist_est.y > exp(60.0f))
			break;
	}
	float	a_a = sqrt(dist_est.y);
	a_a = 2.0f * a_a * log(a_a) / sqrt(dist_est.y);
*/

__kernel void init(uint2 seed, __global float4 *pos_par, __global float4 *acc_par, __global float4 *spd_par, __global float *lc_par, __global int2 *m_par, __global float4 *col)
{
	int			x = get_global_id(0);
	int			y = get_global_id(1);
	int			l = get_global_size(0);
	int			h = get_global_size(1);
	uint		a = seed.x + x + y * l;
	int			ind = x + y * l;

	float t = M_PI + ((float)x / (float)l) * 2.0f * M_PI;
	float d = M_PI + ((float)y / (float)h) * 2.0f * M_PI;//(float4)( 10.0f*sin(t)*cos(d), 10.0f*sin(t)*sin(d), 10.0f*cos(t));
	// pos_par[ind] = (float4)(100.0f*cos(d)*cos(t), 100.0f*cos(d)*sin(t), 100.0f*cos(d*t)*sin(t*d)); // crab
	pos_par[ind] = (float4)(10.0f*cos(d*t)*cos(t*d), 10.0f*cos(d*t)*sin(t*d), 10.0f*cos(d*t)*sin(t*d),0.0f); // torus
//	pos_par[ind] = (float4)(10.0f*cos(d*t)-cos(t*d), 10.0f*cos(d*t)*sin(t*d), 10.0f*cos(d*t)*sin(t*d)); // infinity
	// pos_par[ind] = (float4)(-5.0f + 10.0f * (float)x/(float)h*2.0f, -5.0f + 10.0f * (float)y/(float)l*2.0f, -5.0f + 10.0f * ((float)(cos((float)x*x*x)*cos((float)y*y*y)))); // cube
//	pos_par[ind] = (float4)( 10.0f*sin(t)*cos(d), 10.0f*sin(t)*sin(d), 10.0f*cos(t)); // sphere
	// pos_par[ind] = (float4)((float)x/(float)h*2.0f, (float)y/(float)l*2.0f,  (cbrt((float)((x*x*x)*(y*y*y))))/(float)(h)); // sandwich
	// pos_par[ind] = (float4)(100.0f*(float)(x)/h, 100.0f*(float)(x^y)/(float)l, 100.0f*(float)(x+y)/(l+h));

	pos_par[ind] = 5.0f-10.0f*(float4)((float)x / (float)l, (float)y / (float)h , 0.0f ,0.0f);
//	spd_par[ind] = 0.1f * (float4)(10.0f*cos(d) * cos(t), -10.0f*cos(d) * sin(t), 10.0f*sin(t));
	// spd_par[ind] = 1.1f * (float4)(cos(d) * cos(t), -cos(d) * sin(t), sin(t));//(float4)(0.0f);
	spd_par[ind] = (float4)0.0f;
//	spd_par[ind] = 0.01f * (float4)(pos_par[ind].y, -pos_par[ind].x, pos_par[ind].z);
	acc_par[ind] = (float4)(0.0f);
	m_par[ind] = (int2)(0);
	a = a ^ (a << 11);
	a = seed.y ^ (seed.y >> 19) ^ (a ^ (a >> 8));
	lc_par[ind] = (1.0f + (float)a / (float)(2 ^ 32 - 1)) / 2.0f;
	col[ind] = (float4)(0.0f);//(clamp((float)(ind)/(GLOBAL + GLOBAL * l), 0.3f, 1.0f));
	//col[ind] = (float4)(1.0f);
	// col[ind].x = (1.0f - col[ind].y) * 360.0f;
}

int		hsv(float4 c)
{
	int		ti = (int)(c.x / 60.0f) % 6;
	float	f = c.x / 60.0f - ti;
	float	l = c.z * (1.0f - c.y);
	float	m = c.z * (1.0f - f * c.y);
	float	n = c.z * (1.0f - (1.0f - f) * c.y);
	float4	rgb;

	rgb = 255.0f * (float4)(c.z * (ti == 0 || ti == 5) + m * (ti == 1) +
			l * (ti == 2 || ti == 3) + n * (ti == 4),
			c.z * (ti == 1 || ti == 2) + m * (ti == 3) +
			l * (ti > 3) + n * (ti == 0),
			c.z * (ti == 3 || ti == 4) + m * (ti == 5) +
			l * (ti < 2) + n * (ti == 2),0.0f);
	return (((int)rgb.x << 16) + ((int)rgb.y << 8) + (int)rgb.z);
}

#define RAD 180.0f / 3.14f

float4	mix_vect(float4 crab_a, float4 crab_b, float parity)
{
	float4	ret;

	ret = (1.0f - parity) * crab_a + (1.0f + parity) * crab_b;
	return (ret);
}

__kernel void zeroes(__global float *z, __global int *i)
{
	int			x = get_global_id(0);
	int			y = get_global_id(1);
	int			l = get_global_size(0);

	i[x + y * l] = -1;
	z[x + y * l] = -1;
}

// __kernel	void init_grid(__global long *grid)
// {
	// int			x = get_global_id(0);
	// int			y = get_global_id(1);
	// int			l = get_global_size(0);
	// int			h = get_global_size(1);
// 
	// grid[x + y * l][]
// }
// 

// hash(x,y,z) = ( x p1 xor y p2 xor z p3) mod n
// n == taille de la table de hash ==> ^2 // n == 65537 
// where p1, p2, p3 are large prime numbers, in our case 73856093, 19349663, 83492791, respectively. The value n is the hash table size.
// http://www.beosil.com/download/CollisionDetectionHashing_VMV03.pdf

#define CELLS 100

// __kernel	void adjacency(__global int *grid, __global float4 *pos_par) // fill grid with particle positions , grid est un tableau contenant les indices des particules aux positions ==> pos_par.w
// {
	// int			x = get_global_id(0);
	// int			y = get_global_id(1);
	// int			l = get_global_size(0);
	// int			h = get_global_size(1);

	// ramener position de particule de 0 à h*l

	// pos_par[x + y * l].w = hash((int)(pos_par[x+ y * l].x / CELLS), (int)(pos_par[x+ y * l].y / CELLS), (int)(pos_par[x+ y * l].z / CELLS));
	// how to avoid table collisions ?
	// grid[pos_par[x + y * l].w] = ;
// }

__kernel void particle(__global float4 *pos_par, __global float4 *acc_par, __global float4 *spd_par, __global float *lc_par, __global int2 *m_par, __global float4 *col, 
	__constant t_data *data, __global int *img, __constant long *timer, __global int *switcher, int t, __global float *z, __global int *i)
{
	int			x = get_global_id(0);
	int			y = get_global_id(1);
	int			l = get_global_size(0);
	int			h = get_global_size(1);
	int			ind = x + y * l;
	float4		k;
	float4		cam = (float4)(data->cam.xyz,0.0f);
	float4		c[3];
	float4		v1;
	float4		v2;
	float4		g;
	float		d = length(cam);
	float		r;
	//float		rel_mov = ((pow(data->old_g.x - data->g.x, 2)) + (pow(data->old_g.y - data->g.y, 2)));

	float4		p = pos_par[ind] ;
	float4		acc = acc_par[ind];
	float4		vit = spd_par[ind];
	int2		m_m = m_par[ind];
	float4		col_p = col[ind];
	float4		mad_el = (float4)0.0f;
	// col_p = (float4)( (float)(sin(((3.14f*(x)/(GLOBAL))+sin(timer[0]*3.14f/180.0f))*3.14f)*sin(((3.14f*(y)/(GLOBAL))+sin(timer[0]*3.14f/180.0f))*3.14f) ) );
	// if (col_p.x < 0.0f)
		// col_p = -col_p;
	// col_p.x = (1.0f - col_p.y) * 360.0f;
	c[0] = data->corners[0] * d + cam;
	c[1] = data->corners[1] * d + cam;
	c[2] = data->corners[2] * d + cam;
	v1 = c[1] - c[0];
	v2 = c[2] - c[0];

	// if (mad_el.x != ((float)x - (float)l / 2.0f) / (0.25f * l))
		img[m_m[0] + m_m[1] * data->w] = 0;
	// write_imageui(img, (int2)(m_m[0], m_m[1]), (uint4)0);
	float4	s = dot(vit, vit) * vit-vit;

	if (t & 0x1)
	{
	float tt = /*M_PI +*/ /*(cos(3.14f*timer[0]/180.0f))**/(1.0f-((float)(x) / (float)(l))) /*+ (sin((float)timer[0]/180.0f))*/ * 2.0f * 3.14f;//+ M_PI * cos((float)timer[0]/100.0f)/2.0f;
	float dd = /*M_PI +*/ /*(cos(3.14f*timer[0]/180.0f))**/(1.0f-((float)(y) / (float)(h))) /*+ (cos((float)timer[0]/180.0f))*/ * 2.0f * 3.14f;//+ M_PI * sin((float)timer[0]/100.0f)/2.0f;

	// int j = 0;
	// float4	acc_tmp = (float4)0.0f;
	// while (j < NUM_P)
	// {
	// 	g = (pos_par[j] - p);//+0.001f;
	// 	r = dot(g, g);
	// 	k = 100.0f * (1.0f - (1.0f-r > 1.0f ? r : 1.0f));
	// 	k *= s;
	// 	k = (g) / ((r*10.0f > 0.5f ? r*10.0f : 0.50f)) - k;//2.0f;
	// 	acc_tmp += k;
	// 	g = (pos_par[j+1] - p);//+0.001f;
	// 	r = dot(g, g);
	// 	k = 100.0f * (1.0f - (1.0f-r > 1.0f ? r : 1.0f));
	// 	k *= s;
	// 	k = (g) / ((r*10.0f > 0.5f ? r*10.0f : 0.50f)) - k;//2.0f;
	// 	acc_tmp += k;
	// 	j += 2;
	// }
	// acc_tmp /= (float)NUM_P*2.0f;
	// float4	crab = (float4)(((float)x/(float)h)*2.0f, ((float)y/(float)l)*2.0f, ((float)(x*x/(h)-y)*pow(-1.0f, x)/((float)h))+((float)(y*y/(l)-x)*pow(-1.0f, y)/(float)l));
	// float4	crab = (float4)(((float)(x)/(float)h)*2.0f, ((float)(y)/(float)l)*2.0f, ((float)(x*x/(h)-x-y)*pow(-1.0f, x)/((float)h))*((float)(y*y/(l)-x-y)*pow(-1.0f, y)/(float)l));
	// float4	crab = (float4)(((float)(x)/(float)h)*2.0f, ((float)(y)/(float)l)*2.0f, ((float)(x*x/(h)-x)*pow(-1.0f, x)/((float)h))*((float)(y*y/(l)-y)*pow(-1.0f, y)/(float)l)); // moar
	// float4	crab = (float4)(((float)(x)/(float)h)*2.0f, ((float)(y)/(float)l)*2.0f, ((float)(x*x/(h)-x)*pow(-1.0f, x)/((float)h))+((float)(y*y/(l)-y)*pow(-1.0f, y)/(float)l)); // koopa
	// float4	crab = (float4)(((float)(x)/(float)h)*2.0f, ((float)(y)/(float)l)*2.0f, ((float)(cos((float)x*x*x*6.28f)*sin((float)y*y*y*6.28f))));
		//;((float)(x*x/h-y*x/h)*pow(-1.0f, x)/((float)h)+(float)(y*y/l-x*y/l)*pow(-1.0f, y)/(float)l));
	//(float)(x*x/h-y)*pow(-1.0f, x)/((float)h)+(float)(y*y/l-x)*pow(-1.0f, y)/(float)l);
	//pow((float)(x)*pow(-1.0f, x)/((float)h)+(float)(y^x)*pow(-1.0f, y)/(float)l,1.0f));
	//pow((float)(x^y)*pow(-1.0f, x)/((float)h)+(float)(y^x)*pow(-1.0f, y)/(float)l,1.0f));
	//((float)x*pow(-1.0f, y)/(float)h+(float)y*pow(-1.0f, x)/(float)l));//((float)(cos((float)x*x*x*6.28f)*cos((float)y*y*y*6.28f))));//((float)(pow(cos((float)x*6.28f),3.0f)*pow(cos((float)y*6.28f),3.0f)))
	// float4	crab = (float4)(cos(dd*tt)*cos(tt*dd), cos(dd*tt)*sin(tt*dd), cos(dd*tt)*sin(tt*dd)); // crab;
	// float4	crab = (float4)(cos(dd*tt)*cos(tt*dd), cos(dd*tt)*sin(tt*dd), cos(dd*tt)*sin(tt*dd)); 
	// float4	crab = (float4)(sin(tt)*cos(dd), sin(tt)*sin(dd), cos(tt));
//	float4	crab = (float4)((float)(x)/h +(cos((float)timer[0]/10.0f)/2.0f), (float)(x^y)/(float)l+(sin((float)timer[0]/10.0f)/2.0f), (float)(x+y)/(l)+(tanh((float)timer[0]/10.0f)/2.0f));//(float4)((float)(x)/l, (float)(x^y)/(float)l, (x+y)/l);
// float4 crab = (float4)((float)(x)/h, (float)(x^y)/(float)l, (float)(x+y)/(l+h));
	//	float4	crab = (float4)((float)(x)/h, (float)(x|y)/(float)l, (float)(x+y)/(l+h));
	// float4	crab = (float4)(cos(dd)*cos(tt), cos(dd)*sin(tt), cos(dd*tt)*sin(dd));
	//	float4	crab = (float4)(cos(dd*tt)*cos(tt*tt), cos(dd*dd)*sin(tt*tt), sin(dd*dd)*sin(tt*tt));
	// float4	crab = (float4)(cos(dd)*cos(tt), cos(dd)*sin(tt), sin(tt)); // PART CALABY
	// float4	crab = (float4)(sin(dd)*cos(tt), cos(dd)*sin(tt), sin(tt)*(sin(dd)+cos(tt))); //ANOTHER
	// float4	crab = (float4)(sin(dd)*cos(tt), cos(dd)*sin(tt), sin(tt)*sin(dd)); //ANOTHER
	// float4	crab = (float4)(sin(dd)*cos(tt), cos(dd)*sin(tt), sin(tt));
	// float4	crab = (float4)(sin(dd)*sin(tt), sin(dd)*cos(tt), sin(tt)*cos(dd*tt));//circlecomplete
	// float4	crab = (float4)(sin(dd)*sin(tt), sin(dd)*cos(tt), sin(tt));//beautiful
	// float4	crab = (float4)(cos(dd)*sin(tt), sin(dd)*sin(tt), sin(tt));
	// float4	crab = (float4)(cos(dd*tt)*sin(tt), sin(dd*tt)*sin(tt), sin(tt));//sablier
	// float4	crab = (float4)(sin(dd)*cos(tt), sin(dd)*sin(tt), sin(tt*dd)*cos(dd));
	// float4	crab = (float4)(sin(dd)*cos(tt), sin(dd)-sin(tt)*cos(dd), sin(tt*dd)*cos(dd));
	// float4	crab = (float4)(sin(dd)*cos(tt)-cos(dd), sin(dd)-sin(tt)*cos(dd), sin(tt*dd)*cos(dd)); // GUD
	// float4	crab = (float4)(sin(dd)*cos(tt)+sin(dd)*sin(tt), sin(dd)-sin(tt)*cos(dd), sin(tt*dd)*cos(dd));
	// float4	crab = (float4)(sin(dd)*cos(tt)+sin(dd)*sin(tt), sin(dd)-sin(tt)*cos(dd), sin(tt*dd)*cos(dd)-sin(dd)) ;//+ (float4)(sin(dd)*cos(tt)-cos(dd), sin(dd)-sin(tt)*cos(dd), sin(tt*dd)*cos(dd));
	// float4	crab = (float4)(-sin(dd)*cos(tt-dd)+cos(dd+tt)*sin(tt), sin(dd+tt)-sin(tt)*cos(dd), sin(tt+dd)*cos(dd-tt));
	// float4	crab = (float4)(-sin(dd)*cos(tt-dd)+sin(dd+tt)*sin(tt), sin(dd+tt)-sin(tt)*cos(dd), sin(tt+dd)*cos(dd-tt));
	// float4	crab = (float4)(cos(dd+tt)+cos(tt+dd), cos(dd+tt)+sin(tt+dd), cos(dd-tt)*sin(tt+dd)); //((float)(pow(cos((float)x*6.28f),3.0f)*pow(cos((float)y*6.28f),3.0f))) 
//float4	crab = (float4)(((float)x/(float)h)*2.0f, ((float)y/(float)l)*2.0f, (((float)x/(float)h)*(float)(x*x/(h)-y)*pow(-1.0f, x)/((float)h))+( ((float)y/(float)l) * (float)(y*y/(l)-x)*pow(-1.0f, y)/(float)l));
	// float4	crab = (float4)(((float)x/(float)h)*2.0f, ((float)y/(float)l)*2.0f, (((float)(x)/(float)h)*(float)(x*x/(h)-y)*pow(-1.0f, x)/((float)h))+( ((float)y/(float)l) * (float)(y*y/(l)-x)*pow(-1.0f, y)/(float)l));
// float4	crab = (float4)( (4.0f + 2.0f * cos(tt/0.5f)) * cos(dd/0.5f), (4.0f + 2.0f * cos((tt)/0.5f)) * sin(dd/0.5f), 2.0f * sin(tt/0.5f) );

// float4	crab = (float4)( (3.14f + (tt/0.50f) * cos(dd/0.50f))*cos(dd/0.50f) , (3.14f + (tt/0.50f) * cos(dd/0.50f))*sin(dd/0.50f) , (tt/0.50f) * sin(dd/0.50f) );

// float4	crab = (float4)( (3.0f + cos(dd/2.0f)*sin(tt) -sin(dd/2.0f)*sin(2.0f*tt) ) * cos(dd) , (3.0f + cos(dd/2.0f)*sin(tt) -sin(dd/2.0f)*sin(2.0f*tt) ) * sin(dd) , sin(dd/2.0f)*sin(tt) + cos(dd/2.0f) * sin(2.0f*tt) );

	// //float	r_x = cos(dd/2.0f) * (3.0f+ cos(dd/2.0f) * (sqrt(2.0f) + cos(tt) ) + sin(dd/2.0f) * sin(tt) * cos(tt) );
	// //float	r_y = sin(dd/2.0f) * (3.0f+ cos(dd/2.0f) * (sqrt(2.0f) + cos(tt) ) + sin(dd/2.0f) * sin(tt) * cos(tt) );
	// //float	r_z = -sin(dd/2.0f) * (sqrt(2.0f) + cos(tt) ) + cos(dd/2.0f) * sin(tt) * cos(tt);
	// //
	// //float4	crab = (float4)( r_x, r_y, r_z);

// col_p = (float4)( (float)(sin(((3.14f*(x)/(GLOBAL))+sin(timer[0]*3.14f/180.0f))*3.14f)*sin(((3.14f*(y)/(GLOBAL))+sin(timer[0]*3.14f/180.0f))*3.14f) ) );
// if (col_p.x < 0.0f)
	// col_p = -col_p;
// col_p.x = (1.0f - col_p.y) * 255.0f;

if ((dd) <= ((6.28f) / 6.0f))
{
	col_p.x = 1.0f;
	col_p.y = 6.0f * ((dd) - 0.0f) / 6.28f;
	col_p.z = 0.0f;
}
else if ((dd) <= (2.0f * 6.28f / 6.0f))
{
	col_p.x = 1.0f + 6.0f * (6.28f / 6.0f - (dd)) / 6.28f;
	col_p.y = 1.0f;
	col_p.z = 0.0f;
}
else if ((dd) <= (3.0f * 6.28f / 6.0f))
{
	col_p.x = 0.0f;
	col_p.y = 1.0f;
	col_p.z = 6.0f * ((dd) - 4.0f * 6.28f / 6.0f) / 6.28f;
}
else if ((dd) <= (4.0f * 6.28f / 6.0f))
{
	col_p.x = 0.0f;
	col_p.y = 1.0f + 6.0f * (3.0f * 6.28f / 6.0f - (dd)) / 6.28f;
	col_p.z = 0.0f;
}
else if ((dd) <= (5.0f * 6.28f / 6.0f))
{
	col_p.x = 6.0f * ((dd) - 4.0f * 6.28f / 6.0f) / 6.28f;
	col_p.y = 0.0f;
	col_p.z = 1.0f;
}
else
{
	col_p.x = 1.0f;
	col_p.y = 0.0f;
	col_p.z = 1.0f + 6.0f * (5.0f * 6.28f / 6.0f - (dd)) / 6.28f;
	// printf("%f, %f, %f\n", col_p.x, col_p.y, col_p.z);
}
col_p *= 255.0f;
// col_p = (float4)(2.0f - (float)(sin(((3.14f*((int)p.x^(int)p.y^(int)p.z)))) + cos(((3.14f*((int)p.x^(int)p.y^(int)p.z)))) ) );
// col_p = (float4)(2.0f - (float)(sin(sin(timer[0]*3.14f/180.0f)+((3.14f*((float)x/(float)l)))) * sin(sin(timer[0]*3.14f/180.0f)+((3.14f*((float)(y)/(float)h)))) ) );
// if (col_p.x < 0.0f)
	// col_p = -col_p;
// col_p *= 255.0f;
// col_p.x = (1.0f - col_p.y) * 360.0f;
// col_p = colorizator

float	r_r = 4.0f * ((1.0f - cos(dd)) / 2.0f);
float	r_x = 0.0f;
float	r_y = 0.0f;
float	r_z = 0.0f;

if ( (dd) >= 0.0f &&  (dd) < 3.14f)
{
	r_x = 6.0f * cos(dd) *  (1.0f + sin(dd)) + r_r * cos(dd) * cos(tt) ;
	r_y = 16.0f * sin(dd) + r_r * sin(dd) * cos(tt);
}
else if ( (dd) >= 3.14f &&  (dd) <= 6.28f)
{
	r_x = 6.0f * cos(dd) *  (1.0f + sin(dd)) + r_r * cos(tt + 3.14f);
	r_y = 16.0f * sin(dd);
}
r_z = r_r * sin(tt);

// float	r_x = 0.0f;
// float	r_y = 0.0f;
// float	r_z = 0.0f;
// 
// r_x = (1.5f + cos(2.0f * dd/2.0f) *sin(tt) - sin(2.0f * dd / 2.0f) * sin(2.0f * tt) ) * cos(dd /2.0f);
// r_y = (1.5f + cos(2.0f * dd/2.0f) *sin(tt) - sin(2.0f * dd / 2.0f) * sin(2.0f * tt) ) * sin(dd /2.0f);
// r_z = sin(2.0f * dd / 2.0f) * sin(tt) + cos(2.0f * dd / 2.0f) * sin(2.0f * tt);


// float	r_x = 0.0f;
// float	r_y = 0.0f;
// float	r_z = 0.0f;
// r_x = 5.0f * (1.0f - dd / (2.0f*3.14f)) * cos(1.50f * dd) * (1.0f + cos(tt)) + 2.0f * cos(1.50f * dd);
// r_y = 5.0f * (1.0f - dd / (2.0f*3.14f)) * cos(1.50f * dd) * (1.0f + cos(tt)) + 2.0f * sin(1.50f * dd);
// r_z = 10.0f * dd/6.28f + 5.0f * (1.0f - dd / 6.28f) * sin(tt);

	// float4		lz = (float4)(18.0f, 28.0f, 2.6666f);
	// float4		lorenz;
	// lorenz.x = lz.x * (crab.y - crab.x);
	// lorenz.y = crab.x * (lz.y - crab.z) - crab.y;
	// lorenz.z = crab.x * crab.y - lz.z * crab.z;
	// crab.x += 0.1f*sin(0.5f*3.14f*lorenz.x);
	// crab.y += 0.1f*sin(0.5f*3.14f*lorenz.y);
	// crab.z += 0.1f*sin(0.5f*3.14f*lorenz.z);
	g = c[0] + ((v1 * (data->g.x)) + (v2 * (data->g.y)));// + (-0.0f + 1.0f * pow((crab), 1.0f)/10.0f);//(-5.0f + 10.0f * exp(-sin(crab)));

float4	crab;
// if (x==100 && y==100)
// printf("%f, %f, %f\n", acos((1.0f-dot(normalize(g-p), g-p)/6.28f)), sin(timer[0]/45.0f), (g-p).x + (g-p).y + (g-p).z);

crab = (float4)(0.0f);
// float4	crab_a = (float4)(r_x, r_y, r_z,0.0f);
// float4	crab_b = (float4)( (4.0f + 2.0f * cos(tt)) * cos(dd+3.14f), (4.0f + 2.0f * cos((tt))) * sin(dd+3.14f), 2.0f * sin(tt) ,0.0f);
// 
// crab = mix_vect(crab_a, crab_b, sin(3.14f*timer[0]/180.0f));


float2	offs = (float2)( ((float)x - (float)l / 2.0f) / (0.25f * l), ((float)y - (float)h / 2.0f) / (0.25f * h));
// offs = (float2)(mad_el.x, mad_el.y);
//( (4.0f + 2.0f * cos(tt)) * cos(dd+3.14f), (4.0f + 2.0f * cos((tt))) * sin(dd+3.14f), 2.0f * sin(tt) ,0.0f);
mad_el.x = offs.x;//( cos(tt)) * cos(dd);//offs.x;
mad_el.y = offs.y;//( cos((tt))) * sin(dd);//offs.y;
#define MAX_ITER 50
#define	ESCAPE	pown(2.0f, 8)

// passer crab en global[global^2]
r_x = 0.0f;
r_y = 0.0f;
r_z = 0.0f;
float	itera = -1;
// float   smooth = 0.0f;
col_p = (float4)0.0f;
	while ( ((int)(++itera < MAX_ITER) && !(pown(mad_el.x ,2) + pown(mad_el.y, 2) > ESCAPE)))
	{
		mad_el.z = mad_el.x;
		mad_el.w = mad_el.y;
		mad_el.x = pown(mad_el.z, 2) - pown(mad_el.w, 2) + (cos(3.14f+3.14f*timer[0]/180.0f)/2.0f);
		mad_el.y = 2.0f * mad_el.z * mad_el.w + (sin(3.14f+3.14f*timer[0]/180.0f)/2.0f);
		// r_x = 5.0f*(-3.14f+1.0f*tt);//(1.0f-2.0f*tt/3.14f);
		// r_y = 5.0f*(-3.14f+1.0f*dd);//(1.0f-2.0f*dd/3.14f);
		// r_z = mad_el.z*mad_el.w;
		// r_z = pow(cos(r_x*r_x), 2) + pow(sin(r_y*r_y),2);//sin(3.14f * 3.14f * r_x * r_x )+cos(3.14f * 3.14f * r_y * r_y);

		col_p.x += exp(-fabs(((float)(log((float)(ESCAPE)))/(+itera*(mad_el.y))))); // Orbit traps
		col_p.y += exp(-fabs(((float)(log((float)(ESCAPE)))/(+itera*(mad_el.x)))));
		col_p.z += exp(-fabs(((float)(log((float)(ESCAPE)))/(+itera*(mad_el.y + mad_el.x)))));
		// col_p.x = ((itera + (log(log((float)itera+ESCAPE)) - log(log((float)mad_el.x + mad_el.y))) / log(2.0f)))/MAX_ITER; // joli
		// col_p.x = (2.0f * sqrt(mad_el.x*mad_el.x+mad_el.y*mad_el.y) * log(sqrt(mad_el.x*mad_el.x+mad_el.y*mad_el.y))/sqrt(mad_el.z*mad_el.z+mad_el.w*mad_el.w));
		// col_p.x = (itera + (log(log((float)ESCAPE)) - log(log((float)mad_el.x + mad_el.y))) / log(2.0f));
		// col_p.y = (itera + (log(log((float)ESCAPE)) - log(log((float)mad_el.x + mad_el.y))) / log(2.0f));
		// col_p.z = (itera + (log(log((float)ESCAPE)) - log(log((float)mad_el.x + mad_el.y))) / log(2.0f));
	}


// smooth = itera + 1.0f - log((log(pow(mad_el.x, 2.0f)+pow(mad_el.y, 2.0f)))/2.0f/log(2.0f))/log(2.0f);
//smooth = (pown(mad_el.x ,2) + pown(mad_el.y, 2)) + 1.0f - log(log(sqrt(mad_el.x*mad_el.x + mad_el.y*mad_el.y))) / log(2.0f);
//smooth = itera+log(log((float)ESCAPE*ESCAPE))/log(2.0f) - log(log(dot(sqrt(mad_el.x*mad_el.x + mad_el.y * mad_el.y), sqrt(mad_el.x*mad_el.x + mad_el.y * mad_el.y))))/log(2.0f);
	// else
	// {;
	barrier(CLK_GLOBAL_MEM_FENCE);
		/*if (itera==MAX_ITER)*/ r_x = (1.0f-tt/3.14f);//((4.0f + 2.0f * cos(tt)) * cos(dd));//(1.0f-tt/3.14f);
		/*if (itera==MAX_ITER)*/ r_y = (1.0f-dd/3.14f);//((4.0f + 2.0f * cos((tt))) * sin(dd));//(1.0f-dd/3.14f);
	if (!x)printf("%f, %f\n", mad_el.x, mad_el.y);
	// col_p = (float)itera / (float)MAX_ITER;
	// col_p += exp(-fabs(mad_el.x+mad_el.y));
	col_p *= 255.0f/(float)itera;
	// if (itera == MAX_ITER)
		// {r_x=500.0f;r_y=r_x;
		// col_p = 0.0f;
		// }
//    col_p = 255.0f*smooth/MAX_ITER;
//	col_p = (col_p / 255.0f) * 1.0f;
//	col_p.x = 1.0f - sin((float)(1 * 3.14f/3.0f)+log(3.14f*col_p.x));
//	col_p.y = 1.0f - sin((float)(2 * 3.14f/3.0f)+log(3.14f*col_p.y));
//	col_p.z = 1.0f - sin((float)(3 * 3.14f/3.0f)+log(3.14f*col_p.z));
//	col_p*=255.0f;
	// col_p.x = (int)(1.0f-sin((float)(1 * 3.14f/3.0f + /*0.1f**/(float)((pown(mad_el.x ,2) + pown(mad_el.y, 2))/4.0f) /** (timer[0]%360)*/ )));
	// col_p.y = (int)(1.0f-sin((float)(2 * 3.14f/3.0f + /*0.1f**/(float)((pown(mad_el.x ,2) + pown(mad_el.y, 2))/4.0f) /** (timer[0]%360)*/ )));
	// col_p.z = (int)(1.0f-sin((float)(3 * 3.14f/3.0f + /*0.1f**/(float)((pown(mad_el.x ,2) + pown(mad_el.y, 2))/4.0f) /** (timer[0]%360)*/ )));
	// col_p *= 255.0f;
	// col_p = clamp(col_p, 0.0f, 255.0f);
	// col_p.x = (int)col_p.x <<8;
	r_z = 2.0f * sin(tt);//+(itera/(float)MAX_ITER);
	// if (itera <= MAX_ITER)
		// r_z = mad_el.y*mad_el.y;

		// col_p = 255.0f* ((float)(itera*(-mad_el.x+mad_el.y)/4.0f) / (float)MAX_ITER);
		// r_x = 0.0f;//5.0f*(-3.14f+1.0f*tt);//r_x = 0.0f;//(1.0f-2.0f*tt/3.14f);//*(float)(((timer[0]) % MAX_ITER) / (float)MAX_ITER);//+mad_el.x;
		// r_y = 0.0f;//5.0f*(-3.14f+1.0f*dd);//r_y = 0.0f;//(1.0f-2.0f*dd/3.14f);//*(float)(((timer[0]) % MAX_ITER) / (float)MAX_ITER);//+mad_el.y;
		// r_z = 0.0f;//10.0f*(1.0f - (float)(((timer[0]) % MAX_ITER) / (float)MAX_ITER));
		mad_el = float4(0.0f, 0.0f, 0.0f, 0.0f);
		// col_p.x = (255.0f * (float)(((timer[0]) % MAX_ITER) / (float)MAX_ITER));
		// col_p.y = (255.0f * (float)(((timer[0]) % MAX_ITER) / (float)MAX_ITER));
		// col_p.x = (int)col_p.x << 16;
		// col_p.y = (int)col_p.y << 8;
	// }

crab = (float4)(r_x, r_y, r_z, 0.0f);
//crab.y *= cos(timer[0]/15.0f);
//crab.x *= sin(timer[0]/15.0f);
// if (!x && !y)
	// printf("%f, %f, %f\n", 255.0f*(float)(((timer[0]) % MAX_ITER) / (float)MAX_ITER), mad_el.z, mad_el.w);

	// g = c[0] +v1*0.5f+v2*0.5f;
	// float	g_x;
	// float	g_y;
	// g_x = fmod(x, 4.0f);
	// g_y = fmod(y, 3.0f);
	// if (!(g_y == 1.0f))
		// g_y = g_y == 0.0f ? -2.0f : 2.0f;
	// if (!(g_x == 1.0f))
	// {
		// g_x = g_x == 0.0f ? 2.0f : g_x;
		// g_x = g_x == 2.0f ? -1.0f : g_x;
		// g_x = g_x == 3.0f ? -2.0f : g_x;
	// }
	// g.x += g_x+(float)(x)/(2.0f*(float)h);
	// g.y += g_y+(float)(y)/(2.0f*(float)h);
	// g.z += ((g_y+g_x+(y+x)/(2.0f*(float)h)));//((float)x/(2.0f*(float)h)) - ((float)y/(2.0f*(float)h));
	// g = c[0] + (v1 * ((data->g.x)+cbrt(tan((float)timer[0]/10.0f)/5.0f)/10.0f)) + (v2 * ((data->g.y+cbrt(tan((float)timer[0]/10.0f)/5.0f)/1.0f)));
//	g.y = -g.y;
//	g.x = -g.x;
	r = dot(normalize(g - p), (g - p)); //dot((float4)clamp(g - p, 0.0f, 1.0f), ((float4)clamp(g - p, 0.0f, 1.0f)));
	k = 0.1f;//100.0f * (1.0f - (clamp(sqrt(r), 0.01f, 0.990f)));//min(sqrt(r), 1.0f);
//	k = 10.0f*normalize((k)) * (s-vit);//normalize((s));
	 k = 100.0f*normalize((k)) * normalize((s-vit));
	 // k = ((g - p)) / (0.0f + dot(normalize(g-p),(g-p))) - k/700.0f;
	// k = ((g - p)) / (dot(s-cbrt(crab),s-cbrt(crab))+0.0f + dot(normalize(g-p),(g-p))) - k/700.0f;
	float4	v = (-(crab))+(g - p);if(!x)printf("%f, %f, %f\n", v.x, v.y, v.z);
	k = (v) / (0.001f + dot(normalize(v/*g-p*/),(v/*g-p*/))) - (k/*+(s-vit)*/) /100.0f;//* 0.5f * (1 + sin(3.14f * timer[0] / 180.0f)) / 400.0f;
	
	// k = (g - p) / (0.1f + dot(normalize(g-p),(g-p))) - (k) /700.0f;//* 0.5f * (1 + sin(3.14f * timer[0] / 180.0f)) / 400.0f;
	// k = (g - p) / (0.01f * dot(normalize(g-p),(g-p))) - (k) /700.0f;//* 0.5f * (1 + sin(3.14f * timer[0] / 180.0f)) / 400.0f;
	// k = (g - p) / (1000.01f + dot(normalize(g-p),(g-p))) - (k) /700.0f;//* 0.5f * (1 + sin(3.14f * timer[0] / 180.0f)) / 400.0f;
	
	// k = cbrt(-cbrt((g-p))+(g - p)) / (0.0f + dot(normalize(g-p),(g-p))) - k/200.0f;
	// k = (((g-p))) / (0.01f + dot(normalize(g-p),(g-p))) - k/8500.0f;//normalize(cbrt(k))*length((g-p));//200.0f;
	acc = k;///30.0f;
	// acc = acc_tmp;

	vit += (acc);

//	if (rel_mov > 0.000009f)
		// p += cbrt(vit) + vit * cbrt(dot(acc,acc));//cbrt(vit)+vit;// + cbrt(acc);//quality cubes m8
//	else
//		p -= vit ;

	p += (vit)/2.0f;p=crab;
	}
	k = normalize(p - cam);
	k = p - dot(cam + data->dir * d, data->dir) * data->dir;
	m_m = (int2)((int)(data->w * dot(k - c[0], normalize(v1)) / length(v1)),
		(int)(data->h * (1.0f - dot(k - c[0], normalize(v2)) / length(v2))));
	r = length(p - g);

//printf("color is : %d\n", hsv(col_p));

	if (m_m[0] >= 0 && m_m[0] < data->w && m_m[1] >= 0 && m_m[1] < data->h &&
		dot(data->dir, p - cam) > 0)
	{
	}
	else
		m_m = (int2)(0);
	int in = m_m[0] + m_m[1] * data->w;
	float len = (p+cam).x+(p+cam).y+(p+cam).z;//dot((p - cam), p - cam);
	if (len < z[in] || z[in] == -1)
	{
		atomic_xchg(&z[in], len);
		atomic_xchg(&i[in], in);
	}
	barrier(CLK_LOCAL_MEM_FENCE);
	// if (!col_p.x && !col_p.y && !col_p.z)
		// col_p =(float4)(255.0f);
	// if (in == i[in])
		img[in] = ((int)col_p.x << 16) + ((int)col_p.y << 8) +col_p.z;//hsv(col_p);// write_imageui(img, (int2)(m_m[0], m_m[1]), (uint4)hsv(col_p));//img[m_m[0] + m_m[1] * data->w] = hsv(col_p);
	pos_par[ind] = p;
	spd_par[ind] = vit;
	acc_par[ind] = acc;
	m_par[ind] = m_m;
	col[ind] = col_p;

//	if (x == 0 && y == 0)printf("timer[0] == %lf\n", (float)timer[0]);//printf("r==%f, ex==%f, ey==%f, ez==%f, eX/r==%f, eY/r==%f, eZ/r==%f\n", r, e.x, e.y, e.z, e.x / r, e.y / r, e.z / r);
//	{printf("rel_mov == %f, old_g.x == %f, old_g.u == %f, g.x == %f, g.y == %f\n", rel_mov, data->old_g.x, data->old_g.y, data->g.x, data->g.y);
//		printf("dir : %5.2f %5.2f %5.2f | %5.2f %5.2f %5.2f | %5.2f %5.2f\n",
//			data->dir.x, data->dir.y, data->dir.z, g.x, g.y, g.z,
//			dot(k - c[0], normalize(v1)) / length(v1),
//			1.0f - dot(k - c[0], normalize(v2)) / length(v2));
//		printf("%5.2f %5.2f %5.2f | %7.4f %7.4f %7.4f | %7.4f %7.4f %7.4f\n",
//			pos_par[ind].x, pos_par[ind].y, pos_par[ind].z,
//			vit.x, p.v.y, p.v.z, acc_par[ind].x, acc_par[ind].y, acc_par[ind].z);
//		printf("c : %5.2f %5.2f %5.2f | %5.2f %5.2f %5.2f | %5.2f %5.2f %5.2f |",
//			c[0].x, c[0].y, c[0].z, c[1].x, c[1].y, c[1].z, c[2].x,
//			c[2].y, c[2].z);
//		printf("%5.2f %5.2f %5.2f | %5.2f %5.2f %5.2f \n",
//			v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);
//		printf("%5.2f %5.2f %5.2f | %d %d | %5.2f %5.2f %5.2f\n",
//			k.x, k.y, k.z, m_par[ind], m_par[ind].y);
//	}
}
