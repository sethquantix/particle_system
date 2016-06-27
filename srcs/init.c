/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/27 06:49:38 by cchaumar          #+#    #+#             */
/*   Updated: 2016/04/27 08:45:30 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

char				*source(char *filename)
{
	char	*source;
	char	*buff;
	int		fd;

	source = NULL;
	fd = open(filename, O_RDONLY);
	while (get_next_line(fd, &buff) > 0)
		source = ft_strjoinfree(source, buff, (source != NULL) + 2);
	close(fd);
	return (source);
}

void CL_CALLBACK	notify(const char *err, const void *e_private, size_t cb,
		void *data)
{
	static int	n = 0;

	(void)e_private;
	(void)cb;
	(void)data;
	ft_printf("Notify : Errnum %d | %s\n", ++n, err);
}

void				init(t_env *e)
{
	(void)e;
}
