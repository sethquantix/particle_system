/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/27 05:14:19 by cchaumar          #+#    #+#             */
/*   Updated: 2016/02/29 23:26:43 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_H
# define COLORS_H
# define COLORS		10

typedef struct	s_color
{
	char	name[4];
	char	color[6];
}				t_color;

t_color			g_colors[COLORS] = {
	{"NRM", "\x1B[0m"},
	{"RED", "\x1B[31m"},
	{"GRN", "\x1B[32m"},
	{"YEL", "\x1B[33m"},
	{"BLU", "\x1B[34m"},
	{"MAG", "\x1B[35m"},
	{"CYN", "\x1B[36m"},
	{"WHT", "\x1B[37m"},
	{"RST", "\033[0m"}};

#endif
