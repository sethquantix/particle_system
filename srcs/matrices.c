/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrices.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/24 21:53:56 by cchaumar          #+#    #+#             */
/*   Updated: 2016/04/04 13:37:30 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

cl_float3	new_vector(float x, float y, float z)
{
	cl_float3	v;

	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}

static cl_float3	*sf_new_mat(void)
{
	cl_float3	*mat;

	if ((mat = malloc(sizeof(cl_float3) * 3)) == NULL)
		return (NULL);
	mat[0] = new_vector(1, 0, 0);
	mat[1] = new_vector(0, 1, 0);
	mat[2] = new_vector(0, 0, 1);
	return (mat);
}

cl_float3	v_mult_mat(cl_float3 *mat, cl_float3 vect)
{
	cl_float3	res;
	
	res.x = mat[0].x * vect.x + mat[1].x * vect.y + mat[2].x * vect.z;
	res.y = mat[0].y * vect.x + mat[1].y * vect.y + mat[2].y * vect.z;
	res.z = mat[0].z * vect.x + mat[1].z * vect.y + mat[2].z * vect.z;
	return (res);
}

cl_float3	*mat_mult(cl_float3 *mat1, cl_float3 *mat2)
{
	cl_float3	*res;
	int			i;

	res = sf_new_mat();
	i = -1;
	while (++i < 3)
		res[i] = v_mult_mat(mat1, mat2[i]);
	return (res);
}

static void	sf_set_mats(t_env *e, cl_float3 **mats, int axis)
{
	if (axis == 1)
	{
		mats[0][1].y = cos(RAD * e->rot.x);
		mats[0][1].z = sin(RAD * e->rot.x);
		mats[0][2].y = -sin(RAD * e->rot.x);
		mats[0][2].z = cos(RAD * e->rot.x);
	}
	else
	{
		mats[1][0].x = cos(RAD * e->rot.y);
		mats[1][0].z = -sin(RAD * e->rot.y);
		mats[1][2].x = sin(RAD * e->rot.y);
		mats[1][2].z = cos(RAD * e->rot.y);
	}
}

cl_float3	*calc_mat(t_env *e, int axis)
{
	static cl_float3	**mats = NULL;

	if (mats == NULL)
	{
		mats = malloc(sizeof(cl_float3 *) * 2);
		mats[0] = sf_new_mat();
		mats[1] = sf_new_mat();
	}
	sf_set_mats(e, mats, axis);
	return (mats[axis]);
}
