#include "parser.h"

int		ft_check_vect(char *str, t_tags *tags)
{
	int	i;
	int	size;

	size = ft_strlen(str);
	i = -1;
	while (++i < size)
	{
		if (!ft_isdigit(str[i]) && str[i] != ' ' && str[i] != '.'
			&& str[i] != '-' && str[i] != '+')
		{
			printf("bad arguments to tag : %s, str[i] : %c\n",\
			tags->tag, str[i]);
			return (-1);
		}
	}
	return (0);
}

void	vec3f_from_text(t_tags *tags)
{
	int		i;
	char	**nbrs;

	i = -1;
	nbrs = ft_strsplit(tags->content_text, ' ');
	if (ft_tablen(nbrs) != 3)
	{
		ft_printf("wrong number of arguments to tag : %s\n", tags->tag);
		exit(0);
	}
	tags->content->x = ft_ftoa(nbrs[0]);
	tags->content->y = ft_ftoa(nbrs[1]);
	tags->content->z = ft_ftoa(nbrs[2]);
}

float	ft_ftoa(char *str)
{
	int		i;
	int		sign;
	float	r;
	float	ret;

	i = -1;
	ret = 0.0;
	sign = 0;
	if (str[i + 1] == '-' || str[i + 1] == '+')
	{
		(str[i + 1] == '-') ? (sign = 1) : (0);
		i++;
	}
	while (ft_isdigit(str[++i]))
	{
		ret *= 10.0;
		ret += (float)(str[i] - '0');
	}
	if (str[i] == '.')
	{
		r = 1.0f;
		while (ft_isdigit(str[++i]) && (r *= 10.0f))
			ret += (float)(str[i] - '0') / r;
	}
	return ((sign) ? (-ret) : (ret));
}

char	*get_corresponding_attribute_tag(char *config_file, char *tag)
{
	int			fd;
	char		*buff;

	fd = open(config_file, O_RDONLY);
	buff = NULL;
	if (fd < 0)
	{
		ft_printf("Could not open grammar config file\n");
		exit(0);
	}
	while (get_next_line(fd, &buff) > 0)
	{
		if (!ft_strcmp(ft_strsub(buff, 0, ft_strlen(tag)), tag))
			return (buff);
		free(buff);
	}
	ft_printf("could not find grammar rules for tag : %s\n", tag);
	exit(0);
}

void	ft_little_if_2(int boolee, t_tags *tags)
{
	if (!boolee)
	{
		ft_printf("Bad attribute : %s, to tag : %s \n",\
		tags->attribute, tags->tag);
		exit(0);
	}
}
