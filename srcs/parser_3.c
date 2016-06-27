#include "parser.h"

void		tags_attribute_checker(t_tags *tags)
{
	int		i;
	char	**tab;
	char	*str;
	int		boolee;

	i = -1;
	boolee = 0;
	str = get_corresponding_attribute_tag(GRAMMAR_FILE, tags->tag);
	str += ft_strlen(tags->tag) + 2;
	tab = ft_strsplit(str, '|');
	if (tags->attribute)
	{
		while (++i < ft_tablen(tab))
		{
			if (!ft_strcmp(tags->attribute, tab[i]))
			{
				boolee = 1;
				break ;
			}
		}
		ft_little_if_2(boolee, tags);
	}
}

void		fill_content(t_tags *tags)
{
	int	i;

	i = -1;
	if (!ft_strcmp(tags->tag, "position") || !ft_strcmp(tags->tag, "light"))
	{
		if (ft_check_vect(tags->content_text, tags) < 0)
			exit(0);
		tags->content = malloc(sizeof(t_vec3f));
		vec3f_from_text(tags);
	}
	else if (!ft_strcmp(tags->tag, "color"))
	{
		if (ft_check_color(tags->content_text) < 0)
			exit(0);
		tags->content = malloc(sizeof(t_vec3f));
		color_from_text(tags);
	}
	else
		tags->content = NULL;
}

void		check_src_exist(char **src)
{
	if (!src)
	{
		ft_printf("SRC NULL\n");
		exit(0);
	}
}

void		check_needed_valid(char **buff_0, int i)
{
	if (ft_strncmp("needed:", buff_0[0], 6)
		|| ft_strncmp("valid:", buff_0[1], 5))
	{
		ft_printf("invalid integrity file at line %d\n", i);
		exit(0);
	}
}

t_validator	*get_validator(char **buff_2, char **buff_1)
{
	int			a;
	t_validator	*ret;

	ret = ft_memalloc(sizeof(t_validator) * (ft_tablen(buff_1) + 1));
	a = -1;
	while (buff_1[++a])
	{
		ret[a].tag = NULL;
		buff_2 = ft_strsplit(buff_1[a], '*');
		if (buff_2)
		{
			ret[a].tag = ft_strdup(buff_2[0]);
			ret[a].times = (!ft_strcmp("any", buff_2[1]))\
			? -1 : ft_atoi(buff_2[1]);
		}
	}
	ret[a].tag = NULL;
	return (ret);
}
