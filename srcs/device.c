/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   device.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/27 07:12:19 by cchaumar          #+#    #+#             */
/*   Updated: 2016/04/27 07:48:34 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	info_device(cl_device_id device)
{
	size_t		size;
	char		*data;

	clGetDeviceInfo(device, CL_DEVICE_NAME, 0, NULL, &size);
	data = ft_memalloc(size);
	clGetDeviceInfo(device, CL_DEVICE_NAME, size, data, NULL);
	ft_printf("{CGRN1 : opencl / opengl interoperability device : }");
	ft_printf("\t{CRED%s\n}", data);
	free(data);
	clGetDeviceInfo(device, CL_DEVICE_OPENCL_C_VERSION, 0, NULL, &size);
	data = ft_memalloc(size);
	clGetDeviceInfo(device, CL_DEVICE_OPENCL_C_VERSION, size, data, NULL);
	ft_printf("{CGRN2 : opencl device version : }\t\t\t{CMAG%s\n}", data);
	clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(size_t), &size,
		NULL);
	free(data);
	ft_printf("{CGRN3 : compute units available : }\t\t\t{CMAG%d\n}", size);
	clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, 0, NULL, &size);
	data = ft_memalloc(size);
	clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, size, data, NULL);
	ft_printf("{CGRN4 : available extensions : }\t\t\t{CMAG%s\n}", data);
}
