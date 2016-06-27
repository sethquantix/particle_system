/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_binary.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/13 19:40:12 by cchaumar          #+#    #+#             */
/*   Updated: 2016/05/23 18:12:05 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	ft_pow(int n, int pow)
{
	return ((pow == 0) ? 1 : n * ft_pow(n, pow - 1));
}

char		*ft_itoa_binary(int n, int base)
{
	int		i;
	int		pow;
	char	*buff;
	char	*chars;

	chars = "0123456789ABCDEF";
	if (base == 1)
		return (ft_itoa(n));
	i = 0;
	while (ft_pow(base, i) < n)
		i++;
	if ((buff = malloc(i + 1)) == NULL)
		return (NULL);
	buff[i] = '\0';
	while (i--)
	{
		pow = ft_pow(base, i);
		buff[i] = chars[n / pow];
		n -= pow * (n / pow);
	}
	return (buff);
}
