NAME=minishell
CC=gcc -g
CFLAGS=
RM=rm -f

FILES=buildins/cd_cmd.c buildins/ft_echo.c buildins/get_path.c buildins/is_builtint.c buildins/pwd.c create_commands/append_arguments.c create_commands/create_command.c create_commands/create_new_command.c create_commands/get_commands.c create_commands/parse_command.c create_commands/util.c functions/main.c functions/print_splitted.c functions/search_in_path.c functions/signals.c functions/util.c hashtable/destroy.c hashtable/hash_code.c hashtable/hash_table.c hashtable/manipulate_env.c libft/ft_atoi.c libft/ft_bzero.c libft/ft_calloc.c libft/ft_contains.c libft/ft_digit_count.c libft/ft_ends_with.c libft/ft_isalnum.c libft/ft_isalpha.c libft/ft_isdigit.c libft/ft_iswhite_space.c libft/ft_itoa.c libft/ft_lstadd_back.c libft/ft_lstadd_front.c libft/ft_lstclear.c libft/ft_lstdelentry.c libft/ft_lstdelone.c libft/ft_lstlast.c libft/ft_lstnew.c libft/ft_lstremove_last.c libft/ft_lstsize.c libft/ft_memcpy.c libft/ft_memset.c libft/ft_printf/ft_printf.c libft/ft_printf/hex.c libft/ft_printf/hex_util.c libft/ft_printf/printing.c libft/ft_putchar_fd.c libft/ft_putnbr_fd.c libft/ft_putstr_fd.c libft/ft_split.c libft/ft_strdup.c libft/ft_streq.c libft/ft_strjoin.c libft/ft_strlcpy.c libft/ft_strlen.c libft/ft_strncmp.c libft/ft_strncpy.c libft/ft_strrchr.c libft/ft_strtrim.c libft/get_next_line/get_next_line.c libft/get_next_line/get_next_line_utils.c parser/add_command.c parser/append_content.c parser/parse.c parser/parse_quotation.c parser/parse_variable.c pipes/pipe.c run_commands/exetuce_builtin.c run_commands/run_program.c string/ft_string.c string/ft_string_append.c string/ft_string_operations.c string/ft_string_util.c parser/util.c parser/store_args.c parser/parse_into_data.c create_commands/update_pipe.c create_commands/command_with_pipe_start.c
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
