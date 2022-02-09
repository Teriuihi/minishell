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

void	child(t_command *command, const int *old_pid, const int *new_pid)
{
	char	*cur_dir;

	if (old_pid)
	{
		dup2(old_pid[0], STDIN_FILENO);
		close(old_pid[0]);
		close(old_pid[1]);
	}
	if (new_pid)
	{
		dup2(new_pid[1], STDOUT_FILENO);
		close(new_pid[1]);
		close(new_pid[0]);
	}
	cur_dir = get_pwd();
	chdir(cur_dir);
	free(cur_dir);
	if (execve(command->command, command->args, NULL) < 0)
	{
		ft_printf("Command not found: %s\n", command->command);
		exit(0);
	}
}

void	parent(pid_t c_pid, const int *old_pid)
{
	int	status;

	if (old_pid)
	{
		close(old_pid[1]);
		close(old_pid[0]);
	}
	waitpid(c_pid, &status, 0);
}

void	eval(t_command_data *command_data)
{
	pid_t		c_pid;
	int			*cur_pid;
	int			*old_pid;
	t_command	*command;
	t_list		*entry;

	entry = *command_data->commands;
	old_pid = NULL;
	cur_pid = NULL;
	while (entry)
	{
		command = entry->content;
		if (cur_pid)
			old_pid = cur_pid;
		if (command->type)
		{
			cur_pid = ft_calloc(2, sizeof(int));
			if (!cur_pid)
				return ;
			pipe(cur_pid);
			command->command = search_in_path(command->command);
			if (command == NULL)
			{
				ft_printf("Command not found\n");
				return ;
			}
			*command->args = command->command;
			c_pid = fork();
			if (c_pid == 0)
				child(command, old_pid, cur_pid);
			else
				parent(c_pid, old_pid);
		}
		else
		{
			cur_pid = NULL;
			command->command = search_in_path(command->command);
			if (command == NULL)
			{
				ft_printf("Command not found\n");
				return ;
			}
			*command->args = command->command;
			c_pid = fork();
			if (c_pid == 0)
				child(command, old_pid, cur_pid);
			else
				parent(c_pid, old_pid);
		}
		entry = entry->next;
	}
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
	char			*cur_dir;

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
		eval(command_data);
		free_input_args(input, args);
		input = readline("some shell>");
	}
	return (0);
}
