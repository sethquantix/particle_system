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
	if (e->img)
		mlx_destroy_image(e->mlx, e->img_affich);
	if (e->win)
		mlx_destroy_window(e->mlx, e->win);
	if (e->mlx)
		free(e->mlx);
	exit(exit_code);
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
	e->keys[10] = new_key(0x031, &toggle); //UP
}

void	values(t_env *e)
{
	cl_float2	d;

	e->buff = (int *)malloc(sizeof(int) * WIDTH * HEIGHT);//(int *)mlx_get_data_addr(e->img, &e->bpp, &e->szl, &e->end);
	//int i = -1;
	//while(++i < WIDTH*HEIGHT)
	//{
	//	printf("essai : i == %d\n", i);
	//	e->buff[i] = 0;
	//}
	e->data = &e->data_tt;
	e->keys = e->keys_tt;
	e->rot = float3(0, 0, 0);
	d = float2(tan(M_PI / 6), tan(M_PI / 6) * (float)HEIGHT / (float)WIDTH);
	e->move = 1;
	e->data->cam = float4(0, 0, -100, 100);
	e->data->cam.w = length(float3(e->data->cam.x, e->data->cam.y,
		e->data->cam.z));
	e->data->g = float3(0.5f, 0.5f, 0.0f);
	e->data->old_g = float3(0.5f, 0.5f, 0.0f);
	e->corners[0] = float3(-d.x, d.y, 1);
	e->corners[1] = float3(d.x, d.y, 1);
	e->corners[2] = float3(-d.x, -d.y, 1);
	e->data->w = WIDTH;
	e->data->h = HEIGHT;
	functions(e);
}

void	interop(t_env *e)
{
	CGLContextObj		kCGLContext;
	CGLShareGroupObj	kCGLShareGroup;
	t_props			properties[3];
	int					err;

	if ((e->mlx = mlx_init()) == NULL)
		die("couldn't start mlx", e, EXIT_FAILURE);
	if ((e->win = mlx_new_opengl_window(e->mlx, WIDTH, HEIGHT, "particles")) == NULL)
		die("Couldn't open a window. I know, it's sad.", e, EXIT_FAILURE);
	mlx_opengl_window_set_context(e->win);
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

glGenFramebuffers(1, &e->framebuffer);
glBindFramebuffer(GL_FRAMEBUFFER, e->framebuffer);


	//glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &e->texture[0]);
	glBindTexture(GL_TEXTURE_2D, e->texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_INT, NULL);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, e->texture[0], 0);

//GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
//glDrawBuffers(1, DrawBuffers);
//
glBindFramebuffer(GL_FRAMEBUFFER, e->framebuffer);
//glViewport(0, 0, WIDTH, HEIGHT);

	int	ret;
	e->img = clCreateFromGLTexture(e->context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, e->texture[0], &ret);
	printf("AXAXAXA\n");
	if (!e->img)
		printf("u dun goofd m8\n");
	if (ret)
		printf("Could not create Tex2D, error : %d\n", ret);
	e->buf = clCreateBuffer(e->context, CL_MEM_READ_WRITE, sizeof(cl_int) * WIDTH * HEIGHT, NULL, &ret);
	if (ret)
		printf("Could not create CLBuffer, error : %d\n", ret);

	//info_device(e->device);
}

int		main(void)
{
	t_env		e;

	if ((e.mlx = mlx_init()) == NULL)
		die("couldn't start mlx", &e, EXIT_FAILURE);
	if ((e.win = mlx_new_window(e.mlx, WIDTH, HEIGHT, "particle")) == NULL)
		die("Window broke.", &e, EXIT_FAILURE);
	//interop(&e);
	//if ((e.img = mlx_new_image(e.mlx, WIDTH, HEIGHT)) == NULL)
	//	die("Not enough memory", &e, EXIT_FAILURE);
	e.img_affich = mlx_new_image(e.mlx, WIDTH, HEIGHT);
	e.buf_affich = (int *)mlx_get_data_addr(e.img_affich, &e.bpp, &e.szl, &e.end);
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

