/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 17:04:40 by cchaumar          #+#    #+#             */
/*   Updated: 2016/04/27 06:48:29 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

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

cl_float3	normalize(cl_float3 v)
{
	float	length;

	length = sqrt(dot(v, v));
	v.x /= length;
	v.y /= length;
	v.z /= length;
	return (v);
}

cl_float3	v_add(cl_float3 v1, cl_float3 v2)
{
	cl_float3	v;

	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	v.z = v1.z + v2.z;
	return (v);
}

cl_float3	v_mult(cl_float3 v, float k)
{
	v.x *= k;
	v.y *= k;
	v.z *= k;
	return (v);
}
