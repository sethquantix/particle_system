/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <cchaumar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 00:04:02 by cchaumar          #+#    #+#             */
/*   Updated: 2016/03/02 01:33:10 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/printf.h"
#include "../includes/colors.h"

static char	*ft_realloc(char *buff, size_t size)
{
	char	*newbuff;

	if ((newbuff = ft_memalloc(size + BUFFSIZE)) == NULL)
		return (NULL);
	if (buff)
	{
		ft_strcpy(newbuff, buff);
		ft_memdel((void **)&buff);
	}
	return (newbuff);
}

static void	ft_add(char **buff, char *str, size_t *size, int type)
{
	static size_t	max;
	size_t			len;

	if (!(*size))
		max = 0;
	len = type == 1 ? 1 : ft_strlen(str);
	if (*buff == NULL || *size + len > max)
	{
		*buff = ft_realloc(*buff, *size);
		max += BUFFSIZE;
	}
	ft_strncpy(*buff + *size, str, len);
	if (type != 1)
		ft_memdel((void **)&str);
	*size += len;
}

static char	*set_color(char **s)
{
	char	*color;
	int		i;

	i = -1;
	if (**s == '}')
	{
		++(*s);
		return (ft_strdup(g_colors[0].color));
	}
	*s += 2;
	color = ft_strndup(*s, 3);
	while (++i < COLORS)
		if (ft_strcmp(color, g_colors[i].name) == 0)
		{
			ft_memdel((void **)&color);
			break ;
		}
	*s += 3;
	return (ft_strdup(g_colors[i].color));
}

static void	ft_switch(char **s, va_list args, size_t *size, char **p)
{
	t_format	*opt;
	int			x;
	int			d;

	x = 0;
	d = 0;
	if (**s == '%')
	{
		++(*s);
		if (**s)
			if ((opt = get_format(s, args, 0)))
			{
				ft_add(p, ft_build(args, opt), size, 0);
				ft_memdel((void **)&opt);
			}
	}
	else if ((**s == '{' && *(*s + 1) == 'C') || **s == '}')
		ft_add(p, set_color(s), size, 2);
	else
	{
		ft_add(p, *s, size, 1);
		++(*s);
	}
}

int			ft_printf(const char *format, ...)
{
	va_list			args;
	char			*p;
	size_t			size;

	va_start(args, format);
	p = NULL;
	size = 0;
	while (*format)
		ft_switch((char **)&format, args, &size, &p);
	ft_putstr(p);
	ft_memdel((void **)&p);
	va_end(args);
	return (size + g_null);
}
