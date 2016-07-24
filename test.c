/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/18 15:20:47 by cchaumar          #+#    #+#             */
/*   Updated: 2016/07/21 17:11:35 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"

void	 ft_foreach(int *tab, int length, void (*f)(int))
{
	while (length--)
		f(*(tab++));
}

void	useless(void)
{
	int	i;

	i = 0;
	if (++i)
		ft_printf("++i == 0 !\n");
	if (i++)
		ft_printf("i++ == 0 !\n");
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

long	rec(char *s, long n)
{
	if (!(*s))
		return (n);
	return (rec(s + 1, n * 10 + *s - '0'));
}

int		ft_atoi(char *s)
{
	int		sign;
	long	n;

	sign = *s == '-' ? -1 : 0;
	sign = *s == '+' ? 1 : sign;
	if (sign)
		s++;
	else
		sign = 1;
	n = rec(s, 0);
	return (int)(sign * n);
}

int main(int ac, char **av)
{

	int		tab[6] = {0, 1, 2, 3, 4, 5};
	ft_foreach(tab, 6, ft_putnbr);
	//ft_printf("match :%d | nmatch : %d\n", match(av[1], av[2]),
	//	nmatch(av[1], av[2]));
	ft_printf("%d\n", ft_atoi("-2147483648"));
	(void)ac;
	useless();
	return (0);
}

