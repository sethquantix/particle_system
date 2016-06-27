/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_format.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 01:14:49 by cchaumar          #+#    #+#             */
/*   Updated: 2016/03/08 07:08:15 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sprintf.h"

static t_format	*ft_strip(t_format *opt)
{
	if (opt->type == PTR_V)
	{
		opt->type = HEX_V + L_MOD;
		opt->flags |= ALT_P;
	}
	if (opt->flags & NEG_P || (opt->type &&
				(opt->type & 0xf0) == 0
				&& opt->precision && opt->precision != -1))
		opt->flags &= 0xff - ZER_P;
	if ((opt->flags & ZER_P) && (opt->type & INT_V) &&
			(opt->flags & ALT_P || opt->precision != -1))
		opt->flags &= 0xff - ZER_P;
	if ((opt->type & INT_V) == 0 || opt->type & U_MOD)
		opt->flags &= 0xff - (BLK_P | SGN_P);
	else if (opt->flags & SGN_P)
		opt->flags &= 0xff - BLK_P;
	if ((opt->type & INT_V) == 0)
		opt->flags &= 0xff - THD_P;
	if ((opt->type & 0x000f) < 4)
		opt->flags &= 0xff - ALT_P;
	return (opt);
}

t_format		*ft_merge(t_format *opt, t_format *more)
{
	opt->arg = (opt->arg == -1) ? more->arg : opt->arg;
	opt->width = (opt->width == -1) ? more->width : opt->width;
	opt->precision = (opt->precision == -1) ? more->precision : opt->precision;
	opt->flags |= more->flags;
	opt->type = (((opt->type & 0xff00) && (more->type & 0x7f00) == 0) ?
		opt->type & 0xff00 : more->type & 0xff00) + (more->type & 0xff);
	opt->spec = more->spec;
	ft_memdel((void **)&more);
	return (opt);
}

t_format		*get_format(char **s, va_list va, int deep)
{
	t_format	*opt;

	if ((opt = malloc(sizeof(t_format))) == NULL)
		return (NULL);
	opt->precision = -1;
	opt->width = -1;
	opt->arg = -1;
	opt->type = 0;
	opt->flags = ft_flags(s);
	ft_nbrs(s, va, opt);
	if (ft_get_type(s, opt))
		if (ft_strchr("0123456789#.+- *'", opt->spec) != NULL)
			opt = ft_merge(opt, get_format(s, va, deep + 1));
	if (deep == 0 && **s)
		++(*s);
	return (ft_strip(opt));
}

char			*ft_get(va_list va, t_format *opt)
{
	char	*buff;

	buff = NULL;
	if ((opt->type & 0xf0) == 0 || (opt->type & 0xff) == (HEX_V + UPPER))
		buff = ft_decimal(va, opt);
	if (opt->type & CHARV)
		buff = ft_char(va, opt);
	if ((opt->type & 0xff) == STR_V)
		buff = ft_str(va, opt);
	if ((opt->type & 0xff) == PTR_V)
		buff = ft_ptr(va, opt);
	if ((opt->type & 0xff) == INTAB)
		buff = ft_intab(va, opt);
	return (buff);
}

char			*ft_build(va_list args, t_format *opt)
{
	va_list			ptr;
	char			*content;
	int				i;

	if (opt->type & 0xff)
	{
		if (opt->arg > 0)
		{
			va_copy(ptr, args);
			i = -1;
			while (++i < opt->arg)
				va_arg(ptr, void *);
			content = ft_get(ptr, opt);
			va_end(ptr);
		}
		else
			content = ft_get(args, opt);
	}
	else
		content = ft_alter(ft_ctos(opt->spec), opt, 0, 1);
	return (content);
}
