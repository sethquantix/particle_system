/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/20 10:09:57 by cchaumar          #+#    #+#             */
/*   Updated: 2016/04/28 22:00:43 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H
# include "libft.h"
# define _gl_h_
# include "mlx.h"
# include "mlx_opengl.h"
# include <OpenCL/opencl.h>
# include <OpenCL/cl_ext.h>
# include <OpenGL/gl3.h>
# include <OpenCL/cl_gl.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <math.h>

# define CGL_APPLE	CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE
# undef				main

# define WIDTH		2560
# define HEIGHT		1440

# define POINT		0
# define SPHERE		1
# define CYLINDER	2
# define CONE		3
# define PLANE		4

typedef struct					s_env
{
	void				*mlx;
	void				*win;
	int					move;
	GLuint				tex;
	cl_context			context;
	cl_mem				img;
	cl_device_id		device;
	cl_command_queue	queue;
	cl_program			p_ray;
	cl_program			p_photon;
	cl_kernel			k_ray;
	cl_kernel			k_photon;
}								t_env;

typedef cl_context_properties		cl_props;

extern CGLContextObj	CGLGetCurrentContext(void);
int						loop(t_env *e);
int						key_hook(int keycode, t_env *e);
void					info_device(cl_device_id device);
void					init(t_env *e);
void CL_CALLBACK		notify(const char *err, const void *e_private,
	size_t cb, void *data);
void					die(char *error, t_env *e, int exit_code);
void					parse(char *filename, t_env *e);
void					grammar(t_env *e);

#endif
