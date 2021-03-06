#include "particle.hl"

__kernel void init(__global t_particle *particles, uint2 seed)
{
	int			x = get_global_id(0);
	int			y = get_global_id(1);
	int			l = get_global_size(0);
	int			h = get_global_size(1);
	uint		a = seed.x + x + y * l;
	t_particle	p;
	int			i = -1;

	float t = ((float)x / (float)l) * 2.0f * M_PI;
	float d = ((float)y / (float)h) * 2.0f * M_PI;
	p.p = (float3)(cos(d) * cos(t), cos(d) * sin(t), sin(d));
	p.v = 0.1f * (float3)(p.p.x, -p.p.z, p.p.y);
	p.a = (float3)(0.0f);
	p.lorenz = p.p;
	while (++i < FLOW)
		p.m[i] = (int2)(0);
	a = a ^ (a << 11);
	a = seed.y ^ (seed.y >> 19) ^ (a ^ (a >> 8));
	p.lc = (1.0f + (float)a / (float)(2 ^ 32 - 1)) / 2.0f;
	particles[x + y * l] = p;
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

int		attenuate(int3 col)
{
	return ((((int)((float)col.x * 0.8f)) << 16) +
			(((int)((float)col.y * 0.8f)) << 8) +
			((int)((float)col.z * 0.8f)));
}

__kernel void particle(__global t_particle *particles, __constant t_data *data,
	int n, __global int *img)
{
	int			x = get_global_id(0);
	int			y = get_global_id(1);
	int			l = get_global_size(0);
	int			h = get_global_size(1);
	t_particle	p = particles[x + y * l];
	float3		k;
	float3		col;
	float3		cam = data->cam.xyz;
	float3		c[3];
	float3		v1;
	float3		v2;
	float3		g;
	int2		m;
	float		d = length(cam);
	float3		lz = (float3)(1.0f, 16.0f, 2.3f);
	int			i = FLOW - 1;
	int			color;
	float		r;

	c[0] = data->corners[0] * d + cam;
	c[1] = data->corners[1] * d + cam;
	c[2] = data->corners[2] * d + cam;
	v1 = c[1] - c[0];
	v2 = c[2] - c[0];
	p.lorenz.x = lz.x * (p.p.y - p.p.x);
	p.lorenz.y = p.p.x * (lz.y - p.p.z) - p.p.y;
	p.lorenz.z = p.p.x * p.p.y - lz.z * p.p.z;

	g = !LORENZ * (c[0] + (v1 * (data->g.x)) + (v2 * (data->g.y))) +
		LORENZ * p.lorenz;

	img[p.m[i].x + p.m[i].y * data->w] = 0;
	r = dot(g - p.p, g - p.p);
	k = 1.0f * clamp(1.0f - sqrt(r), 0.0f, 1.0f) * dot(p.v, p.v) * p.v;
	if (LORENZ)
		p.a = 100.0f * (g - p.p) / (0.01f + r);
	else
		p.a = 0.01f * (g - p.p) / (0.01f + r);
	if (LORENZ)
		p.v = p.a;
	else
		p.v += p.a;
	p.p += p.v;
	k = normalize(p.p - cam);
	k = p.p - dot(cam + data->dir * d, data->dir) * data->dir;
	m = (int2)((int)(data->w * dot(k - c[0], normalize(v1)) / length(v1)),
		(int)(data->h * (1.0f - dot(k - c[0], normalize(v2)) / length(v2))));
	g = c[0] + (v1 * (data->g.x)) + (v2 * (data->g.y));
	r = length(p.p - g);
	if (x == 0 && y == 0)
		printf("%f\n", (9.0f + log(dot(p.v, p.v))) / 11.0f);
	col = (float3)(clamp((9.0f + log(dot(p.v, p.v))) / 11.0f, 0.2f, 1.0f));
	col.x = ((1.0f - col.x) / 0.8f) * 360.0f;
//	col = (float3)(1.0f, 1.0f, 1.0f);
	if (m.x >= 0 && m.x < data->w && m.y >= 0 && m.y < data->h &&
		dot(data->dir, p.p - cam) > 0)
		img[m.x + m.y * data->w] = hsv(col);
	else
		m = (int2)(0);
	while (i-- > 0)
	{
		p.m[i + 1] = p.m[i];
		color = img[p.m[i + 1].x + p.m[i + 1].y * data->w];
		img[p.m[i + 1].x + p.m[i + 1].y * data->w] =
			attenuate((int3)(color >> 16, (color >> 8) & 0xff, color & 0xff));
	}
	p.m[0] = m;
	particles[x + y * l] = p;
//		printf("dir : %5.2f %5.2f %5.2f | %5.2f %5.2f %5.2f | %5.2f %5.2f\n",
//			data->dir.x, data->dir.y, data->dir.z, g.x, g.y, g.z,
//			dot(k - c[0], normalize(v1)) / length(v1),
//			1.0f - dot(k - c[0], normalize(v2)) / length(v2));
//		printf("%5.2f %5.2f %5.2f | %7.4f %7.4f %7.4f | %7.4f %7.4f %7.4f\n",
//			p.p.x, p.p.y, p.p.z,
//			p.v.x, p.v.y, p.v.z, p.a.x, p.a.y, p.a.z);
//		printf("c : %5.2f %5.2f %5.2f | %5.2f %5.2f %5.2f | %5.2f %5.2f %5.2f |",
//			c[0].x, c[0].y, c[0].z, c[1].x, c[1].y, c[1].z, c[2].x,
//			c[2].y, c[2].z);
//		printf("%5.2f %5.2f %5.2f | %5.2f %5.2f %5.2f \n",
//			v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);
//		printf("%5.2f %5.2f %5.2f | %d %d | %5.2f %5.2f %5.2f\n",
//			k.x, k.y, k.z, p.m.x, p.m.y);
}
