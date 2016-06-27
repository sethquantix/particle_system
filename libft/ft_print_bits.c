/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_bits.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <cchaumar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/08 20:11:28 by cchaumar          #+#    #+#             */
/*   Updated: 2016/05/23 18:11:39 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_print_bits(size_t const size, void const *const ptr)
{
	unsigned char	*b;
	unsigned char	byte;
	size_t			i;
	size_t			j;

	b = (unsigned char *)ptr;
	i = 0;
	while (i < size)
	{
		j = 8;
		while (j--)
		{
			byte = (b[i] & (1 << j)) >> j;
			ft_putchar('0' + byte);
		}
		ft_putchar(' ');
		++i;
	}
	ft_putchar('\n');
}
