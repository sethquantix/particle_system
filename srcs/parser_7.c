#include "parser.h"

int		ft_check_color(char *s)
{
	int	a;

	a = 1;
	if (ft_strlen(s) != 8)
	{
		ft_printf("color tag content of invalid length\n");
		return (-1);
	}
	if (s[0] != '0' || s[1] != 'x')
	{
		ft_printf("color tag contain trash content\n");
		return (-1);
	}
	while (s[++a])
	{
		if (!((s[a] >= '0' && s[a] <= '9') || (s[a] >= 'a' && s[a] <= 'f')))
		{
			ft_printf("color tag contain trash content\n");
			return (-2);
		}
	}
	return (1);
}

void	color_from_text(t_tags *tags)
{
	int	color;

	color = ft_atoi(tags->content);
	tags->content->x = color & 0xff;
	tags->content->y = (color << 8) & 0xff;
	tags->content->z = (color << 16) & 0xff;
}

int		count_without_spaces(char *src)
{
	int	i;
	int	ret;

	i = -1;
	ret = 0;
	while (src[++i])
	{
		if (ft_iswhite(src[i]))
		{
			while (ft_iswhite(src[i]))
				i++;
			ret += 1;
		}
		ret++;
	}
	return (ret);
}

char	*ft_exterminate(char *src)
{
	int		i;
	int		a;
	int		len;
	char	*ret;

	len = 0;
	len = count_without_spaces(src);
	ret = ft_memalloc(sizeof(char) * (len + 1));
	i = -1;
	a = 0;
	while (src[++i] && a <= len)
	{
		if (ft_iswhite(src[i]))
		{
			while (ft_iswhite(src[i]))
				i++;
			ret[a] = ' ';
			a++;
		}
		ret[a] = src[i];
		a++;
	}
	ret[a] = '\0';
	free(src);
	return (ret);
}
