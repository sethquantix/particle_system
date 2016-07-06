/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/28 15:47:59 by cchaumar          #+#    #+#             */
/*   Updated: 2016/06/30 14:15:57 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "particle.h"

cl_float2	float2(float x, float y)
{
	cl_float2	v;

	v.x = x;
	v.y = y;
	return (v);
}

cl_float4	float4(float x, float y, float z, float w)
{
	cl_float4	v;

	v.x = x;
	v.y = y;
	v.z = z;
	v.w = w;
	return (v);
}

cl_int2		int2(int x, int y)
{
	cl_int2		v;

	v.x = x;
	v.y = y;
	return (v);
}
