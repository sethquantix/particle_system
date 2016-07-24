#include "particle.hl"

__kernel void init(uint2 seed, __global float3 *pos_par, __global float3 *acc_par, __global float3 *spd_par, __global float *lc_par, __global int2 *m_par, __global float3 *col)
{
	int			x = get_global_id(0);
	int			y = get_global_id(1);
	int			l = get_global_size(0);
	int			h = get_global_size(1);
	uint		a = seed.x + x + y * l;
	int			ind = x + y * l;

	float t = ((float)x / (float)l) * 2.0f * M_PI;
	float d = ((float)y / (float)h) * 2.0f * M_PI;
//	pos_par[ind] = (float3)(10.0f*cos(d)*cos(t), 10.0f*cos(d)*sin(t), 10.0f*cos(d*t)*sin(t*d)); // crab
//	pos_par[ind] = (float3)(10.0f*cos(d*t)*cos(t*d), 10.0f*cos(d*t)*sin(t*d), 10.0f*cos(d*t)*sin(t*d)); // torus
//	pos_par[ind] = (float3)(10.0f*cos(d*t)-cos(t*d), 10.0f*cos(d*t)*sin(t*d), 10.0f*cos(d*t)*sin(t*d)); // infinity
//	pos_par[ind] = (float3)(-5.0f + 10.0f * (float)x/(float)h*2.0f, -5.0f + 10.0f * (float)y/(float)l*2.0f, -5.0f + 10.0f * ((float)(cos((float)x*x*x)*cos((float)y*y*y)))); // cube
//	pos_par[ind] = (float3)( 10.0f*sin(t)*cos(d), 10.0f*sin(t)*sin(d), 10.0f*cos(t)); // sphere
//	pos_par[ind] = (float3)((float)x/(float)h*2.0f, (float)y/(float)l*2.0f,  (cbrt((float)((x*x*x)*(y*y*y))))/(float)(h)); // sandwich
	pos_par[ind] = (float3)((float)(x)/h, (float)(x^y)/(float)l, (float)(x+y)/(l+h));

//	spd_par[ind] = 0.1f * (float3)(10.0f*cos(d) * cos(t), -10.0f*cos(d) * sin(t), 10.0f*sin(t));
	spd_par[ind] = (float3)(0.0f);
//	spd_par[ind] = 0.01f * (float3)(pos_par[ind].y, -pos_par[ind].x, pos_par[ind].z);
	acc_par[ind] = (float3)(0.0f);
	m_par[ind] = (int2)(0);
	a = a ^ (a << 11);
	a = seed.y ^ (seed.y >> 19) ^ (a ^ (a >> 8));
	lc_par[ind] = (1.0f + (float)a / (float)(2 ^ 32 - 1)) / 2.0f;
	col[ind] = (float3)(clamp((float)(ind)/(GLOBAL + GLOBAL * l), 0.0f, 1.0f));
	//col[ind] = (float3)(1.0f);
	col[ind].x = (1.0f - col[ind].y) * 360.0f;
}

int		hsv(float3 c)
{
	int		ti = (int)(c.x / 60.0f) % 6;
	float	f = c.x / 60.0f - ti;
	float	l = c.z * (1.0f - c.y);
	float	m = c.z * (1.0f - f * c.y);
	float	n = c.z * (1.0f - (1.0f - f) * c.y);
	float3	rgb;

	rgb = 255.0f * (float3)(c.z * (ti == 0 || ti == 5) + m * (ti == 1) +
			l * (ti == 2 || ti == 3) + n * (ti == 4),
			c.z * (ti == 1 || ti == 2) + m * (ti == 3) +
			l * (ti > 3) + n * (ti == 0),
			c.z * (ti == 3 || ti == 4) + m * (ti == 5) +
			l * (ti < 2) + n * (ti == 2));
	return (((int)rgb.x << 16) + ((int)rgb.y << 8) + (int)rgb.z);
}

#define RAD 180.0f / 3.14f

__kernel void particle(__global float3 *pos_par, __global float3 *acc_par, __global float3 *spd_par, __global float *lc_par, __global int2 *m_par, __global float3 *col, 
	__constant t_data *data, __global int *img, __constant long *timer)
{
	int			x = get_global_id(0);
	int			y = get_global_id(1);
	int			l = get_global_size(0);
	int			h = get_global_size(1);
	int			ind = x + y * l;
	float3		k;
	float3		cam = data->cam.xyz;
	float3		c[3];
	float3		v1;
	float3		v2;
	float3		g;
	float		d = length(cam);
	float		r;
	//float		rel_mov = ((pow(data->old_g.x - data->g.x, 2)) + (pow(data->old_g.y - data->g.y, 2)));

	float3		p = pos_par[ind] ;
	float3		acc = acc_par[ind];
	float3		vit = spd_par[ind];
	int2		m_m = m_par[ind];
	float3		col_p = col[ind];

	c[0] = data->corners[0] * d + cam;
	c[1] = data->corners[1] * d + cam;
	c[2] = data->corners[2] * d + cam;
	v1 = c[1] - c[0];
	v2 = c[2] - c[0];

	img[m_m[0] + m_m[1] * data->w] = 0;
	// write_imageui(img, (int2)(m_m[0], m_m[1]), (uint4)0);
	float3	s = dot(vit, vit) * vit;

	float tt = ((float)x / (float)l) * 2.0f * M_PI;
	float dd = ((float)y / (float)h) * 2.0f * M_PI;
	// float3	crab = (float3)((float)x/(float)h*2.0f, (float)y/(float)l*2.0f, ((float)(cos((float)x*x*x)*cos((float)y*y*y))));
	// float3	crab = (float3)(10.0f*cos(dd*tt)*cos(tt*dd), 10.0f*cos(dd*tt)*sin(tt*dd), 10.0f*cos(dd*tt)*sin(tt*dd)); // crab;
	// float3	crab = (float3)(10.0f*cos(dd*tt)*cos(tt*dd), 10.0f*cos(dd*tt)*sin(tt*dd), 10.0f*cos(dd*tt)*sin(tt*dd)); 
	// float3	crab = (float3)(10.0f*sin(tt)*cos(dd), 10.0f*sin(tt)*sin(dd), 10.0f*cos(tt));
	float3	crab = (float3)((float)(x)/h +(cos((float)timer[0]/10.0f)/2.0f), (float)(x^y)/(float)l+(sin((float)timer[0]/10.0f)/2.0f), (float)(x+y)/(l)+(tanh((float)timer[0]/10.0f)/2.0f));//(float3)((float)(x)/l, (float)(x^y)/(float)l, (x+y)/l);
//	float3	crab = (float3)((float)(x % h), (float)(y ^ l), (float)((x + y)%(l + h)));
	//	float3	crab = (float3)((float)(x)/h, (float)(x|y)/(float)l, (float)(x+y)/(l+h));
	// float3	crab = (float3)(10.0f*cos(dd)*cos(tt), 10.0f*cos(dd)*sin(tt), 10.0f*cos(dd*tt)*sin(tt*d));
	g = c[0] + ((v1 * (data->g.x)) + (v2 * (data->g.y))) + (-5.0f + 10.0f * exp(-sin(crab)));
//	g = c[0] + (v1 * ((data->g.x)+cbrt(tan((float)timer[0]/10.0f)/5.0f)/10.0f)) + (v2 * ((data->g.y+cbrt(tan((float)timer[0]/10.0f)/5.0f)/10.0f)));
//	g.y = -g.y;
//	g.x = -g.x;
	r = dot((float3)clamp(g - p, 0.0f, 1.0f), ((float3)clamp(g - p, 0.0f, 1.0f)));
	k = 100.0f * (1.0f - (clamp(1.0f - sqrt(r), 0.0f, 1.0f)));//min(sqrt(r), 1.0f);
	k = k * s;
	k = (g - p) / (1.0f + dot(g-p,g-p)) - k/1000.0f;
	acc = k;
	vit += acc;

//	if (rel_mov > 0.000009f)
		// p += cbrt(vit) + vit * cbrt(dot(acc,acc));//cbrt(vit)+vit;// + cbrt(acc);//quality cubes m8
//	else
//		p -= vit ;

	p += (vit);

	k = normalize(p - cam);
	k = p - dot(cam + data->dir * d, data->dir) * data->dir;
	m_m = (int2)((int)(data->w * dot(k - c[0], normalize(v1)) / length(v1)),
		(int)(data->h * (1.0f - dot(k - c[0], normalize(v2)) / length(v2))));
	r = length(p - g);

//printf("color is : %d\n", hsv(col_p));

	if (m_m[0] >= 0 && m_m[0] < data->w && m_m[1] >= 0 && m_m[1] < data->h &&
		dot(data->dir, p - cam) > 0)
		img[m_m[0] + m_m[1] * data->w] = hsv(col_p);// write_imageui(img, (int2)(m_m[0], m_m[1]), (uint4)hsv(col_p));//img[m_m[0] + m_m[1] * data->w] = hsv(col_p);
	else
		m_m = (int2)(0);

		pos_par[ind] = p;
		spd_par[ind] = vit;
		acc_par[ind] = acc;
		m_par[ind] = m_m;
		col[ind] = col_p;

	if (x == 0 && y == 0)printf("timer[0] == %lf\n", (float)timer[0]);//printf("r==%f, ex==%f, ey==%f, ez==%f, eX/r==%f, eY/r==%f, eZ/r==%f\n", r, e.x, e.y, e.z, e.x / r, e.y / r, e.z / r);
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
