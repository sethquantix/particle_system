NAME = libft.a
CFLG = -Wextra -Wall -Werror -g
SRCS =  get_next_line.c \
		ft_atoi.c \
		ft_bzero.c \
		ft_del.c \
		ft_deltab.c \
		ft_isalnum.c \
		ft_isalpha.c \
		ft_isascii.c \
		ft_isdigit.c \
		ft_isinside.c \
		ft_islower.c \
		ft_isprint.c \
		ft_isupper.c \
		ft_iswhite.c \
		ft_itoa.c \
		ft_itoa_binary.c \
		ft_lstadd.c \
		ft_lstadd_end.c \
		ft_lstdel.c \
		ft_lstdelone.c \
		ft_lstiter.c \
		ft_lstmap.c \
		ft_lstnew.c \
		ft_lstsize.c \
		ft_memalloc.c \
		ft_memccpy.c \
		ft_memchr.c \
		ft_memcmp.c \
		ft_memcpy.c \
		ft_memdel.c \
		ft_memmove.c \
		ft_memset.c \
		ft_print_bits.c \
		ft_putchar.c \
		ft_putchar_fd.c \
		ft_putendl.c \
		ft_putendl_fd.c \
		ft_putnbr.c \
		ft_putnbr_fd.c \
		ft_putstr.c \
		ft_putstr_fd.c \
		ft_strcat.c \
		ft_strchr.c \
		ft_strclr.c \
		ft_strcmp.c \
		ft_strcpy.c \
		ft_strdel.c \
		ft_strdup.c \
		ft_strequ.c \
		ft_striter.c \
		ft_striteri.c \
		ft_strjoin.c \
		ft_strjoinfree.c \
		ft_strlcat.c \
		ft_strlen.c \
		ft_strmap.c \
		ft_strmapi.c \
		ft_strncat.c \
		ft_strncmp.c \
		ft_strncpy.c \
		ft_strndup.c \
		ft_strnequ.c \
		ft_strnew.c \
		ft_strnstr.c \
		ft_strrchr.c \
		ft_strsplit.c \
		ft_strstr.c \
		ft_strsub.c \
		ft_strtolower.c \
		ft_strtoupper.c \
		ft_strtrim.c \
		ft_tabsize.c \
		ft_tolower.c \
		ft_toupper.c \
		ft_lst_delif.c
OBJS = $(SRCS:.c=.o)
DIRP = printf/objs/
OBJP = ft_conv.o ft_format.o ft_get.o ft_itoa.o ft_printf.o ft_out.o ft_var.o
DIRSP = sprintf/objs/
OBJSP = ft_conv.o ft_format.o ft_get.o ft_itoa.o ft_sprintf.o ft_out.o ft_var.o
CC   = gcc

all : $(NAME)

%.o: %.c
		$(CC) $(CFLG) -c -o $@ $<

$(NAME):	$(OBJS)
		@make -C printf
		@make -C sprintf
		@echo "Linking ...\c"
		@ar rc $(NAME) $(OBJS) $(addprefix $(DIRP),$(OBJP)) $(addprefix $(DIRSP),$(OBJSP))
		@echo "  Done !"
		@echo "Sorting ...\c"
		@ranlib $(NAME)
		@echo "  Done !"

link:
		@ar rc $(NAME) $(OBJS) $(addprefix $(DIRP),$(OBJP)) $(addprefix $(DIRSP),$(OBJSP))

clean:
		rm -f $(OBJS)
		make -C printf clean
		make -C sprintf clean

fclean: clean
		rm -f $(NAME)
		make -C printf fclean
		make -C sprintf fclean

re: fclean all
