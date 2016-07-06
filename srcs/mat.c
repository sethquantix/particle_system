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

void	rot_mat(cl_float3 *m, int axis, float deg)
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
}

void	print_mat(cl_float3 *m)
{
	printf("%f %f %f\n%f %f %f\n%f %f %f\n\n", m[0].x, m[0].y, m[0].z,
		m[1].x, m[1].y, m[1].z, m[2].x, m[2].y, m[2].z);
}
