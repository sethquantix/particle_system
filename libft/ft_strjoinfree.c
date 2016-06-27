/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoinfree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <cchaumar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/04 09:18:09 by cchaumar          #+#    #+#             */
/*   Updated: 2016/05/23 18:18:03 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#define FREE_S1		1
#define FREE_S2		2

char	*ft_strjoinfree(char const *s1, char const *s2, int mask)
{
	char	*o;
	int		i;
	int		j;

	i = -1;
	j = -1;
	if ((o = (char *)malloc(ft_strlen((char *)s1) + ft_strlen((char *)s2)
			+ 1)) == NULL)
		return (NULL);
	if (s1)
		while (s1[++i])
			o[i] = s1[i];
	i = i == -1 ? 0 : i;
	if (s2 != NULL)
		while (s2[++j])
			o[i + j] = s2[j];
	j = j == -1 ? 0 : j;
	o[i + j] = '\0';
	if (mask & FREE_S1)
		ft_memdel((void **)&s1);
	if (mask & FREE_S2)
		ft_memdel((void **)&s2);
	return (o);
}
