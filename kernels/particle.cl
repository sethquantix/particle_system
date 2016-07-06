#include "particle.hl"

__kernel void init(__global t_particle *particles, uint2 seed)
{
	int			x = get_global_id(0);
	int			y = get_global_id(1);
	int			l = get_global_size(0);
	int			h = get_global_size(1);
	uint		a = seed.x + x + y * l;
	t_particle	p;

	float t = ((float)x / (float)l) * 2.0f * M_PI;
	float d = ((float)y / (float)h) * 2.0f * M_PI;
	p.p = (float3)(cos(d) * cos(t), cos(d) * sin(t), sin(d));
	p.v = 0.1f * (float3)(p.p.y, -p.p.x, p.p.z);
	p.a = (float3)(0.0f);
	p.m = (int2)(0);
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
	float		r = length(cam);
	float		d = r;

	c[0] = data->corners[0] * d + cam;
	c[1] = data->corners[1] * d + cam;
	c[2] = data->corners[2] * d + cam;
	v1 = c[1] - c[0];
	v2 = c[2] - c[0];
	g = c[0] + v1 * data->g.x + v2 * data->g.y;
	img[p.m.x + p.m.y * data->w] = 0;
	p.a = r * 0.01f * (g - p.p) / (0.1f + dot(g - p.p, g - p.p));
	p.v += p.a;
	p.p += p.v;
	k = normalize(p.p - cam);
	k = p.p - dot(cam + data->dir * d, data->dir) * data->dir;
	p.m = (int2)((int)(data->w * dot(k - c[0], normalize(v1)) / length(v1)),
		(int)(data->h * (1.0f - dot(k - c[0], normalize(v2)) / length(v2))));
	r = length(p.p - g);
	col = (float3)(clamp(1.0f - r / d, 0.0f, 1.0f));
	col.x = (1.0f - col.x) * 360.0f;
	if (p.m.x >= 0 && p.m.x < data->w && p.m.y >= 0 && p.m.y < data->h &&
		dot(data->dir, p.p - cam) > 0)
		img[p.m.x + p.m.y * data->w] = hsv(col);
	else
		p.m = (int2)(0);
	particles[x + y * l] = p;
	if (x == 0 && y == 0 && 1 == 1)
	{
		printf("dir : %5.2f %5.2f %5.2f | %5.2f %5.2f %5.2f | %5.2f %5.2f\n",
			data->dir.x, data->dir.y, data->dir.z, g.x, g.y, g.z,
			dot(k - c[0], normalize(v1)) / length(v1),
			1.0f - dot(k - c[0], normalize(v2)) / length(v2));
		printf("%5.2f %5.2f %5.2f | %7.4f %7.4f %7.4f | %7.4f %7.4f %7.4f\n",
			p.p.x, p.p.y, p.p.z,
			p.v.x, p.v.y, p.v.z, p.a.x, p.a.y, p.a.z);
		printf("c : %5.2f %5.2f %5.2f | %5.2f %5.2f %5.2f | %5.2f %5.2f %5.2f |",
			c[0].x, c[0].y, c[0].z, c[1].x, c[1].y, c[1].z, c[2].x,
			c[2].y, c[2].z);
		printf("%5.2f %5.2f %5.2f | %5.2f %5.2f %5.2f \n",
			v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);
		printf("%5.2f %5.2f %5.2f | %d %d | %5.2f %5.2f %5.2f\n",
			k.x, k.y, k.z, p.m.x, p.m.y);
	}
}
