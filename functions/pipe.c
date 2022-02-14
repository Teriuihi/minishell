/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipe.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/09 18:15:27 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/09 18:15:27 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/functions.h"
#include <readline/history.h>
#include <unistd.h>
#include <fcntl.h>

void	start_child(const int *old_pid, const int *cur_pid, t_pipe_type type,
					t_minishell *minishell)
{
	char	*cur_dir;

	if (old_pid)
	{
		dup2(old_pid[0], STDIN_FILENO);
		close(old_pid[0]);
		close(old_pid[1]);
	}
	if (cur_pid)
	{
		if (type != DELIMITER_INPUT)
			dup2(cur_pid[1], STDOUT_FILENO);
		close(cur_pid[1]);
		close(cur_pid[0]);
	}
	cur_dir = get_pwd(minishell);
	chdir(cur_dir);
}

void	read_input_write(t_command *command, int write_pid)
{
	char	*input;

	if (command->args_len != 2 || command->args[1] == NULL)
		err_exit("parse error", 0);
	input = readline("heredoc> ");
	while (input && !ft_streq(input, command->args[1]))
	{
		ft_putstr_fd(input, write_pid);
		input = readline("heredoc> ");
	}
	close(write_pid);
}

void	redirect_file(t_command *command, t_minishell *minishell)
{
	char	buffer[1000];
	char	*path;
	int		fd;
	int		len;

	path = get_pwd(minishell);
	if (path == NULL)
		err_exit("out of memory", 0);
	chdir(path);
	if (command->args_len != 2 || command->args[1] == NULL)
		err_exit("parse error", 0);
	fd = open(command->args[1], O_RDONLY);
	if (fd <= 2)
		err_exit("no such file or directory", 0);
	len = 1000;
	while (len == 1000)
	{
		ft_bzero(buffer, sizeof(char) * 1000);
		len = read(fd, buffer, 1000);
		write(1, buffer, len);
	}
}

void	child_execute_built_in(t_command *command, const int *old_pid,
								const int *cur_pid, t_minishell *minishell)
{
	int	pid;

	if (command->type == DELIMITER_INPUT)
		pid = dup(cur_pid[1]);
	start_child(old_pid, cur_pid, command->type, minishell);
	if (command->type == REDIRECT_INPUT)
	{
		redirect_file(command, minishell);
		exit(0);
	}
	if (command->type == DELIMITER_INPUT)
	{
		read_input_write(command, pid);
		exit(0);
	}
	if (execute_builtin(command, minishell) == false)
		ft_printf("Unable to execute command: %s\n", command->command);
	exit(0);
}

void	child_execute_external(t_command *command, const int *old_pid,
								const int *cur_pid, t_minishell *minishell)
{
	start_child(old_pid, cur_pid, command->type, minishell);
//	if (execve(command->command, command->args, NULL) < 0)
//		start_child(old_pid, cur_pid, command->type);
	if (execve(command->command, command->args, get_envp(minishell->data->env)) < 0) //here we should pass instead of NULL an array of strings for env variable
	{
		ft_printf("Unable to execute command: %s\n", command->command);
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

t_bool should_be_child(t_command *command)
{
	if (env_variable_found(command->command) == true) //what happens if its false but because of incorrect input? hello==myvar
		return (false);
	if (ft_streq(command->command, "cd"))
		return (false);
	if (ft_streq(command->command, "unset"))
		return (false);
	return (true);
}

void	exec_command(t_command *command, int *old_pid, int *cur_pid,
			t_bool is_built_in, t_minishell *minishell)
{
	pid_t	c_pid;

	if (ft_streq(command->command, "exit"))
		exit (0);
	if (is_built_in == false)
	{
		command->command = search_in_path(command->command); //this is where we could just pull from hashtable
		if (command->command == NULL)
		{
			ft_printf("Command not found: %s\n", *command->args);
			return ;
		}
		*command->args = command->command;
	}
	if (is_built_in && should_be_child(command) == false)
	{
		//only cd, export, unset
		child_execute_built_in_not_child(command, minishell);
		return ;
	}
	c_pid = fork();
	if (c_pid == 0) //only in this case do we pass the child_htable
	{
		if (is_built_in == true)
			child_execute_built_in(command, old_pid, cur_pid, minishell);
		else
			child_execute_external(command, old_pid, cur_pid, minishell);
	}
	else
		parent(c_pid, old_pid);
}
