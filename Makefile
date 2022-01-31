NAME=minishell
CC=gcc
CFLAGS=-Wall -Wextra -Werror
RM=rm -f

FUNCTIONS=args.c cd_cmd.c
FILES=main.c $(addprefix functions/, $(FUNCTIONS))
FILES_OBJ=$(FILES:%.c=%.o)

all: $(NAME)

$(NAME): $(FILES_OBJ)
	$(MAKE) -C libft/
	$(CC) $(CFLAGS) -o $(NAME) $(FILES_OBJ) libft/libft.a -lreadline

clean:
	$(MAKE) -C libft/ clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C libft/ fclean

re: fclean $(NAME)

.PHONY: make all clean fclean re
