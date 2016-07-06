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
	static int	n = -1;
	int			i = -1;

	rotate(e);
	while (++i < NUM_KEYS)
		if (e->keys[i].pressed)
			e->keys[i].f(e->keys[i].keycode, e);
	if (n == 3600 * 60)
		n = -1;
	++n;
	clEnqueueWriteBuffer(e->queue, e->cl_data, CL_TRUE, 0, sizeof(t_data),
		e->data, 0, NULL, NULL);
	clSetKernelArg(e->particle, 1, sizeof(cl_mem), &e->cl_data);
	clSetKernelArg(e->particle, 2, sizeof(int), &n);
	clEnqueueNDRangeKernel(e->queue, e->particle, 2, NULL,
		e->global, e->local, 0, NULL, NULL);
	clFinish(e->queue);
	clEnqueueReadBuffer(e->queue, e->buf, CL_TRUE, 0,
		sizeof(int) * WIDTH * HEIGHT, e->buff, 0, NULL, NULL);
	clFinish(e->queue);
	mlx_put_image_to_window(e->mlx, e->win, e->img, 0, 0);
}
