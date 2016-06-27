# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cchaumar <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/02/23 12:31:27 by cchaumar          #+#    #+#              #
#    Updated: 2016/04/29 20:31:25 by cchaumar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME   = rtv1
CFLG   =  -Wextra -Wall -Ofast -g -fsanitize=address
FILES  = main.c loops.c vect.c init.c device.c parser.c parser_1.c parser_2.c parser_3.c parser_4.c parser_5.c parser_6.c parser_7.c
SRCD   = srcs/
OBJD   = objs/
MLX    = -Lmlx -lmlx
SRCS   = $(addprefix $(SRCD),$(FILES))
OBJS   = $(addprefix $(OBJD),$(FILES:.c=.o))
INC    = -Iincludes/ -Imlx/
FROPEN = -framework OpenGL -framework OpenCL
FRMCOS = -framework Appkit
FRAMEWORKS = $(FROPEN) $(FRMCOS)

all : $(NAME)

$(OBJD)%.o:$(SRCD)%.c
	gcc $(CFLG) $(INC) -c -o $@ $<

$(NAME): $(OBJS)
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
