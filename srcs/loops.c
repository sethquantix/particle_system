/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loops.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/23 09:07:44 by cchaumar          #+#    #+#             */
/*   Updated: 2016/04/27 07:36:55 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		key_hook(int keycode, t_env *e)
{
	e->move = 1;
	if (keycode == 53)
		die("Until next time !\n", e, EXIT_SUCCESS);
	ft_printf("did you do something ? Felt like you did something. Okay, nevermind. I'll keep doing nothing interesting. By the way, I solved quantum loops to thirteen degree recursion last week. Like you would care.\n");
	return (0);
}

int		loop(t_env *e)
{
	if (e->move)
	{
		e->move = 0;
	}
	return (0);
}
