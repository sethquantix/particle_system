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
	p.v = (float3)(0.0f);//0.1f * (float3)(p.p.y, -p.p.x, p.p.z);
	p.a = (float3)(0.0f);
	p.lorenz = p.p;
	p.m = (int2)(0);
	a = a ^ (a << 11);
	a = seed.y ^ (seed.y >> 19) ^ (a ^ (a >> 8));
	p.lc = (1.0f + (float)a / (float)(2 ^ 32 - 1)) / 2.0f;

	p.mass = 1.0f;
	p.density = 0.0f;//fmod(seed.y ^ (seed.y >> 19) ^ (a ^ (a >> 8)), 100.0f);

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

#define RAD 180.0f / 3.14f

void	update_density(__global t_particle *particles, __constant t_data *data, int x, int y, int l)
{
	float	density_2;
	float	density_8;
	float	density_coeff;
	float2	delta;
	float	r2;
	float	z;
	float	density_help;

	density_2 = pow(data->reference_density, 2);
	density_8 = pow(data->reference_density, 8);
	density_coeff = 4.0f / 3.14f / density_8;

	particles[x + y * l].density += 4.0f / 3.14f / density_2;
	delta.x = particles[x + y * l].p.x - particles[x + 1 + y * l].p.x;
	delta.y = particles[x + y * l].p.y - particles[x + 1 + y * l].p.y;
	r2 = pow(delta.x, 2) + pow(delta.y, 2);
	z = -density_2 + r2;
	if (z > 0.0f)
	{
		density_help = density_coeff * pow(z, 3);
		particles[x + y * l].density += density_help;
		particles[x + 1 + y * l].density += density_help;
	}
}

void	update_acceleration(__global t_particle *particles, __constant t_data *data, int x, int y, int l)
{
	float	CO;
	float	Cp;
	float	Cv;
	float	density_2;
	float2	delta;
	float	r2;
	float	q;
	float	u;
	float	wO;
	float	wp;
	float	wv;
	float2	delta_v;

	particles[x + y * l].a.x = 0.0f;
	particles[x + y * l].a.y = data->gravity_strength;

	density_2 = pow(data->reference_density, 2);

	CO = 1.0f / 3.14 / (density_2 * density_2);
	Cp = 15.0f * data->bulk_modulus;
	Cv = -40.0f * data->viscosity;

	delta.x = particles[x + y * l].p.x - particles[x + 1 + y * l].p.x;
	delta.y = particles[x + y * l].p.y - particles[x + 1 + y * l].p.y;
	r2 = pow(delta.x, 2) + pow(delta.y, 2);
	if (r2 < density_2)
	{
		q = sqrt(r2);
		u = 1.0f - q;
		wO = CO * u / particles[x + y * l].density / particles[x + 1 + y * l].density;
		wp = wO * Cp * (particles[x + y * l].density + particles[x + 1 + y * l].density - 2.0f * data->reference_density) * (u / q);
		wv = wO * Cv;
		delta_v.x = particles[x + y * l].v.x - particles[x + 1 + y * l].v.x;
		delta_v.y = particles[x + y * l].v.y - particles[x + 1 + y * l].v.y;
		particles[x + y * l].a.x += wp * delta.x + wv * delta_v.x;
		particles[x + y * l].a.y += wp * delta.y + wv * delta_v.y;
		particles[x + 1 + y * l].a.x -= wp * delta.x + wv * delta_v.x;
		particles[x + 1 + y * l].a.y -= wp * delta.y + wv * delta_v.y;
	}
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


	g = c[0] + (v1 * (data->g.x)) + (v2 * (data->g.y));
	img[p.m.x + p.m.y * data->w] = 0;
	if (x + y * l >= 1048576 - 2)
		return ;
	update_density(particles, data, x, y, l);
	update_acceleration(particles, data, x, y, l) ;
	p.v += p.a;
	p.p += p.v ;





	k = normalize(p.p - cam);
	k = p.p - dot(cam + data->dir * d, data->dir) * data->dir;
	p.m = (int2)((int)(data->w * dot(k - c[0], normalize(v1)) / length(v1)),
		(int)(data->h * (1.0f - dot(k - c[0], normalize(v2)) / length(v2))));
	r = length(p.p - g);
	col = (float3)(clamp(1.0f - r / d, 0.0f, 1.0f));
	col.x = (1.0f - col.x) * 360.0f;
	col = (float3)(1.0f,1.0f,1.0f);
	if (p.m.x >= 0 && p.m.x < data->w && p.m.y >= 0 && p.m.y < data->h &&
		dot(data->dir, p.p - cam) > 0)
		img[p.m.x + p.m.y * data->w] = hsv(col);
	else
		p.m = (int2)(0);
	particles[x + y * l] = p;
}
