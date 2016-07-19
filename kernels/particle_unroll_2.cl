#include "particle.hl"

__kernel void init(uint2 seed, __global float3 *pos_par, __global float3 *acc_par, __global float3 *spd_par, __global float *lc_par, __global int2 *m_par, __global float *mass)
{
	int			x = get_global_id(0);
	int			y = get_global_id(1);
	int			l = get_global_size(0);
	int			h = get_global_size(1);
	uint		a = seed.x + x + y * l;
	int			ind = x + y * l;

	float t = ((float)x / (float)l) * 2.0f * M_PI;
	float d = ((float)y / (float)h) * 2.0f * M_PI;
	pos_par[ind] = (float3)(d*cos(d) * cos(t), d*cos(d) * sin(t), d*sin(d));
	spd_par[ind] = 0.1f * (float3)(pos_par[ind].y, -pos_par[ind].x, pos_par[ind].z);
	acc_par[ind] = (float3)(0.0f);
	m_par[ind] = (int2)(0);
	a = a ^ (a << 11);
	a = seed.y ^ (seed.y >> 19) ^ (a ^ (a >> 8));
	mass[ind] = cos(d) * cos(t)+2.0f;
	lc_par[ind] = (1.0f + (float)a / (float)(2 ^ 32 - 1)) / 2.0f;
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

/*

	t_struct tonton[10] = {
		[0] = {
			.champa = ;
			.champB = ;
		},
		[1] = {

		}

	}

*/

__kernel void particle(__global float3 *pos_par, __global float3 *acc_par, __global float3 *spd_par, __global float *lc_par, __global int2 *m_par, __global float *mass, 
	__constant t_data *data, __global int *img)
{
	int			x = get_global_id(0);
	int			y = get_global_id(1);
	int			l = get_global_size(0);
	int			h = get_global_size(1);
	int			ind = x + y * l;
	float3		k;
	float3		col;
	float3		cam = data->cam.xyz;
	float3		c[3];
	float3		v1;
	float3		v2;
	float3		g;
	float		d = length(cam);
	float		r;
	float		rel_mov = ((pow(data->old_g.x - data->g.x, 2)) + (pow(data->old_g.y - data->g.y, 2)));

	__private	float3		p = pos_par[ind] ;
	__private	float3		acc = acc_par[ind];
	__private	float3		vit = spd_par[ind];
	__private	int2		m_m = m_par[ind];
	__private	float		mass_p = mass[ind];

	c[0] = data->corners[0] * d + cam;
	c[1] = data->corners[1] * d + cam;
	c[2] = data->corners[2] * d + cam;
	v1 = c[1] - c[0];
	v2 = c[2] - c[0];

	img[m_m[0] + m_m[1] * data->w] = 0;
	float3	s = dot(vit, vit) * vit;
	int	j;

	acc = 0.0f;

	j = 0;
	while (j < 16384)
	{
			g = pos_par[j] - p;
			r = dot(g, g);
			k =  100.0f * (1.0f - (r < 1.0f ? 1.0f : r));
			k *= s;
			k = g / (r*10.0f > 7.1f : r : 7.1f) - k;// max(r*10.0f, 7.1f)) - k;
			acc += k;
	j++;					
	}
	acc /= 16384;

	//barrier(CLK_LOCAL_MEM_FENCE);

	//g = c[0] + (v1 * (data->g.x)) + (v2 * (data->g.y)) ;
	//r = dot((float3)clamp(g - p, 0.0f, 1.0f), ((float3)clamp(g - p, 0.0f, 1.0f)));
	//k = 100.0f * (1.0f - min((r), 1.0f));//(clamp(1.0f - (r), 0.0f, 1.0f));
	//k = k * s;
	//k = g / (1000.0f) - k/1000.0f;
	//acc += k;

	//acc =  acc + 0.1f * ((((g - p))) / (100.00001f + max(r-1.0f, 7.1f)) ) - k/100.0f;
	//acc =  acc + 1.0f * ((((g - p))) / (10.00001f + (r)) ) - k/100.0f;
	//acc =  0.1f*(((g - p))) / (0.1f + length(g - p) * length(g - p) * dot((g - p), (g - p)));
	//acc = 0.001f * (-9.8f) * ( normalize(g - p) ) / length(g - p);
	vit += acc;



//	vit = vit /(1.0f+(g - p)*rel_mov*1.0f.0f);
	//acc = r * 10.0f * (p.lorenz) / (0.01f + dot(p, p));
	//vit = acc;
	//vit += r * 0.1f * (g - p) / (0.01f + dot(g - p, g - p)); // grand galop spiralien
//	if (rel_mov > 0.000009f)
		p += vit ;//-acc;
//	else
//		p -= vit ;
	k = normalize(p - cam);
	k = p - dot(cam + data->dir * d, data->dir) * data->dir;
	m_m = (int2)((int)(data->w * dot(k - c[0], normalize(v1)) / length(v1)),
		(int)(data->h * (1.0f - dot(k - c[0], normalize(v2)) / length(v2))));
	r = length(p - g);
	col = (float3)(clamp(1.0f -  (r / d) * cbrt(vit), 0.0f, 1.0f));
	col.x = (1.0f - col.x) * 360.0f;

//	col = (float3)(1.0f, 1.0f, 1.0f);
	if (m_m[0] >= 0 && m_m[0] < data->w && m_m[1] >= 0 && m_m[1] < data->h &&
		dot(data->dir, p - cam) > 0)
		img[m_m[0] + m_m[1] * data->w] = hsv(col);
	else
		m_m = (int2)(0);
		pos_par[ind] = p;
		spd_par[ind] = vit;
		acc_par[ind] = acc;
		m_par[ind] = m_m;
//	if (x == 0 && y == 0 && 1 == 1)printf("r==%f, ex==%f, ey==%f, ez==%f, eX/r==%f, eY/r==%f, eZ/r==%f\n", r, e.x, e.y, e.z, e.x / r, e.y / r, e.z / r);
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
