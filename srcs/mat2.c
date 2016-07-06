/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/29 13:40:18 by cchaumar          #+#    #+#             */
/*   Updated: 2016/06/29 14:59:39 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "particle.h"

void	transpose(cl_float3 *m)
{
	cl_float3	t[3];

	mat_cpy(t, m);
	m[0] = float3(t[0].x, t[1].x, t[2].x);
	m[1] = float3(t[0].y, t[1].y, t[2].y);
	m[2] = float3(t[0].z, t[1].z, t[2].z);
}
