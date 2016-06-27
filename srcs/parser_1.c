#include "parser.h"

t_integrity	*parse_integrity(char **src, int i)
{
	t_integrity	*integrity_matrix;
	char		**buff[3];

	check_src_exist(src);
	integrity_matrix = ft_memalloc(sizeof(t_integrity) * (ft_tablen(src) + 1));
	while (++i < (ft_tablen(src) - 1) && src[i])
	{
		integrity_matrix[i].original = ft_strsub(src[i], 0,
			find_next_symbol(src[i], 0, ' '));
		buff[0] = ft_strsplit(src[i] + ((find_next_symbol(src[i], 0, '{') > 0) ?
			(find_next_symbol(src[i], 0, '{') + 1) : 0), ' ');
		if (ft_strlen(buff[0][1]))
			buff[0][1] = ft_strndup(buff[0][1], ft_strlen(buff[0][1]) - 1);
		ft_printf("%s\n%s\n", buff[0][0], buff[0][1]);
		check_needed_valid(buff[0], i);
		buff[0][0] = ft_strsub(buff[0][0], 8, ft_strlen(buff[0][0]) - 9);
		buff[0][1] = ft_strsub(buff[0][1], 7, ft_strlen(buff[0][1]) - 8);
		buff[1] = ft_strsplit(buff[0][0], ',');
		integrity_matrix[i].needed = get_validator(buff[2], buff[1]);
		free(buff[1]);
		buff[1] = ft_strsplit(buff[0][1], ',');
		integrity_matrix[i].optional = get_validator(buff[2], buff[1]);
	}
	integrity_matrix[i].original = NULL;
	return (integrity_matrix);
}

int			retard_way_of_getting_number_of_lines(int i, int fd, char *buff)
{
	fd = open(INTEGRITY_FILE, O_RDONLY);
	if (fd < 0)
	{
		ft_printf("could not open INTEGRITY_FILE\n");
		exit(0);
	}
	i = -1;
	while (get_next_line(fd, &buff) > 0)
	{
		++i;
		free(buff);
		buff = NULL;
	}
	close(fd);
	return (i);
}

void		create_integrity_matrix(t_integrity **integrity)
{
	int		fd;
	int		i;
	char	**src;
	char	*buff;

	src = NULL;
	buff = NULL;
	i = 0;
	fd = 0;
	i = retard_way_of_getting_number_of_lines(i, fd, buff);
	src = (char **)malloc(sizeof(char *) * (i + 1));
	fd = open(INTEGRITY_FILE, O_RDONLY);
	i = -1;
	while (get_next_line(fd, &buff) > 0)
	{
		src[++i] = ft_strdup(buff);
		free(buff);
		buff = NULL;
	}
	close(fd);
	src[i + 0] = NULL;
	*integrity = parse_integrity(src, -1);
}

void		validity_creator_opt(int i, int a, t_integrity *i_m, t_tags *tags)
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
				while (i_m[i].optional[++a].tag)
				{
					if (!ft_strcmp(tags->tags->tag, i_m[i].optional[a].tag))
					{
						i_m[i].optional[a].saved += 1;
						i_m[i].optional[a].present = 1;
					}
				}
				tags->tags = tags->tags->next;
			}
		}
	}
}

int			contain_valid(t_tags *tags, t_integrity *i_m)
{
	int	i;
	int	a;

	i = -1;
	a = -1;
	validity_creator_opt(i, a, i_m, tags);
	i = -1;
	while (i_m[++i].original != NULL)
	{
		a = -1;
		while (i_m[i].optional[++a].tag)
		{
			if (i_m[i].optional[a].present && i_m[i].optional[a].times > 0
				&& i_m[i].optional[a].times < i_m[i].optional[a].saved)
			{
				ft_printf("tag contain bad number of optional args : %s, %s ==\
					%d\n", i_m[i].original, i_m[i].\
					optional[a].tag, i_m[i].optional[a].saved);
				return (0);
			}
		}
	}
	while (tags->tags && tags->tags->prev)
		tags->tags = tags->tags->prev;
	return (1);
}
