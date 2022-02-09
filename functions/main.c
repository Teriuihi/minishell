/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:22:29 by sappunn       #+#    #+#                 */
/*   Updated: 2022/01/26 14:22:29 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/functions.h"
#include "../headers/structs.h"
#include <readline/history.h>
#include <sys/stat.h>
#include <unistd.h>

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dst_len;
	size_t	i;

	dst_len = ft_strlen(dst) >= dstsize ? dstsize : ft_strlen(dst);
	i = 0;
	if (dstsize == dst_len || dstsize == 0)
		return (dstsize + ft_strlen(src));
	if (ft_strlen(src) >= dstsize - dst_len)
	{
		while (i < dstsize - dst_len - 1)
		{
			dst[dst_len + i] = src[i];
			i++;
		}
	}
	else
	{
		while (src[i] && i < dstsize - dst_len)
		{
			dst[dst_len + i] = src[i];
			i++;
		}
	}
	dst[dst_len + i] = '\0';
	return (dst_len + ft_strlen(src));
}

void	free_input_args(char *input, char **args)
{
	char	**tmp;

	tmp = args;
	free(input);
	while (*args)
	{
		free(*args);
		args++;
	}
	free(tmp);
}

int	*eval(int *read_pid, t_command *command)
{
	pid_t	cpid;
	int		*pid;
	char	*cur_dir;

	pid = ft_calloc(2, sizeof(int));
	if (!pid)
		return (NULL);
	if (is_builtin(command->command) == true)
	{
		cur_dir = get_pwd(); //TODO don't repeat in build in
		chdir(cur_dir);
		free(cur_dir);
		if (command->type == NONE)
			execute_builtin(command, -1);
		else if (command->type == OUTPUT_TO_COMMAND)
		{
			//TODO start listening on another thread
			execute_builtin(command, 0);
		}
		return (0);
	}
	command->command = search_in_path(command->command);
	if (command == NULL)
	{
		ft_printf("Command not found\n");
		return (0);
	}
	*command->args = command->command;
	cpid = fork();
	if (cpid == 0)
	{
		if (read_pid != NULL)
			close(read_pid[1]);
		if (command->type == OUTPUT_TO_COMMAND)
		{
			pipe(pid);
			dup2(pid[1], 1); //write
			dup2(pid[0], 0); //read
		}
		cur_dir = get_pwd(); //TODO don't repeat in build in
		chdir(cur_dir);
		free(cur_dir);
		if (execve(command->command, command->args, NULL) < 0)
		{
			ft_printf("Command not found\n");
			exit(0);
		}
		if (read_pid != NULL)
			close(read_pid[0]);
	}
	else
		wait(NULL); //its not guaranteed that the child process will execute first
	if (command->type == OUTPUT_TO_COMMAND)
		return (pid);
	return (NULL);
}

t_hash_table	*get_hash_table(void)
{
	static t_hash_table	*table;

	if (!table)
	{
		table = duplicates_are_found_in_argv();
		if (table == NULL)
		{
			ft_printf("Error\n");
			exit (0);
		}
	}
	return (table);
}

int	main(void)
{
	t_signal		*signal_struct;
	char			*input;
	char			**args;
	t_command_data	*command_data;
	t_list			*entry;
	t_command		*command;
	int				*pid;
	char			*cur_dir;

	pid = NULL;
	cur_dir = getcwd(NULL, 0);
	if (cur_dir == NULL)
	{
		ft_printf("Error\n");
		return (0);
	}
	set_pwd(cur_dir); //doens't need free

	
	//print_splitted(environ);

//	h_table = get_hash_table();
//	ft_printf("%s\n", ft_get_env_val("PWD", h_table));
//	ft_set_env("TURO", "NEW VAR TURO", h_table);
//	//ft_printf("%s\n", ft_get_env_val("TURO", h_table));
//	//ft_remove_env("TURO", h_table);
//	//ft_printf("%s\n", ft_get_env_val("TURO", h_table));
//	ft_set_env("TERI", "NEW VAR TERI", h_table);
//	ft_printf("%s\n", ft_get_env_val("TERI", h_table));
//	ft_printf("%s\n", ft_get_env_val("TURO", h_table));

	signal_struct = init_signal();
	signal(SIGQUIT, sigquit_handler);
	//FIXME this segfaults???
//	set_pwd(getcwd(NULL, 0)); //TODO check for null check if needed
	input = readline("some shell>");
	while (input)
	{
		add_history(input);
		args = get_args(input);
		if (args == NULL)
		{
			ft_printf("Error\n");
			return (0);
		}
		command_data = find_commands(args);
		if (command_data == NULL)
		{
			ft_printf("Error\n");
			return (0);
		}
		entry = *(command_data->commands);
		while (entry)
		{
			command = entry->content;
			if (command == NULL)
			{
				ft_printf("Error\n");
				return (0);
			}
			pid = eval(pid, command);
			entry = entry->next;
			free(command->args);
			free(command);
		}
		free_input_args(input, args);
		input = readline("some shell>");
	}
	return (0);
}
