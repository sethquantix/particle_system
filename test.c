/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/18 15:20:47 by cchaumar          #+#    #+#             */
/*   Updated: 2016/07/18 16:06:01 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"

void	 ft_foreach(int *tab, int length, void (*f)(int))
{
	while (length--)
		f(*(tab++));
}

int		match(char *s1, char *s2)
{
	if (!(*s1) && !(*s2))
		return (1);
	if (*s1 == *s2 && *s1 != '*')
		return (match(s1 + 1, s2 + 1));
	if (!(*s1) && *s2 == '*')
		return (match(s1, s2 + 1));
	if (!(*s2) && *s1 == '*')
		return (match(s1 + 1, s2));
	if (*s1 == '*' && *s2 == '*')
		return (match(s1, s2 + 1) | match(s1 + 1, s2));
	if (*s1 == '*' && *s2 != '*')
		return (match(s1 + 1, s2) | match(s1, s2 + 1));
	if (*s1 != '*' && *s2 == '*')
		return (match(s1 + 1, s2) | match(s1, s2 + 1));
	return (0);
}

int		nmatch(char *s1, char *s2)
{
	if (!(*s1) && !(*s2))
		return (1);
	if (*s1 == *s2 && *s1 != '*')
		return (nmatch(s1 + 1, s2 + 1));
	if (!(*s1) && *s2 == '*')
		return (nmatch(s1, s2 + 1));
	if (!(*s2) && *s1 == '*')
		return (nmatch(s1 + 1, s2));
	if (*s1 == '*' && *s2 == '*')
		return (nmatch(s1, s2 + 1) + nmatch(s1 + 1, s2));
	if (*s1 == '*' && *s2 != '*')
		return (nmatch(s1 + 1, s2) + nmatch(s1, s2 + 1));
	if (*s1 != '*' && *s2 == '*')
		return (nmatch(s1 + 1, s2) + nmatch(s1, s2 + 1));
	return (0);
}

int main(int ac, char **av)
{
	ft_printf("match :%d | nmatch : %d\n", match(av[1], av[2]),
		nmatch(av[1], av[2]));
	(void)ac;
	return (0);
}
