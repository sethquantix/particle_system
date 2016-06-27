/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/20 10:08:54 by cchaumar          #+#    #+#             */
/*   Updated: 2016/04/28 21:16:23 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	die(char *error, t_env *e, int exit_code)
{
	ft_printf("%s%s\n", exit_code == EXIT_FAILURE ? "error : " : "", error);
	if (e)
	{
		if (e->p_ray)
			clReleaseProgram(e->p_ray);
		if (e->p_photon)
			clReleaseProgram(e->p_photon);
		if (e->k_ray)
			clReleaseKernel(e->k_ray);
		if (e->k_photon)
			clReleaseKernel(e->k_photon);
		if (e->context)
			clReleaseContext(e->context);
	}
	exit(exit_code);
}

void	gl_tex(t_env *e)
{
	cl_int				err;

	glGenTextures(1, &(e->tex));
	glBindTexture(GL_TEXTURE_2D, e->tex);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, WIDTH, HEIGHT, 0,GL_RGB,
			GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFinish();
	e->img = clCreateFromGLTexture(e->context, CL_MEM_READ_WRITE,
			GL_TEXTURE_2D, 0, e->tex, &err);
	if (err)
		ft_printf("uh ? what was that again ? %d\n", err);
	if (!e->img || err != CL_SUCCESS)
		die("Failed to create OpenGL texture reference!", e, EXIT_FAILURE);
}

void	interop(t_env *e)
{
	CGLContextObj		kCGLContext;
	CGLShareGroupObj	kCGLShareGroup;
	cl_props			properties[3];
	int					err;

	if ((e->mlx = mlx_init()) == NULL)
		die("couldn't start mlx", e, EXIT_FAILURE);
	if ((e->win = mlx_new_opengl_window(e->mlx, WIDTH, HEIGHT, "RT")) == NULL)
		die("Couldn't open a window. I know, it's sad.", e, EXIT_FAILURE);
	kCGLContext = CGLGetCurrentContext();
	kCGLShareGroup = CGLGetShareGroup(kCGLContext);
	properties[0] = CGL_APPLE;
	properties[1] =(cl_props)kCGLShareGroup;
	properties[2] = 0;
	e->context = clCreateContext(properties, 0, NULL, &notify, 0, &err);
 	err = clGetGLContextInfoAPPLE(e->context, kCGLContext,
		CL_CGL_DEVICE_FOR_CURRENT_VIRTUAL_SCREEN_APPLE,
		sizeof(cl_device_id), &e->device, NULL);
	info_device(e->device);
}

int		main(int ac, char **av)
{
	t_env		*e;

	(void)ac;
	(void)av;
	if ((e = ft_memalloc(sizeof(t_env))) == NULL)
		die("couldn't allocate memory for env", e, EXIT_FAILURE);
	if (ac < 2 || ft_strlen(av[1]) <= 3 || av[1][ft_strlen(av[1]) -1] != 'c' || av[1][ft_strlen(av[1]) - 2] != 's' || av[1][ft_strlen(av[1]) - 3] != '.')
		die("bad or missing config file", e, EXIT_FAILURE);
	parse(av[1], e);
	interop(e);
	gl_tex(e);
	init(e);
	mlx_hook(e->win, 2, 3, key_hook, e);
	mlx_loop_hook(e->mlx, &loop, e);
	e->move = 1;
	mlx_loop(e->mlx);
	return (0);
}

