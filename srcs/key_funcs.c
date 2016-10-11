/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_funcs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/30 18:58:39 by cchaumar          #+#    #+#             */
/*   Updated: 2016/06/30 19:42:47 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "particle.h"

void	move_cam(int keycode, void *env)
{
	cl_float3	v;
	cl_float3	c;
	t_env		*e;

	e = (t_env *)env;
	c = e->data->cam;
	v = float3(((keycode == 0x2) - (keycode == 0x0)) * SPEED,
				((keycode == 0xD) - (keycode == 0x1)) * SPEED,
				((keycode == 0xE) - (keycode == 0xC)) * SPEED);
	v = v_mult_mat(e->data->mat, v);
	e->data->cam = float4(v.x + c.x, v.y + c.y, v.z + c.z, c.w);
}

void	rotate_cam(int keycode, void *env)
{
	cl_float3	v;
	cl_float3	r;
	t_env		*e;

	e = (t_env *)env;
	r = e->rot;
	v = float3(((keycode == 0x7E) - (keycode == 0x7D)) * RAD_SPEED,
				((keycode == 0x7C) - (keycode == 0x7B)) * RAD_SPEED, 0.0f);
	e->rot = float3(v.x + r.x, v.y + r.y, v.z + r.z);
}

void	toggle(int keycode, void *env)
{
	t_env		*e;	
	int			i;

	e = (t_env *)env;
	i = -1;
	while (++i < NUM_KEYS)
		if (e->keys[i].keycode == keycode)
			e->keys[i].pressed = 0;
	e->t ^= 1;
}