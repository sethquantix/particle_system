/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/27 08:45:35 by cchaumar          #+#    #+#             */
/*   Updated: 2016/04/28 22:04:42 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#include <stdio.h>

void	parse(char *filename, t_env *e)
{
	int					fd;
	char				*file;
	char				*buff;
	t_tags				*tags;
	static t_integrity	*integrity_matrix = NULL;

	if (!integrity_matrix)
		create_integrity_matrix(&integrity_matrix);
	file = NULL;
	buff = NULL;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return ;
	else
	{
		file = ft_strdup("");
		while (get_next_line(fd, &buff))
			file = ft_strjoinfree(file, buff, (file != NULL) + 2);
	}
	file = ft_exterminate(file);
	if (find_tag(file, "scene") < 0)
		die("Invalid scene, no scene tag !\n", NULL, EXIT_FAILURE);
	if (find_closing_tag(file, "scene") < 0)
		die("Invalid scene, no closing scene tag !\n", NULL, EXIT_FAILURE);
	tags = list_tags(file);
	check_validity(tags, integrity_matrix);
	if (!tags->tags)
		ft_printf("that's fucked up\n");
	while (tags->tags->next)
	{
		check_validity(tags->tags, integrity_matrix);
		if (tags->tags->next)
			tags->tags = tags->tags->next;
	}
	while (tags->tags->prev)
		tags->tags = tags->tags->prev;
	while (tags->next)
	{
		ft_printf("tag == %s, attribute == %s\n", tags->tag, (tags->attribute)
			? (tags->attribute) : ("nill"));
		ft_printf("tag_content == %s\n", tags->content_text);
		if (tags->content_text)
		{
			while (tags->tags->next)
			{
				ft_printf("taggggg == %s\n", tags->tags->tag);
				tags->tags = tags->tags->next;
			}
		}
		tags = tags->next;
	}
	(void)e;
}

int		find_next_symbol(char *src, int offset, char symbol)
{
	int	i;

	i = 0;
	while (offset + i < (int)ft_strlen(src) && src && src[offset + i]
			&& src[offset + i] != symbol)
		i++;
	if (src[offset + i] && src[offset + i] != symbol)
		return (-1);
	else
		return (i);
}

int		find_attributes(char *src, int offset, int len_tagat, t_tags *tag)
{
	int	i;

	i = -1;
	if ((i = find_next_symbol(src, offset, ':')) > 0 && i < len_tagat)
	{
		offset += i;
		tag->attribute = ft_strsub(src, offset + 1, len_tagat - (i + 0));
		return (i - 1);
	}
	else
	{
		tag->attribute = NULL;
		return (len_tagat);
	}
}

int		ft_tablen(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	type_giver_checker(t_tags *tags)
{
	int					i;
	int					size_tab;
	int					control;
	static const char	*tab_tags[] = AVAILABLE_TAGS;

	i = -1;
	size_tab = ft_tablen((char **)tab_tags);
	control = 0;
	while (++i < size_tab)
		if (!ft_strcmp(tags->tag, tab_tags[i]))
		{
			control++;
			if (!ft_strcmp(tags->tag, "color")
				|| !ft_strcmp(tags->tag, "position")
				|| !ft_strcmp(tags->tag, "light"))
				tags->type = CONTAINER;
			else
				tags->type = EMBEDDER;
			break ;
		}
	if (!control)
	{
		ft_printf("invalid tag, %s\n", tags->tag);
		exit(0);
	}
}
