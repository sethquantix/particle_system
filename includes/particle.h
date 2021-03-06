/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   particle.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/27 10:53:29 by cchaumar          #+#    #+#             */
/*   Updated: 2016/07/21 13:31:25 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARTICLE_H
# define PARTICLE_H
# include "../libft/libft.h"
# include "mlx.h"
# include "mlx_opengl.h"
# include "inputs.h"
# include <OpenCL/opencl.h>
# include <OpenCL/cl_ext.h>
# include <OpenGL/gl3.h>
# include <OpenCL/cl_gl.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <time.h>
# include <math.h>
# include "vect.h"
# include <sys/time.h>

# define CGL_APPLE	CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE

# define WIDTH		2560
# define HEIGHT		1440
# define RAD		M_PI / 180.0
# define UGLY		void CL_CALLBACK
# define KERNEL		"kernels/particle.cl"
# define GLOBAL		1280
# define NUM_P		GLOBAL * GLOBAL
# define NUM_KEYS	10
# define SPEED		20
# define RAD_SPEED	1
# define FLOW		1

typedef UGLY					t_callback;

typedef struct					s_particle
{
	cl_int2				m[FLOW];
	cl_float3			p;
	cl_float3			v;
	cl_float3			a;
	cl_float3			lorenz;
	float				lc;
}								t_particle;

typedef struct					s_data
{
	cl_float4			cam;
	cl_float3			dir;
	cl_float3			corners[3];
	cl_float3			g;
	cl_float3			old_g;
	cl_float3			mat[3];
	int					w;
	int					h;
}								t_data;

typedef struct					s_key
{
	int		keycode;
	int		pressed;
	void	(*f)(int, void *);
}								t_key;

typedef struct					s_env
{
	void				*mlx;
	void				*win;
	void				*img;
	int					bpp;
	int					szl;
	int					end;
	int					move;
	cl_context			context;
	cl_device_id		device;
	cl_command_queue	queue;
	cl_program			program;
	cl_mem				particles;
	cl_mem				buf;
	cl_mem				cl_data;
	int					*buff;
	cl_kernel			particle;
	cl_float3			rot;
	char				*source;
	size_t				global[2];
	size_t				local[2];
	t_data				*data;
	cl_float3			corners[3];
	t_key				*keys;
}								t_env;

typedef cl_context_properties	t_props;

extern CGLContextObj	CGLGetCurrentContext(void);
int						loop(t_env *e);
int						key_press(int keycode, t_env *e);
int						key_release(int keycode, t_env *e);
void					info_device(cl_device_id device);
void					init(t_env *e);
t_callback				notify(const char *err, const void *e_private,
	size_t cb, void *data);
void					die(char *error, t_env *e, int exit_code);
void					draw(t_env *e);
void					opencl_init(t_env *e);
t_key					new_key(int keycode, void (*f)(int, void *));
int						mouse_free_hook(int x, int y, t_env *e);
void					move_cam(int keycode, void *e);
void					rotate_cam(int keycode, void *e);

#endif
