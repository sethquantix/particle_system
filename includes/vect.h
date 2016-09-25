/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/28 14:56:25 by cchaumar          #+#    #+#             */
/*   Updated: 2016/06/30 14:15:24 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECT_H
#define VECT_H
#include <OpenCL/opencl.h>

cl_float3	cross(cl_float3 u, cl_float3 v);
float		dot(cl_float3 u, cl_float3 v);
float		length(cl_float3 v);
cl_float3	normalize(cl_float3 v);
cl_float3	float3(float x, float y, float z);
void		ident(cl_float3 *mat);
cl_float3	v_mult_mat(cl_float3 *mat, cl_float3 v);
void		mat_mult(cl_float3 *m1, cl_float3 *m2);
// void		rot_mat(cl_float3 *m, int axis, float deg);
void	rot_mat(float deg, cl_float3 m[3], int r);
void		mat_cpy(cl_float3 *m1, cl_float3 *m2);
void		transpose(cl_float3 *m);
cl_float2	float2(float x, float y);
cl_float4	float4(float x, float y, float z, float w);
void		print_mat(cl_float3 *m);
cl_int2		int2(int x, int y);

#endif

