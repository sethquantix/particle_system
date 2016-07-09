# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/02/23 12:31:27 by cchaumar          #+#    #+#              #
#    Updated: 2016/07/09 12:36:10 by cchaumar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME   = particle
CFLG   =  -Wextra -Wall -Ofast -g
FILES  = main.c loops.c vect.c opencl.c device.c mat.c draw.c \
		 vect2.c mat2.c keys.c key_funcs.c
SRCD   = srcs/
OBJD   = objs/
GITLIB = 'https://github.com\/sethquantix/libft.git'
MLX    = -Lmlx -lmlx
SRCS   = $(addprefix $(SRCD),$(FILES))
OBJS   = $(addprefix $(OBJD),$(FILES:.c=.o))
INC    = -Iincludes/ -Imlx/
FROPEN = -framework OpenGL -framework OpenCL
FRMCOS = -framework Appkit
FRAMEWORKS = $(FROPEN) $(FRMCOS)

all : libft $(NAME)

$(OBJD)%.o:$(SRCD)%.c includes/particle.h
	@mkdir -p objs
	gcc $(CFLG) $(INC) -c -o $@ $<

libft:
		sh lib.sh

$(NAME): $(OBJS) libft
		make -C libft
		make -C mlx
		gcc $(CFLG) -o $(NAME) $(INC) $(OBJS) -Llibft -lft $(MLX) $(FRAMEWORKS)

nolib:
		gcc $(CFLG) -o $(NAME) $(INC) $(SRCS) -Llibft -lft $(MLX) $(FRAMEWORKS)

clean:
		rm -f $(OBJS)
		make -C libft clean
		make -C mlx clean

fclean: clean
		rm -f $(NAME)
		make -C libft fclean

re: fclean all
