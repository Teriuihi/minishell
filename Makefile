NAME=minishell
CC=gcc
CFLAGS=-Wall -Wextra -Werror
RM=rm -f
SRC_DIR=functions

FUNCTIONS=args.c exetuce_builtin.c get_path.c main.c search_in_path.c cd_cmd.c ft_echo.c is_builtint.c pwd.c
FILES=$(wildcard $(SRC_DIR)/*.c)
FILES_OBJ=$(FILES:%.c=%.o)

all: $(NAME)

$(NAME): $(FILES_OBJ)
	$(MAKE) -C libft/
	$(CC) $(CFLAGS) -o $(NAME) $(FILES_OBJ) libft/libft.a -lreadline

clean:
	$(RM) $(FILES_OBJ)
	$(MAKE) -C libft/ clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C libft/ fclean

re: fclean $(NAME)

.PHONY: make all clean fclean re
