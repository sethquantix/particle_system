/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loops.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/23 09:07:44 by cchaumar          #+#    #+#             */
/*   Updated: 2016/06/30 22:08:27 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "particle.h"

int		key_press(int keycode, t_env *e)
{
	int		i;

	i = -1;
	if (keycode == 53)
		die("Until next time !\n", e, EXIT_SUCCESS);
	while (++i < NUM_KEYS)
		if (keycode == e->keys[i].keycode)
			e->keys[i].pressed = 1;
	return (0);
}

int		key_release(int keycode, t_env *e)
{
	int		i;

	i = -1;
	if (keycode == 53)
		die("Until next time !\n", e, EXIT_SUCCESS);
	while (++i < NUM_KEYS)
		if (keycode == e->keys[i].keycode)
			e->keys[i].pressed = 0;
	return (0);
}

int		mouse_free_hook(int x, int y, t_env *e)
{
	cl_float2			k;

	k = float2((float)x / (float)WIDTH, (float)y / (float)HEIGHT);
	e->data->g = float3(k.x, 1.0f - k.y, 0.0f);
	return (0);
}

int		loop(t_env *e)
{
	static int		fcount = 0;
	static struct timeval	start;
	struct timeval			end;
	struct timeval			delta;

	if (fcount == 0)
		gettimeofday(&start, NULL);
	draw(e);
	++fcount;
	gettimeofday(&end, NULL);
	timersub(&end, &start, &delta);
	if (delta.tv_sec >= 1)
	{
		ft_printf("FPS : %d\n", fcount);
		fcount = 0;
	}
	return (0);
}
