/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <cchaumar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 00:16:24 by cchaumar          #+#    #+#             */
/*   Updated: 2016/05/23 18:13:33 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPRINTF_H
# define SPRINTF_H
# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdint.h>
# include <stddef.h>
# include <fcntl.h>
# include <locale.h>
# include <stdio.h>
# include "libft.h"
# include "defines.h"

typedef struct	s_format
{
	int				arg;
	int				width;
	int				precision;
	unsigned char	flags;
	unsigned short	type;
	char			spec;
}				t_format;

typedef union	u_wchar
{
	unsigned int	n;
	char			s[4];
}				t_wchar;

size_t			g_null;

char			*ft_sprintf(char const *format, ...);
t_format		*get_format(char **format, va_list va, int deep);
void			ft_nbrs(char **s, va_list va, t_format *opt);
int				ft_get_nbrs(char **s);
void			get_precision(char **s, va_list va, t_format *opt);
unsigned char	ft_flags(char **s);
int				ft_get_type(char **tmp, t_format *opt);
char			*ft_ctos(char c);
char			*ft_build(va_list args, t_format *opt);
char			*ft_var(va_list va, t_format *opt);
char			*ft_u_var(va_list va, t_format *opt);
char			*ft_up_var(va_list va, t_format *opt);
char			*ft_u_up_var(va_list va, t_format *opt);
char			*ft_anytoa_bin(uint64_t n, int base, int pre, int flag);
char			*ft_decimal(va_list va, t_format *opt);
char			*ft_double(va_list va, t_format *opt);
char			*ft_char(va_list va, t_format *opt);
char			*ft_str(va_list va, t_format *opt);
char			*ft_ptr(va_list va, t_format *opt);
char			*ft_intab(va_list va, t_format *opt);
char			*ft_getwc(unsigned int c);
char			*ft_wstr(unsigned int *buf, t_format *opt, size_t size);
int64_t			ft_conv_dec(int64_t n, unsigned short mod, int *sign);
void			ft_fill(char *nbuff, char *buff, t_format *opt, int sign);
size_t			ft_sign(char *buff, size_t offset, int sign, t_format *opt);
char			*ft_alter(char *buff, t_format *opt, int sign, size_t size);

#endif
