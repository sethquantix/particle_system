/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_conv.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/17 20:37:06 by cchaumar          #+#    #+#             */
/*   Updated: 2016/03/09 06:15:05 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/printf.h"

size_t			ft_sign(char *buff, size_t offset, int sign, t_format *opt)
{
	char	*s;

	s = buff;
	if (sign == -1)
		*(((opt->flags & ZER_P) ? 0 : offset) + (buff++)) = '-';
	if (sign == 1)
	{
		if (opt->flags & SGN_P)
			*(((opt->flags & ZER_P) ? 0 : offset) + (buff++)) = '+';
		if (opt->flags & BLK_P)
			*(((opt->flags & ZER_P) ? 0 : offset) + (buff++)) = ' ';
	}
	if (opt->flags & ALT_P)
	{
		ft_strcpy(s + ((opt->flags & ZER_P) ? 0 : offset),
				(opt->type & OCT_V) ? "0" : "0x");
		buff += (opt->type & OCT_V) ? 1 : 2;
		s[((opt->flags & ZER_P) ? 0 : offset) + ((opt->type & OCT_V) ?
				1 : 2)] = (opt->flags & ZER_P) ? '0' : ' ';
	}
	return (buff - s);
}

char			*ft_getwc(unsigned int c)
{
	t_wchar			wc;
	unsigned int	tmp;
	size_t			i;
	size_t			j;

	wc.n = (c > BYTE2) ? MASK_2 : 0;
	wc.n = (c > BYTE3) ? MASK_3 : wc.n;
	wc.n = (c > BYTE4) ? MASK_4 : wc.n;
	i = ((c > BYTE4) ? 1 : 0) + ((c > BYTE3) ? 1 : 0) + ((c > BYTE2) ? 1 : 0);
	j = i;
	while (j)
	{
		wc.n |= (c & 0x3F) << ((i - j) * 8);
		c >>= 6;
		j--;
	}
	wc.n |= c << (8 * (i));
	tmp = wc.n;
	j = (++i);
	while (j--)
	{
		wc.s[j] = tmp & 0xff;
		tmp >>= 8;
	}
	return (ft_strdup(wc.s));
}

static uint64_t	ft_unsigned(unsigned short mod, uint64_t n)
{
	n = (uint64_t)((mod & HHMOD) ? (unsigned char)n : n);
	n = (uint64_t)((mod & H_MOD) ? (unsigned short)n : n);
	n = (uint64_t)((mod & LLMOD) ? (unsigned long long)n : n);
	n = (uint64_t)((mod & (L_MOD | SZMOD | IMMOD | PTMOD))
				? (unsigned long)n : n);
	n = (uint64_t)(((mod & 0xFF00) == U_MOD) ? (unsigned int)n : n);
	return (n);
}

int64_t			ft_conv_dec(int64_t n, unsigned short mod, int *sign)
{
	if (mod & U_MOD)
		return (ft_unsigned(mod, n));
	if (mod & HHMOD)
		n = (uint64_t)((char)n);
	if (mod & H_MOD)
		n = (uint64_t)((short)n);
	if ((mod & 0x7f00) == 0)
		n = (uint64_t)((int)n);
	*sign = (n >> 63) ? -1 : 1;
	n *= *sign;
	return (n);
}
