/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrices.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/24 21:53:56 by cchaumar          #+#    #+#             */
/*   Updated: 2016/06/29 15:02:09 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "particle.h"

void		ident(cl_float3 *mat)
{
	int		i;

	i = -1;
	while (++i < 3)
		mat[i] = float3(i == 0, i == 1, i == 2);
}

cl_float3	v_mult_mat(cl_float3 *mat, cl_float3 v)
{
	return (float3(dot(mat[0], v), dot(mat[1], v), dot(mat[2], v)));
}

void		mat_cpy(cl_float3 *m1, cl_float3 * m2)
{
	int		i = -1;

	while (++i < 3)
		m1[i] = m2[i];
}

void		mat_mult(cl_float3 *mat1, cl_float3 *mat2)
{
	cl_float3	res[3];
	cl_float3	inv[3];
	int			i;

	inv[0] = float3(mat2[0].x, mat2[1].x, mat2[2].x);
	inv[1] = float3(mat2[0].y, mat2[1].y, mat2[2].y);
	inv[2] = float3(mat2[0].z, mat2[1].z, mat2[2].z);
	mat_cpy(res, mat1);
	i = -1;
	while (++i < 3)
		mat1[i] = v_mult_mat(inv, res[i]);
}

/*void	rot_mat(cl_float3 *m, int axis, float deg)
{
	float	rad;

	rad = deg * M_PI / 180.0;
	ident(m);
	if (axis == 0)
	{
		m[1].y = cos(rad);
		m[1].z = -sin(rad);
		m[2].y = sin(rad);
		m[2].z = cos(rad);
	}
	if (axis == 1)
	{
		m[0].x = cos(rad);
		m[0].z = sin(rad);
		m[2].x = -sin(rad);
		m[2].z = cos(rad);
	}
	if (axis == 2)
	{
		m[0].x = cos(rad);
		m[0].y = -sin(rad);
		m[1].x = sin(rad);
		m[1].y = cos(rad);
	}
}*/

cl_float3	vect3(float x, float y, float z)
{
	cl_float3	v;

	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}

cl_float3	vect_mult_k(cl_float3 v, float k)
{
	v.x *= k;
	v.y *= k;
	v.z *= k;
	return (v);
}

float		dot3(cl_float3 u, cl_float3 v)
{
	return (u.x * v.x + u.y * v.y + u.z * v.z);
}

void	mat_ident3(cl_float3 mat[3])
{
	int		i;

	i = -1;
	while (++i < 3)
		mat[i] = vect3(i == 0, i == 1, i == 2);
}

void	mat_mult_mat3(cl_float3 m1[3], cl_float3 m2[3])
{
	cl_float3	v;
	int			i;

	i = -1;
	while (++i < 3)
	{
		v.x = dot3(m1[i], vect3(m2[0].x, m2[1].x, m2[2].x));
		v.y = dot3(m1[i], vect3(m2[0].y, m2[1].y, m2[2].y));
		v.z = dot3(m1[i], vect3(m2[0].z, m2[1].z, m2[2].z));
		m1[i] = v;
	}
}

void	rot_mat(float deg, cl_float3 m[3], int r)
{
	float	rad;

	rad = (deg / 180.0) * M_PI;
	m[0] = vect3(r == 0 ? 1 : cos(rad), r == 2 ? -sin(rad) : 0,
		r == 1 ? sin(rad) : 0);
	m[1] = vect3(r == 2 ? sin(rad) : 0, r == 1 ? 1 : cos(rad),
		r == 0 ? -sin(rad) : 0);
	m[2] = vect3(r == 1 ? -sin(rad) : 0, r == 0 ? sin(rad) : 0,
		r == 2 ? 1 : cos(rad));
}

void		rot_mats3(cl_float3 res[3], cl_float3 rot)
{
	static cl_float3	rots[3][3];
	static int			start = 0;

	if (!start)
	{
		start = 1;
		mat_ident3(rots[0]);
		mat_ident3(rots[1]);
		mat_ident3(rots[2]);
	}
	rot_mat(rot.x, rots[0], 0);
	rot_mat(rot.y, rots[1], 1);
	rot_mat(rot.z, rots[2], 2);
	res[0] = rots[0][0];
	res[1] = rots[0][1];
	res[2] = rots[0][2];
	mat_mult_mat3(res, rots[1]);
}

void	print_mat(cl_float3 *m)
{
	printf("%f %f %f\n%f %f %f\n%f %f %f\n\n", m[0].x, m[0].y, m[0].z,
		m[1].x, m[1].y, m[1].z, m[2].x, m[2].y, m[2].z);
}
