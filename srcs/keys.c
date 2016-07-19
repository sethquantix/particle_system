/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/30 12:51:59 by cchaumar          #+#    #+#             */
/*   Updated: 2016/06/30 12:57:11 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "particle.h"

t_key	new_key(int keycode, void (*f)(int, void *))
{
	t_key	key;

	key.keycode = keycode;
	key.f = f;
	key.pressed = 0;
	return (key);
}
