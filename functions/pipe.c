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

/**
 * Assign pid's to STDOUT or STDIN as needed
 * Close unused file descriptors for child
 * Reset wd to current working directory
 *
 * @param	old_pid		PIDs used by the previous process
 * @param	cur_pid		PIDs used by the current process
 * @param	type		Type of pipe for this child process
 * @param	minishell	Data for minishell
 */
void	init_child(const int *old_pid, const int *cur_pid, t_pipe_type type,
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
		if (type != DELIMITER_INPUT)
			close(cur_pid[1]);
		close(cur_pid[0]);
	}
	cur_dir = get_pwd(minishell);
	chdir(cur_dir);
}

/**
 * Read input using readline until delimiter is found and write it to pipe
 *
 * @param	command		Current command
 * @param	write_pid	PID to write to
 */
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

/**
 * Redirect the contents of a file to a pipe
 *
 * @param	command		Current command
 * @param	minishell	Data for minishell
 */
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

/**
 * Execute a build in command
 * 	should only be called from child process
 *
 * @param	command		Current command
 * @param	old_pid		PIDs used by the previous process
 * @param	cur_pid		PIDs used by the current process
 * @param	minishell	Data for minishell
 */
void	child_execute_built_in(t_cmd_data *cmd_data, const int *old_pid,
								const int *cur_pid, t_minishell *minishell)
{
	t_command	*command;

	command = &cmd_data->command;
	init_child(old_pid, cur_pid, cmd_data->output.type, minishell);
	if (cmd_data->output.type == REDIRECT_INPUT)
	{
		redirect_file(command, minishell);
		exit(0);
	}
	if (cmd_data->output.type == DELIMITER_INPUT)
	{
		read_input_write(command, cur_pid[1]);
		exit(0);
	}
	if (execute_builtin(command, minishell) == false)
		ft_printf("Unable to execute command: %s\n", command->command);
	exit(0);
}

/**
 * Execute an external command
 * 	should only be called from child process
 *
 * @param	command		Current command
 * @param	old_pid		PIDs used by the previous process
 * @param	cur_pid		PIDs used by the current process
 * @param	minishell	Data for minishell
 */
void	child_execute_external(t_cmd_data *cmd_data, const int *old_pid,
								const int *cur_pid, t_minishell *minishell)
{
	t_command	*command;

	command = &cmd_data->command;
	init_child(old_pid, cur_pid, cmd_data->output.type, minishell);
	if (execve(command->command, command->args,
			NULL) < 0) //here we should pass instead of NULL an array of strings for env variable
	{
		ft_printf("Unable to execute command: %s\n", command->command);
		exit(0);
	}
}

/**
 * Code to be ran by parent after fork
 * 	closes unused PIDs
 * 	waits for child to finish
 *
 * @param	c_pid	PID of fork
 * @param	old_pid	PIDs from previous pipes
 */
void	parent(pid_t c_pid, const int *old_pid)
{
	int	status;

	if (old_pid[0])
	{
		close(old_pid[1]);
		close(old_pid[0]);
	}
	waitpid(c_pid, &status, 0);
}

/**
 * Check if a command should be executed in a child process
 *
 * @param	command	Command to check
 *
 * @return	True if it should be executed in a child process, false if not
 */
t_bool	should_be_child(t_command *command)
{
	if (env_variable_found(command->command) == true) //what happens if its false but because of incorrect input? hello==myvar
		return (false);
	if (ft_streq(command->command, "cd"))
		return (false);
	if (ft_streq(command->command, "unset"))
		return (false);
	return (true);
}

/**
 * Execute a command
 *
 * @param	command		Command to execute
 * @param	old_pid		PIDs used by the previous process
 * @param	cur_pid		PIDs used by the current process
 * @param	is_built_in	If a command is a built in command or not
 * @param	minishell	Data for minishell
 */
void	exec_command(t_cmd_data *cmd_data, int *old_pid, int *cur_pid,
			t_bool is_built_in, t_minishell *minishell)
{
	pid_t		c_pid;
	t_command	*command;

	command = &cmd_data->command;
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
		free(*command->args);
		*command->args = ft_strdup(command->command);
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
			child_execute_built_in(cmd_data, old_pid, cur_pid, minishell);
		else
			child_execute_external(cmd_data, old_pid, cur_pid, minishell);
	}
	else
		parent(c_pid, old_pid);
}
