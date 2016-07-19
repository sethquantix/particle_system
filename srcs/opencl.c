/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/04 14:23:08 by cchaumar          #+#    #+#             */
/*   Updated: 2016/06/30 14:04:41 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "particle.h"

static void			get_source(t_env *e)
{
	char	*source;
	char	*buff;
	int		fd;

	source = NULL;
	buff = NULL;
	if ((fd = open(KERNEL, O_RDONLY)) == -1)
		die("couldnt load kernel.", e, EXIT_FAILURE);
	while (get_next_line(fd, &buff) != 0)
		source = ft_strjoinfree(source,
				ft_strjoinfree(buff, "\n", 1), 2 + (source ? 1 : 0));
	e->source = source;
	close(fd);
}

t_callback		notify(const char *errinfo, const void *private_info,
		size_t cb, void *user_data)
{
	static int		errcount = 0;

	(void)private_info;
	(void)cb;
	(void)user_data;
	ft_printf("Error %d (notified) : %s\n", ++errcount, errinfo);
	//die("Opencl kernel error, exiting.",  NULL, EXIT_FAILURE);
}

static void			init_kernels(t_env *e)
{
	cl_kernel	init;
	int			err;
	cl_uint2	seed;

	seed.x = 74345676;
	seed.y = 1354343545;
	init = clCreateKernel(e->program, "init", &err);
	e->particle = clCreateKernel(e->program, "particle", &err);

	clSetKernelArg(e->particle, 0, sizeof(cl_mem), &e->pos_par);
	clSetKernelArg(e->particle, 1, sizeof(cl_mem), &e->acc_par);
	clSetKernelArg(e->particle, 2, sizeof(cl_mem), &e->spd_par);
	clSetKernelArg(e->particle, 3, sizeof(cl_mem), &e->lc_par);
	clSetKernelArg(e->particle, 4, sizeof(cl_mem), &e->m_par);
	clSetKernelArg(e->particle, 5, sizeof(cl_mem), &e->mass);

	clSetKernelArg(init, 1, sizeof(cl_mem), &e->pos_par);
	clSetKernelArg(init, 2, sizeof(cl_mem), &e->acc_par);
	clSetKernelArg(init, 3, sizeof(cl_mem), &e->spd_par);
	clSetKernelArg(init, 4, sizeof(cl_mem), &e->lc_par);
	clSetKernelArg(init, 5, sizeof(cl_mem), &e->m_par);
	clSetKernelArg(init, 6, sizeof(cl_mem), &e->mass);

	clSetKernelArg(init, 0, sizeof(cl_uint2), &seed);
	clSetKernelArg(e->particle, 7, sizeof(cl_image), &e->img);
	clSetKernelArg(e->particle, 8, sizeof(cl_mem), &e->timer);
	if (err)
		die("Couldnt create buffer on VRAM. Cause reasons.", e, EXIT_FAILURE);
	ft_printf("{CGRNVRAM buffer allocated.\n}");
	clEnqueueNDRangeKernel(e->queue, init, 2, NULL, e->global, e->local, 0,
		NULL, NULL);
	clFinish(e->queue);
	clReleaseKernel(init);
}

static void			opencl_memory(t_env *e)
{
	int		err;
	int		n;

	n = 0;
	if ((e->queue = clCreateCommandQueue(e->context, e->device,
		CL_QUEUE_PROFILING_ENABLE, NULL)) == NULL)
		die("Couldnt create command queue.", e, EXIT_FAILURE);
	ft_printf("{CGRNCommand queue created.\n}");
	if ((e->program = clCreateProgramWithSource(e->context, 1,
		(const char **)(&e->source), NULL, NULL)) == NULL)
		die("couldnt create program.", e, EXIT_FAILURE);
	free(e->source);
	ft_printf("{CGRNPRogram created from sources.\n}");
	if (clBuildProgram(e->program, 0, NULL, "-I kernels -cl-fast-relaxed-math", NULL, NULL))
		die("Couldnt build program.", e, EXIT_FAILURE);
	ft_printf("{CGRNProgram successfully built\n}");

	e->pos_par = clCreateBuffer(e->context, CL_MEM_READ_WRITE,
			sizeof(cl_float3) * NUM_P, NULL, &err);
	e->acc_par = clCreateBuffer(e->context, CL_MEM_READ_WRITE,
			sizeof(cl_float3) * NUM_P, NULL, &err);
	e->spd_par = clCreateBuffer(e->context, CL_MEM_READ_WRITE,
			sizeof(cl_float3) * NUM_P, NULL, &err);
	e->lc_par = clCreateBuffer(e->context, CL_MEM_READ_WRITE,
			sizeof(cl_float) * NUM_P, NULL, &err);
	e->m_par = clCreateBuffer(e->context, CL_MEM_READ_WRITE,
			sizeof(cl_uint2) * NUM_P, NULL, &err);
	e->mass = clCreateBuffer(e->context, CL_MEM_READ_WRITE,
			sizeof(cl_float3) * NUM_P, NULL, &err);

	e->cl_data = clCreateBuffer(e->context, CL_MEM_READ_WRITE,
			sizeof(t_data), NULL, &err);
	e->timer = clCreateBuffer(e->context, CL_MEM_READ_WRITE,
			sizeof(cl_long), NULL, &err);
	init_kernels(e);

}

void				opencl_init(t_env *e)
{
	cl_platform_id	platform;
	int				err;

	ft_printf("initializing opencl...\n");
	ft_printf("initializing gpu kernels...\n");
	get_source(e);
	clGetPlatformIDs(1, &platform, NULL);
	ft_printf("{CGRNPlatform detected\n}");
	if (clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &e->device, NULL))
		die("GPU not detected by opencl. Now that's new", e, EXIT_FAILURE);
	ft_printf("{CGRNGPU detected.\n");

	info_device(e->device);
	e->global[0] = GLOBAL;
	e->global[1] = GLOBAL;
	e->local[0] = 16;
	e->local[1] = 16;
	ft_printf("{CGRNKernel sizes set\n}");
	opencl_memory(e);
	ft_printf("{CGRNKernels set\n}");
}
