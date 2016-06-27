#include "parser.h"

void	validity_creator_need(int i, int a, t_integrity *i_m, t_tags *tags)
{
	while (i_m[++i].original != NULL)
	{
		while (tags->tags && tags->tags->prev)
			tags->tags = tags->tags->prev;
		if (!ft_strcmp(tags->tag, i_m[i].original))
		{
			while (tags->tags->next)
			{
				a = -1;
				while (i_m[i].needed[++a].tag)
				{
					if (!ft_strcmp(tags->tags->tag, i_m[i].needed[a].tag))
					{
						i_m[i].needed[a].saved += 1;
						i_m[i].needed[a].present = 1;
					}
				}
				tags->tags = tags->tags->next;
			}
		}
	}
}

int		integrity_check(t_tags *tags, t_integrity *i_m)
{
	if (!contain_valid(tags, i_m) || !contain_min(tags, i_m))
		return (0);
	return (1);
}

int		reality_check(char *tag)
{
	int					i;
	int					size_tab;
	int					control;
	static const char	*tab_tags[] = AVAILABLE_TAGS;

	i = -1;
	size_tab = ft_tablen((char **)tab_tags);
	control = 0;
	while (++i < size_tab)
		if (!ft_strcmp(tag, tab_tags[i]))
		{
			control++;
			break ;
		}
	if (!control)
		return (0);
	else
		return (1);
}

void	check_validity(t_tags *tags, t_integrity *integrity_matrix)
{
	if (!tags || !tags->tag)
	{
		ft_printf("something is wrong\n");
		exit(0);
	}
	ft_printf("I am here\n");
	if (!reality_check(tags->tag))
	{
		ft_printf("inexistant tag : %s\n", tags->tag);
		exit(0);
	}
	if (!integrity_check(tags, integrity_matrix))
	{
		ft_printf("tag missing informations or have invalid tags : %s\n",\
		tags->tag);
		exit(0);
	}
}

int		contain_min(t_tags *tags, t_integrity *i_m)
{
	int	i;
	int	a;

	i = -1;
	a = -1;
	validity_creator_need(i, a, i_m, tags);
	i = -1;
	while (i_m[++i].original != NULL)
	{
		a = -1;
		while (i_m[i].needed[++a].tag)
		{
			if (i_m[i].needed[a].present && i_m[i].needed[a].times > 0
				&& i_m[i].needed[a].times != i_m[i].needed[a].saved)
			{
				ft_printf("tag contain bad number of needed args : %s, %s ==\
					%d\n", i_m[i].original, i_m[i].\
					needed[a].tag, i_m[i].needed[a].saved);
				return (0);
			}
		}
	}
	while (tags->tags && tags->tags->prev)
		tags->tags = tags->tags->prev;
	return (1);
}
