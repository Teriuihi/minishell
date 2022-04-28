NAME=minishell
CC=gcc
CFLAGS=-Wall -Wextra -Werror
RM=rm -f

FILES=builtins/ft_cd.c builtins/ft_echo.c builtins/builtin.c \
create_commands/append_arguments.c create_commands/create_command.c \
create_commands/create_new_command.c create_commands/get_commands.c \
create_commands/parse_command.c create_commands/update_pipe.c \
run_commands/verify_key.c create_commands/util.c functions/main.c \
functions/search_in_path.c functions/signals.c functions/util.c \
hashtable/destroy.c hashtable/hash_code.c hashtable/hash_table.c \
hashtable/manipulate_env.c parser/add_command.c parser/append_content.c \
parser/parse.c parser/parse_into_data.c parser/parse_quotation.c \
parser/parse_variable.c parser/store_args.c parser/util.c \
pipes/control_child_and_pipes.c pipes/pipe.c pipes/precheck.c \
pipes/redirects.c run_commands/exetuce_builtin.c \
run_commands/run_program.c string/ft_string.c \
string/ft_string_append.c string/ft_string_util.c \
functions/free_functions.c functions/search_in_folder.c \
hashtable/export.c hashtable/get_envp.c functions/set_exit_status.c \
builtins/ft_pwd.c run_commands/split_functions.c pipes/pipe_utils.c \
hashtable/hash_utils.c run_commands/run_program_utils.c \
run_commands/run_commands.c pipes/redirects2.c \
create_commands/update_pipes_util.c
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
