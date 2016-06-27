/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 01:50:50 by cchaumar          #+#    #+#             */
/*   Updated: 2016/03/02 01:31:22 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/printf.h"
#include "../includes/types.h"
#include <stdio.h>

int				ft_get_nbrs(char **s)
{
	char			*p;
	char			*nbr;
	unsigned int	arg;

	p = *s;
	while (ft_isdigit(*p))
		p++;
	if (p == *s)
		return (0);
	nbr = ft_strndup(*s, p - *s);
	arg = ft_atoi(nbr);
	ft_memdel((void **)&nbr);
	*s = p;
	return (arg);
}

void			ft_nbrs(char **s, va_list va, t_format *opt)
{
	int		val;

	val = -1;
	if (**s == '.')
		get_precision(s, va, opt);
	else
	{
		if (ft_isdigit(**s) || **s == '*')
		{
			if (**s == '*')
			{
				val = va_arg(va, int);
				++(*s);
			}
			else
				val = ft_get_nbrs(s);
		}
		if (**s == '$')
			opt->arg = (val == -1) ? 1 : val;
		else
			opt->width = (val == -1) ? opt->width : val;
	}
}

unsigned char	ft_flags(char **s)
{
	unsigned char	val;
	unsigned char	ret;

	ret = 0;
	val = 1;
	while (val)
	{
		val = (**s == '#') ? ALT_P : 0;
		val = (**s == '0') ? ZER_P : val;
		val = (**s == '-') ? NEG_P : val;
		val = (**s == ' ') ? BLK_P : val;
		val = (**s == '+') ? SGN_P : val;
		val = (**s == '\'') ? THD_P : val;
		if (val)
			(*s)++;
		ret |= val;
	}
	return (ret);
}

unsigned short	ft_mod(char **s, unsigned short mod)
{
	int				i;
	char			stmp[2];
	char			ltmp[3];

	if (!(**s))
		return (0);
	stmp[1] = '\0';
	ltmp[2] = '\0';
	i = 0;
	while (i != 8)
	{
		i = -1;
		ft_strncpy(stmp, *s, 1);
		ft_strncpy(ltmp, *s, 2);
		while (++i < 8)
			if (ft_strcmp(stmp, g_mods[i].s) == 0 ||
					ft_strcmp(ltmp, g_mods[i].s) == 0)
			{
				*s += ft_strlen(g_mods[i].s);
				mod = (mod > g_mods[i].mod) ? mod : g_mods[i].mod;
				break ;
			}
	}
	return (mod);
}

int				ft_get_type(char **tmp, t_format *opt)
{
	unsigned short	type;
	size_t			i;
	char			c;

	i = 0;
	type = ft_mod(tmp, 0);
	opt->type = type;
	if (!**tmp)
		return (0);
	opt->spec = **tmp;
	c = **tmp;
	while (i < 26)
		if (c == g_types[i].c)
		{
			opt->type = (g_types[i].type & L_MOD) ? g_types[i].type :
				opt->type | g_types[i].type;
			break ;
		}
		else
			++i;
	return ((opt->type == type) ? 1 : 0);
}
