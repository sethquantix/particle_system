/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_out.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 01:32:06 by cchaumar          #+#    #+#             */
/*   Updated: 2016/03/08 07:08:33 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sprintf.h"

char	*ft_wstr(unsigned int *buf, t_format *opt, size_t size)
{
	char	*tmp;
	char	buff[1024];

	while (*buf)
	{
		tmp = ft_getwc(*(buf++));
		if (opt->precision == -1 ||
				ft_strlen(tmp) + size <= (size_t)opt->precision)
		{
			ft_strcpy(buff + size, tmp);
			size += ft_strlen(tmp);
		}
		else
			break ;
	}
	return (ft_alter(ft_strndup(buff, size), opt, 0, size));
}

char	*ft_intab(va_list va, t_format *opt)
{
	int		size;
	int		*tab;
	char	*buff;

	buff = NULL;
	opt->type = STR_V;
	size = va_arg(va, int);
	tab = va_arg(va, int *);
	buff = ft_anytoa_bin(*tab, 1, -1, 0);
	buff = ft_strjoinfree(buff, " ", 1);
	while (--size)
	{
		buff = ft_strjoinfree(buff, ft_anytoa_bin(*(++tab), 1, -1, 0), 3);
		if (size != 1)
			buff = ft_strjoinfree(buff, " ", 1);
	}
	return (buff);
}

char	*ft_ctos(char c)
{
	char	*s;

	if ((s = malloc(2)) == NULL)
		return (NULL);
	s[0] = c >= 0 && c < 31 && !ft_iswhite(c) ? 0 : c;
	s[1] = '\0';
	return (s);
}

void	get_precision(char **s, va_list va, t_format *opt)
{
	++(*s);
	if (**s == '*')
	{
		opt->precision = va_arg(va, int);
		++(*s);
	}
	else
		opt->precision = ft_get_nbrs(s);
}
