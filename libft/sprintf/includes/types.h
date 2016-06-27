/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/18 16:27:35 by cchaumar          #+#    #+#             */
/*   Updated: 2016/02/27 07:04:38 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H
# include "defines.h"

typedef struct	s_types
{
	char			c;
	unsigned short	type;
}				t_types;

typedef struct	s_mods
{
	char			s[3];
	unsigned short	mod;
}				t_mods;

t_types			g_types[26] = {
	{'d', INT_V},
	{'D', L_MOD + INT_V},
	{'i', INT_V},
	{'b', U_MOD + BIN_V},
	{'B', U_MOD + BIN_V + L_MOD},
	{'o', U_MOD + OCT_V},
	{'O', U_MOD + OCT_V + L_MOD},
	{'x', U_MOD + HEX_V},
	{'X', U_MOD + HEX_V + UPPER},
	{'u', U_MOD + INT_V},
	{'U', U_MOD + INT_V + L_MOD},
	{'e', DDECV},
	{'E', L_MOD + DDECV},
	{'f', DEXPV},
	{'F', L_MOD + DEXPV},
	{'a', DHEXV},
	{'A', L_MOD + DHEXV},
	{'g', DVARV},
	{'G', L_MOD + DVARV},
	{'c', CHARV},
	{'C', CHARV + L_MOD},
	{'s', STR_V},
	{'S', STR_V + L_MOD},
	{'p', PTR_V},
	{'T', INTAB}};

t_mods			g_mods[8] = {
	{"hh", HHMOD},
	{"ll", LLMOD},
	{"h\0", H_MOD},
	{"l\0", L_MOD},
	{"L\0", LLMOD + L_MOD},
	{"j\0", IMMOD},
	{"t\0", PTMOD},
	{"z\0", SZMOD}};

#endif
