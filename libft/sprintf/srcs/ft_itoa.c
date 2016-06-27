/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <cchaumar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/22 20:57:22 by cchaumar          #+#    #+#             */
/*   Updated: 2016/03/08 07:08:28 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sprintf.h"

static size_t	ft_getsize(uint64_t n)
{
	size_t	size;

	size = (n == 0) ? 1 : 0;
	while (n)
	{
		size++;
		n /= 10;
	}
	return (size);
}

char			*ft_anytoa(uint64_t n, int pre, int flag)
{
	char	*result;
	size_t	i;
	size_t	j;

	i = ft_getsize(n);
	if (flag)
		i += ((i - 1) / 3);
	i = ((int)i < pre) ? pre : i;
	if ((result = malloc(i + 1)) == NULL)
		return (NULL);
	result[i] = '\0';
	j = 0;
	while (i--)
	{
		result[i] = n % 10 + '0';
		if ((++j % 3) == 0 && flag && (n / 10))
			result[--i] = *localeconv()->thousands_sep;
		n /= 10;
	}
	return (result);
}

char			*ft_anytoa_bin(uint64_t n, int base, int pre, int flag)
{
	int			i;
	int			off;
	uint64_t	pow;
	char		*buff;
	char		*chars;

	chars = "0123456789abcdef";
	if (base == 1)
		return (ft_anytoa(n, pre, flag));
	i = 1;
	pow = n;
	off = (base == 8) ? 4 : base - 1;
	while ((pow >>= off))
		++i;
	i = (i < pre) ? pre : i;
	if ((buff = malloc(i + 1)) == NULL)
		return (NULL);
	buff[i] = '\0';
	while (i--)
	{
		buff[i] = chars[n & ((base == 2) ? base : base * 2) - 1];
		n >>= (base == 8) ? 4 : base - 1;
	}
	return (buff);
}
