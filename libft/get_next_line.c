/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <cchaumar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/04 08:24:18 by cchaumar          #+#    #+#             */
/*   Updated: 2016/02/23 05:54:29 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		ft_last(t_file *file, char **line)
{
	if (file->buff && *(file->buff))
	{
		*line = ft_strdup(file->buff);
		file->buff = NULL;
		return (1);
	}
	return (0);
}

int		ft_getline(t_file *file, char **line)
{
	char	*end;
	char	*temp;
	char	tmp[BUFF_SIZE + 1];
	int		in;

	if ((in = read(file->fd, tmp, BUFF_SIZE)) == -1)
		return (-1);
	tmp[in] = '\0';
	if (file->buff)
	{
		file->buff = ft_strjoinfree(file->buff, tmp, 1);
		end = ft_strchr(file->buff, '\n');
	}
	else
		return (ft_last(file, line));
	if (end == NULL)
		return ((in == 0) ? ft_last(file, line) : ft_getline(file, line));
	*end = '\0';
	*line = ft_strdup(file->buff);
	*end = '\n';
	temp = file->buff;
	file->buff = ft_strdup(end + 1);
	ft_memdel((void **)&temp);
	return (1);
}

t_file	*ft_new_file(int fd)
{
	t_file	*file;

	if ((file = malloc(sizeof(t_file))) == NULL)
		return (NULL);
	file->fd = fd;
	file->buff = ft_memalloc(BUFF_SIZE + 1);
	ft_bzero(file->buff, BUFF_SIZE + 1);
	return (file);
}

t_list	*ft_create_file(t_list **files, int fd)
{
	t_list	*node;
	t_file	*file;

	file = ft_new_file(fd);
	if (*files == NULL)
	{
		*files = ft_lstnew(file, sizeof(t_file));
		node = *files;
	}
	else
	{
		node = *files;
		while (node->next)
			node = node->next;
		node->next = ft_lstnew(file, sizeof(t_file));
		node = node->next;
	}
	free(file);
	return (node);
}

int		get_next_line(int const fd, char **line)
{
	static t_list	*files;
	t_list			*node;
	t_file			*file;
	int				ret;

	file = NULL;
	node = files;
	while (node)
		if (((t_file *)(node->content))->fd == fd)
			break ;
		else
			node = node->next;
	if (!node)
		if ((node = ft_create_file(&files, fd)) == NULL)
			return (-1);
	file = (t_file *)(node->content);
	if ((ret = ft_getline(file, line)) <= 0)
	{
		if (file->buff)
			ft_memdel((void **)&(file->buff));
		ft_lst_delif(&files, node);
		return (ret);
	}
	return (1);
}
