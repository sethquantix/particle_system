/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/20 10:08:54 by cchaumar          #+#    #+#             */
/*   Updated: 2016/07/01 19:20:49 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "particle.h"

void	die(char *error, t_env *e, int exit_code)
{
	ft_printf("%s%s\n", exit_code == EXIT_FAILURE ? "error : " : "", error);
	if (e->program)
		clReleaseProgram(e->program);
	if (e->particle)
		clReleaseKernel(e->particle);
	if (e->context)
		clReleaseContext(e->context);
	if (e->buf)
		clReleaseMemObject(e->buf);
	if (e->particles)
		clReleaseMemObject(e->particles);
	exit(exit_code);
}

void	interop(t_env *e)
{
	CGLContextObj		kCGLContext;
	CGLShareGroupObj	kCGLShareGroup;
	t_props			properties[3];
	int					err;

	if ((e->mlx = mlx_init()) == NULL)
		die("couldn't start mlx", e, EXIT_FAILURE);
	if ((e->win = mlx_new_opengl_window(e->mlx, WIDTH, HEIGHT, "RT")) == NULL)
		die("Couldn't open a window. I know, it's sad.", e, EXIT_FAILURE);
	kCGLContext = CGLGetCurrentContext();
	kCGLShareGroup = CGLGetShareGroup(kCGLContext);
	properties[0] = CGL_APPLE;
	properties[1] = (t_props)kCGLShareGroup;
	properties[2] = 0;
	e->context = clCreateContext(properties, 0, NULL, &notify, 0, &err);
 	if (err)
		ft_printf("Error : %d\n", err);
	err = clGetGLContextInfoAPPLE(e->context, kCGLContext,
		CL_CGL_DEVICE_FOR_CURRENT_VIRTUAL_SCREEN_APPLE,
		sizeof(cl_device_id), &e->device, NULL);
 	if (err)
		die("Error getting shared device", e, EXIT_FAILURE);
	info_device(e->device);
}

void	functions(t_env *e)
{
	e->keys[0] = new_key(0xC, &move_cam); //Q
	e->keys[1] = new_key(0xD, &move_cam); //W
	e->keys[2] = new_key(0xE, &move_cam); //E
	e->keys[3] = new_key(0x0, &move_cam); //A
	e->keys[4] = new_key(0x1, &move_cam); //S
	e->keys[5] = new_key(0x2, &move_cam); //D
	e->keys[6] = new_key(0x07B, &rotate_cam); //LEFT
	e->keys[7] = new_key(0X07D, &rotate_cam); //DOWN
	e->keys[8] = new_key(0x07C, &rotate_cam); //RIGHT
	e->keys[9] = new_key(0x07E, &rotate_cam); //UP
}

void	values(t_env *e)
{
	cl_float2	d;

	e->buff = (int *)mlx_get_data_addr(e->img, &e->bpp, &e->szl, &e->end);
	if ((e->data = ft_memalloc(sizeof(t_data))) == NULL)
		die("Not enough memory", e, EXIT_FAILURE);
	if ((e->keys = ft_memalloc(sizeof(t_key) * NUM_KEYS)) == NULL)
		die("Not enough memory", e, EXIT_FAILURE);
	e->rot = float3(0, 0, 0);
	d = float2(tan(M_PI / 6), tan(M_PI / 6) * (float)HEIGHT / (float)WIDTH);
	e->move = 1;
	e->data->cam = float4(0, 0, -100, 100);
	e->data->cam.w = length(float3(e->data->cam.x, e->data->cam.y,
		e->data->cam.z));
	e->data->g = float3(0.5f, 0.5f, 0.0f);
	e->data->old_g = float3(0.5f, 0.5f, 0.0f);

//	e->data->reference_density = 1000.0f;
//	e->data->bulk_modulus = pow(10, 3);
//	e->data->viscosity = 0.1f;
//	e->data->gravity_strength = 9.8f;

	e->corners[0] = float3(-d.x, d.y, 1);
	e->corners[1] = float3(d.x, d.y, 1);
	e->corners[2] = float3(-d.x, -d.y, 1);
	e->data->w = WIDTH;
	e->data->h = HEIGHT;
	functions(e);
}

int		main(void)
{
	t_env		e;

	if ((e.mlx = mlx_init()) == NULL)
		die("couldn't start mlx", &e, EXIT_FAILURE);
	if ((e.win = mlx_new_window(e.mlx, WIDTH, HEIGHT, "particle")) == NULL)
		die("Window broke.", &e, EXIT_FAILURE);
	if ((e.img = mlx_new_image(e.mlx, WIDTH, HEIGHT)) == NULL)
		die("Not enough memory", &e, EXIT_FAILURE);
	values(&e);
	opencl_init(&e);
	ft_printf("max texture size : %zu\n",GL_MAX_TEXTURE_SIZE);
	mlx_hook(e.win, KeyPress, KeyPressMask, &key_press, &e);
	mlx_hook(e.win, KeyRelease, KeyReleaseMask, &key_release, &e);
	mlx_hook(e.win, 6, 64, &mouse_free_hook, &e);
	mlx_loop_hook(e.mlx, &loop, &e);
	mlx_loop(e.mlx);
	return (0);
}

