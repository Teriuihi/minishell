NAME=minishell
CC=gcc -g
CFLAGS=-Wall -Wextra -Werror
RM=rm -f
SRC_DIR=functions

FUNCTIONS=args.c cd_cmd.c destroy.c exetuce_builtin.c ft_echo.c get_commands.c \
get_path.c get_working_directory.c hash_code.c hash_table.c is_builtint.c main.c \
manipulate_env.c print_splitted.c pwd.c search_in_path.c signals.c util.c
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
