#include "parser.h"

void	do_next(t_tags **tags, int *i, int len_0)
{
	(*tags)->next = tag_next_init(*tags);
	*tags = (*tags)->next;
	*i += len_0;
}

void	tags_rewind(t_tags **tags)
{
	while ((*tags)->prev)
		*tags = (*tags)->prev;
}

t_tags	*list_tags(char *src)
{
	int					i;
	int					len[2];
	t_tags				*tags;

	tags = tag_init();
	i = -1;
	while ((size_t)++i < ft_strlen(src))
		if (src[i] == '<' && src[i + 1] && src[i + 1] != '/' && (i += 1))
		{
			len[0] = find_next_symbol(src, i, '>') - 1;
			if (len[0] < 0)
				die_nofree("syntax error, tag not closed");
			len[1] = find_attributes(src, i, len[0], tags);
			tags->tag = ft_strsub(src, i, len[1] + 1);
			if (find_closing_tag(src + i + len[0], tags->tag) < 0)
				die_nofree(ft_strjoin("no closing tag for tag : ", tags->tag));
			type_giver_checker(tags);
			tags->content_text = ft_strsub(src, i + len[0] + 2,
			find_closing_tag(src + i + len[0], tags->tag) - 2);
			content_filler(&tags);
			little_if(&tags, &i);
			do_next(&tags, &i, len[0]);
		}
	tags_rewind(&tags);
	return (tags);
}

int		find_tag(char *src, char *tag)
{
	size_t	i;
	char	*buff;

	buff = NULL;
	i = -1;
	while (++i < ft_strlen(src) - (ft_strlen(tag) + 2))
	{
		buff = ft_strjoin(ft_strjoin("<", tag), ">");
		if (!ft_strcmp(ft_strsub(src, i, 2 + ft_strlen(tag)),
			buff))
		{
			return (i);
		}
	}
	return (-1);
}

int		find_closing_tag(char *src, char *tag)
{
	size_t	i;
	char	*buff;

	buff = NULL;
	i = -1;
	while (++i < ft_strlen(src) - (ft_strlen(tag) + 2))
	{
		buff = ft_strjoin(ft_strjoin("</", tag), ">");
		if (!ft_strcmp(ft_strsub(src, i, 3 + ft_strlen(tag)),
			buff))
		{
			free(buff);
			return (i);
		}
		free(buff);
	}
	return (-1);
}
