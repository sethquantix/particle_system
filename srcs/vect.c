/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 17:04:40 by cchaumar          #+#    #+#             */
/*   Updated: 2016/06/28 16:01:45 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "particle.h"

cl_float3	cross(cl_float3 u, cl_float3 v)
{
	cl_float3			w;

	w.x = u.y * v.z - u.z * v.y;
	w.y = u.z * v.x - u.x * v.z;
	w.z = u.x * v.y - u.y * v.x;
	return (w);
}

float		dot(cl_float3 v1, cl_float3 v2)
{
	return (v1.x * v2.x + v1.y *	 v2.y + v1.z * v2.z);
}

float		length(cl_float3 v)
{
	return (sqrt(dot(v, v)));
}

cl_float3	normalize(cl_float3 v)
{
	float	l;

	l = length(v);
	v.x /= l;
	v.y /= l;
	v.z /= l;
	return (v);
}

cl_float3	float3(float x, float y, float z)
{
	cl_float3	v;

	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}
