CFLG  = -Wextra -Wall -Werror
SRCD  = srcs/
OBJD  = objs/
FILES = ft_sprintf.c ft_conv.c ft_itoa.c ft_var.c ft_get.c ft_format.c ft_out.c
OBJS  = $(addprefix $(OBJD),$(FILES:.c=.o))
SRCS  = $(addprefix $(SRCD),$(FILES))
CC    = gcc

all : $(OBJS)

$(OBJD)%.o: $(SRCD)%.c
		$(CC) $(CFLG) -c -o $@ $<
clean:
		rm -f $(OBJS)

fclean: clean

re: fclean all
