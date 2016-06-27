/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_var.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/13 18:20:02 by cchaumar          #+#    #+#             */
/*   Updated: 2016/03/08 07:08:38 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sprintf.h"

char	*ft_alter(char *buff, t_format *opt, int sign, size_t n)
{
	size_t	an;
	char	*nbuff;
	size_t	off;

	an = ((opt->flags & (SGN_P + BLK_P)) || sign == -1) ? n + 1 : n;
	if (opt->flags & ALT_P)
	{
		an += (opt->type & OCT_V) ? 1 : 0;
		an += (opt->type & HEX_V) ? 2 : 0;
	}
	off = ((int)an > opt->width) ? an : opt->width;
	nbuff = ft_memset(ft_memalloc(off + 1), (opt->flags & ZER_P)
			? '0' : ' ', off);
	off = (opt->flags & NEG_P) ? 0 : off - an;
	off -= n ? 0 : 1;
	off += ft_sign(nbuff, off, sign, opt);
	ft_strncpy(nbuff + off, buff, n);
	if (!(*buff) && opt->type & CHARV)
	{
		nbuff[off] = '\0';
		g_null += (an != ft_strlen(nbuff)) ? 1 : 0;
	}
	ft_memdel((void **)&buff);
	return (nbuff);
}

char	*ft_decimal(va_list va, t_format *opt)
{
	char			*buff;
	int64_t			val;
	int				sign;
	unsigned short	type;

	sign = 1;
	type = opt->type & 0xf;
	val = ft_conv_dec(va_arg(va, int64_t), opt->type, &sign);
	if (opt->precision == 0 && val == 0)
		buff = ((opt->flags & ALT_P) && (opt->type & OCT_V)) ?
			ft_strdup("0") : ft_strdup("");
	else
		buff = ft_anytoa_bin(val, type, opt->precision,
			(opt->flags & THD_P) && *localeconv()->thousands_sep ? 1 : 0);
	opt->flags &= (val == 0) ? 0xff - (ALT_P) : 0xff;
	buff = ft_alter(buff, opt, sign, ft_strlen(buff));
	if (opt->type & UPPER)
		ft_strtoupper(buff);
	return (buff);
}

char	*ft_char(va_list va, t_format *opt)
{
	char			*buff;
	unsigned int	c;

	if (!(opt->type & L_MOD))
	{
		c = va_arg(va, unsigned int);
		buff = ft_ctos((char)c);
		buff = ft_alter(buff, opt, 0, ft_strlen(buff));
		g_null += *buff ? 0 : 1;
	}
	else
	{
		c = va_arg(va, unsigned int);
		buff = ft_getwc(c);
		buff = ft_alter(buff, opt, 0, ft_strlen(buff));
	}
	return (buff);
}

char	*ft_str(va_list va, t_format *opt)
{
	char			*buff;
	unsigned int	*buf;
	size_t			size;

	if (!(opt->type & L_MOD))
	{
		(void)buf;
		buff = va_arg(va, char *);
		size = buff ? ft_strlen(buff) : 6;
		if (opt->precision != -1)
			size = ((int)size < opt->precision) ? size : opt->precision;
		buff = ft_alter(ft_strndup(buff ? buff : "(null)", size), opt, 0, size);
	}
	else
	{
		buf = va_arg(va, unsigned int *);
		return (ft_wstr(buf, opt, 0));
	}
	return (buff);
}

char	*ft_ptr(va_list va, t_format *opt)
{
	char	*buff;

	opt->flags |= ALT_P;
	opt->type = HEX_V + L_MOD;
	buff = ft_anytoa_bin((uint64_t)va_arg(va, unsigned long), 16, 0, 0);
	buff = ft_alter(buff, opt, 1, ft_strlen(buff));
	return (buff);
}
