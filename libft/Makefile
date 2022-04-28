NAME=libft.a
CC=gcc
CFLAGS=-Wall -Wextra -Werror
RM=rm -f
FILES=ft_bzero.c ft_isdigit.c ft_atoi.c ft_calloc.c ft_contains.c \
ft_iswhite_space.c ft_itoa.c ft_lstadd_back.c ft_lstclear.c ft_lstdelentry.c \
ft_lstdelone.c ft_lstlast.c ft_lstnew.c ft_memcpy.c ft_memset.c \
ft_putchar_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_strdup.c ft_strjoin.c \
ft_strlcpy.c ft_strlen.c ft_strncmp.c ft_strncpy.c \
ft_streq.c ft_lstadd_front.c ft_isalnum.c ft_isalpha.c ft_isdigit.c ft_lstremove_last.c \
ft_strcmp.c ft_split_first.c
PRINTF=ft_printf.c hex.c hex_util.c printing.c
FILES_OBJ=$(FILES:%.c=%.o) $(PRINTF:%.c=ft_printf/%.o)

all: $(NAME)

$(NAME): $(FILES_OBJ)
	ar -rcs $(NAME) $(FILES_OBJ)

clean:
	$(RM) $(FILES_OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)

.PHONY: make all clean fclean re
