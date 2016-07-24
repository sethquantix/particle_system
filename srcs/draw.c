/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/27 11:11:22 by cchaumar          #+#    #+#             */
/*   Updated: 2016/06/30 19:32:16 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "particle.h"

void	translate(t_env *e, int axis, int dir)
{
	cl_float3	v;

	v = v_mult_mat(e->data->mat, float3(0.1 * (axis == 0) * dir,
		0.1 * (axis == 1) * dir, 0.1 * (axis == 2) * dir));
	e->data->cam = float3(e->data->cam.x + v.x, e->data->cam.y + v.y,
		e->data->cam.z + v.z);
}

void	rotate(t_env *e)
{
	cl_float3			mat[3];

	ident(e->data->mat);
	rot_mat(mat, 0, e->rot.x);
	mat_mult(e->data->mat, mat);
	rot_mat(mat, 1, e->rot.y);
	mat_mult(e->data->mat, mat);
	rot_mat(mat, 2, e->rot.z);
	mat_mult(e->data->mat, mat);
	e->data->dir = v_mult_mat(e->data->mat, float3(0, 0, 1));
	e->data->corners[0] = v_mult_mat(e->data->mat, e->corners[0]);
	e->data->corners[1] = v_mult_mat(e->data->mat, e->corners[1]);
	e->data->corners[2] = v_mult_mat(e->data->mat, e->corners[2]);
	transpose(e->data->mat);
}

void	draw(t_env *e)
{
	int				i;
	struct timeval	time_now;

	i = -1;
	static long ouch;
	ouch++;
	gettimeofday(&time_now, NULL);
	rotate(e);
	while (++i < NUM_KEYS)
		if (e->keys[i].pressed)
			e->keys[i].f(e->keys[i].keycode, e);
//	glBindFramebuffer(GL_FRAMEBUFFER, e->framebuffer);
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, e->texture[0]);
//	glClear(GL_COLOR_BUFFER_BIT);
//	glFinish();
	clEnqueueWriteBuffer(e->queue, e->cl_data, CL_TRUE, 0, sizeof(t_data),
		e->data, 0, NULL, NULL);
	clSetKernelArg(e->particle, 6, sizeof(cl_mem), &e->cl_data);
	clEnqueueWriteBuffer(e->queue, e->timer, CL_TRUE, 0, sizeof(cl_mem),
		&(ouch), 0, NULL, NULL);
	clSetKernelArg(e->particle, 8, sizeof(cl_mem), &e->timer);
//	clEnqueueAcquireGLObjects(e->queue, 1, &e->img, 0, 0, 0);
	//printf("hellooooooo\n");
//	clSetKernelArg(e->particle, 7, sizeof(cl_image), &e->img);
	clSetKernelArg(e->particle, 7, sizeof(cl_image), &e->buf);
	clEnqueueNDRangeKernel(e->queue, e->particle, 2, NULL,
		e->global, e->local, 0, NULL, NULL);
	clFinish(e->queue);
	//printf("axaxaxaxaxa\n");
//	clEnqueueReleaseGLObjects(e->queue, 1, &e->img, 0, 0, 0);
	//printf("worddddddd\n");
//	clEnqueueWriteBuffer(e->queue, e->img, CL_TRUE, 0, sizeof(cl_image), &e->img, 0, 0, 0);	
	clEnqueueReadBuffer(e->queue, e->buf, CL_TRUE, 0,
		sizeof(int) * WIDTH * HEIGHT, e->buf_affich, 0, NULL, NULL);
	
	// clEnqueueReadBuffer(e->queue, &e->img_affich, CL_TRUE, 0, sizeof(cl_mem), &e->img, 0, 0, 0);
	//printf("testererere\n");
	clFinish(e->queue);
	mlx_put_image_to_window(e->mlx, e->win, e->img_affich, 0, 0);

	//glEnable(GL_TEXTURE_2D);
	//glActiveTexture(GL_TEXTURE0);
	// glFinish();
	// glDrawElements(GL_TEXTURE_2D, 1, GL_UNSIGNED_INT, 0);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, e->texture[0]);
//	glDrawBuffer(GL_FRONT_AND_BACK);
//	mlx_opengl_swap_buffers(e->win);
}
