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
#include <readline/readline.h>
#include <unistd.h>
#include <fcntl.h>
#include "../buildins/buildins.h"
#include "../run_commands/run_commands.h"
#include <sys/wait.h>

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

	if (old_pid[0] > -1)
	{
		dup2(old_pid[0], STDIN_FILENO);
		close(old_pid[0]);
		close(old_pid[1]);
	}
	if (cur_pid[0] > -1)
	{
		if (type != DELIMITER_INPUT)
		{
			dup2(cur_pid[1], STDOUT_FILENO);
			close(cur_pid[1]);
		}
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
void	read_input_write(t_cmd_data *cmd_data, int old_pid[2], int cur_pid[2],
						t_minishell *minishell)
{
	char		*input;

	(void)minishell;
	if (old_pid[0])
	{
		dup2(old_pid[0], STDIN_FILENO);
		close(old_pid[0]);
		close(old_pid[1]);
	}
	input = readline("heredoc> ");
	signal_check(input);
	while (input != NULL && !ft_streq(input, cmd_data->input.file))
	{
		ft_putstr_fd(ft_strjoin(input, "\n"), cur_pid[1]);
		input = readline("heredoc> ");
		signal_check(input);
	}
	close(cur_pid[1]);
}

/**
 * Redirects the output to a location pointed by cmd_data->output.file
 *
 * @param	cmd_data		Current command and it's attributes
 * @param	minishell		Data for minishell
 */
void	redirect_output(t_cmd_data *cmd_data, t_minishell *minishell)
{
	char	*path;
	int		fd;

	path = get_pwd(minishell);
	if (path == NULL)
		err_exit("out of memory", 0);
	chdir(path);
	fd = open(cmd_data->output.file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
		err_exit("no such file or directory", 0);
	dup2(fd, STDOUT_FILENO);
}

void	append_output(t_cmd_data *cmd_data, t_minishell *minishell)
{
	char	*path;
	int		fd;

	path = get_pwd(minishell);
	if (path == NULL)
		err_exit("out of memory", 0);
	chdir(path);
	fd = open(cmd_data->output.file, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (fd < 0)
		err_exit("no such file or directory", 0);
	dup2(fd, STDOUT_FILENO);
}

/**
 * Redirect the contents of a file to a pipe
 *
 * @param	command		Current command
 * @param	minishell	Data for minishell
 */
void	redirect_file(t_cmd_data *cmd_data, int *old_pid, int *cur_pid,
						t_minishell *minishell)
{
	char	buffer[1000];
	char	*path;
	int		fd;
	int		len;

	path = get_pwd(minishell);
	if (path == NULL)
		err_exit("out of memory", 0);
	chdir(path);
	if (old_pid[0] > -1)
	{
		close(old_pid[0]);
		close(old_pid[1]);
	}
	pipe(old_pid);
	fd = open(cmd_data->input.file, O_RDONLY);
	if (fd < 0)
		err_exit("no such file or directory", 0);
	len = 1000;
	while (len == 1000)
	{
		ft_bzero(buffer, sizeof(char) * 1000);
		len = read(fd, buffer, 1000);
		write(old_pid[1], buffer, len);
	}
	close(old_pid[1]);
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

	command = cmd_data->command;
	init_child(old_pid, cur_pid, cmd_data->output.type, minishell);
	if (execute_builtin(command, minishell) == false)
	{
		ft_printf("Unable to execute command: %s\n", command->command);
		exit(minishell->exit_status);
	}
	exit(0);
}

void	check_input_pipes(t_cmd_data *cmd_data, int *old_pid, int *cur_pid,
							t_minishell *minishell)
{
	if (cmd_data->input.type == REDIRECT_INPUT)
		redirect_file(cmd_data, old_pid, cur_pid, minishell);
	else if (cmd_data->input.type == DELIMITER_INPUT)
		read_input_write(cmd_data, old_pid, cur_pid, minishell);
}

void	close_pipes(int *pid1, int *pid2)
{
	if (pid1 != NULL)
		close(*pid1);
	if (pid2 != NULL)
		close(*pid2);
}

void	control_pipes(t_cmd_data *cmd_data, int *old_pid, int *cur_pid,
						t_minishell *minishell)
{
	char	*cur_dir;

	if (cmd_data->input.type == OUTPUT_TO_COMMAND)
	{
		dup2(cur_pid[0], STDIN_FILENO);
		close_pipes(&cur_pid[0], &cur_pid[1]);
	}
	if (cmd_data->input.type == DELIMITER_INPUT)
	{
		dup2(cur_pid[0], STDIN_FILENO);
		close_pipes(&cur_pid[0], &cur_pid[1]);
	}
	if (cmd_data->input.type == REDIRECT_INPUT)
	{
		dup2(old_pid[0], STDIN_FILENO);
		close_pipes(NULL, &old_pid[0]);
	}
	if (cmd_data->output.type == OUTPUT_TO_COMMAND)
	{
		if (old_pid[0] > -1)
		{
			dup2(old_pid[0], STDIN_FILENO);
			close_pipes(&old_pid[0], &old_pid[1]);
		}
		if (cur_pid[0] > -1)
		{
			dup2(cur_pid[1], STDOUT_FILENO);
			close_pipes(&cur_pid[0], &cur_pid[1]);
		}
	}
	if (cmd_data->output.type == REDIRECT_OUTPUT)
	{
		redirect_output(cmd_data, minishell);
	}
	if (cmd_data->output.type == APPEND_OUTPUT)
	{
		append_output(cmd_data, minishell);
	}
	cur_dir = get_pwd(minishell);
	chdir(cur_dir);
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
void	child_execute_non_builtin(t_cmd_data *cmd_data, const int *old_pid,
								const int *cur_pid, t_minishell *minishell)
{
	t_command	*command;

	init_child(old_pid, cur_pid, cmd_data->output.type, minishell);
	command = cmd_data->command;
	control_pipes(cmd_data, (int *)old_pid, (int *)cur_pid, minishell);
	if (cmd_data->executable_found == false)
	{
		ft_printf("command not found\n");
		exit(127);
	}
	if (execve(command->command, command->args,
			get_envp(minishell->env)) < 0)
	{
		ft_printf("%s: command not found\n", command->command);
		close(old_pid[0]);
		exit(126);
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
void	parent(pid_t c_pid, const int *old_pid, t_minishell *minishell)
{
	int	status;

	if (old_pid[0] > -1)
	{
		close_pipes((int *)old_pid, (int *)(old_pid + 1));
	}
	waitpid(c_pid, &status, 0);
	if (WIFEXITED(status))
	{
		if (g_signal.sigint == 1)
		{
			minishell->exit_status = 128 + 2;
		}
		else
		{
			minishell->exit_status = WEXITSTATUS(status);
		}
	}
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
	if (env_variable_found(command->command) == true)
		return (false);
	if (ft_streq(command->command, "cd"))
		return (false);
	if (ft_streq(command->command, "unset"))
		return (false);
	if (ft_streq(command->command, "env"))
		return (false);
	return (true);
}

static t_bool	assign_path_to_command(char *executable, t_bool should_path_extend, t_command *command)
{
	if (should_path_extend == true)
	{
		free(*command->args);
		*command->args = ft_strdup(command->command);
		command->command = ft_strdup(executable);
		free(executable);
		return (true);
	}
	else
	{
		free(*command->args);
		*command->args = ft_strdup(command->command);
		return (true);
	}
}

static t_bool	search_executable(t_cmd_data *cmd_data,
				t_minishell *minishell)
{
	t_command	*command;
	char		*executable;

	command = cmd_data->command;
	executable = NULL;
	if (command->command != NULL)
	{
		if (command->command[0] == '/')
		{
			return (assign_path_to_command(executable, false, command));
		}
	}
	executable = search_folder(command->command, minishell);
	if (executable != NULL)
	{
		return (assign_path_to_command(executable, true, command));
	}
	executable = search_in_path(command->command);
	if (executable != NULL)
	{
		return (assign_path_to_command(executable, true, command));
	}
	else
	{
		ft_printf("command not found: %s\n", *command->args);
		return (false);
	}
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

	command = cmd_data->command;
	if (ft_streq(command->command, "exit"))
	{
		signal_check(NULL);
		return ;
	}
	check_input_pipes(cmd_data, old_pid, cur_pid, minishell);
	if (is_built_in == false)
		cmd_data->executable_found = search_executable(cmd_data, minishell);
	if (should_be_child(command) == false)
	{
		if (execute_non_forked_builtin(command, minishell) == false)
			ft_printf("command not found: %s\n", command->command);
		return ;
	}
	ft_printf("old: %i - %i new: %i - %i\n", old_pid[0], old_pid[1], cur_pid[0], cur_pid[1]);
	g_signal.pid = fork();
	if (g_signal.pid == 0)
	{
		if (is_built_in == true)
			child_execute_built_in(cmd_data, old_pid, cur_pid, minishell);
		else
			child_execute_non_builtin(cmd_data, old_pid, cur_pid, minishell);
	}
	else
		parent(g_signal.pid, old_pid, minishell);
}
