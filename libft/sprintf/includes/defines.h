/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/11 01:17:35 by cchaumar          #+#    #+#             */
/*   Updated: 2016/02/27 05:13:27 by cchaumar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H
/*
**behaviour will be undefined for values above 2^63 - 1
**as decimal types cast conversion can fail; behaviour is still correct
**if input is of the right type (in short, dont screw with my printf)
*/
# define BUFFSIZE	1024

# define BYTE2		0x7F
# define BYTE3		0x7FF
# define BYTE4		0xFFFF
# define MASK_2		0xC080
# define MASK_3		0xE08080
# define MASK_4		0xF0808080
/*
**value type ; if uppercase, set L_MOD as well
*/
# define INT_V		0x01
# define BIN_V		0x02
# define OCT_V		0x04
# define HEX_V		0x08

/*
**for double support ; may be implemented later
*/
# define DDECV		0x11
# define DEXPV		0x12
# define DVARV		0x14
# define DHEXV		0x18

# define CHARV		0x20
# define STR_V		0x40
# define SZE_V		0x41
# define PTR_V		0x42
# define INTAB		0x44
# define UPPER		0x80
/*
**size modulation
*/
# define HHMOD		0x0100
# define H_MOD		0x0200
# define L_MOD		0x0400
# define LLMOD		0x0800
# define IMMOD		0x1000
# define SZMOD		0x2000
# define PTMOD		0x4000
/*
**value is unsigned; INT, BIN, OCT and HEX values only
*/
# define U_MOD		0x8000
/*
**flags
*/
# define ZER_P		0x01
# define NEG_P		0x02
# define SGN_P		0x04
# define BLK_P		0x08
# define THD_P		0x10
# define ALT_P		0x20

#endif
