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
/*
t_bool		init_child(const int *old_pid, const int *cur_pid, t_pipe_type type,
					t_minishell *minishell)
{
	char    *cur_dir;

	if (old_pid[0] > -1)
	{
		if (dup2(old_pid[0], STDIN_FILENO) == -1)
		{
			ft_printf(2, "dupfail\n");
			return (set_exit_status(minishell, 1, NULL));
		}
		if (close(old_pid[0]) == -1)
		{
			ft_printf(2, "closefail1\n");
			return (set_exit_status(minishell, 1, NULL));
		}
		if (close(old_pid[1]) == -1)
		{
			ft_printf(2, "closefail2\n");
			return (set_exit_status(minishell, 1, NULL));
		}
	}
	if (cur_pid[0] > -1)
	{
		if (type != DELIMITER_INPUT)
		{
			if (dup2(cur_pid[1], STDOUT_FILENO) == -1)
			{
				ft_printf(2, "closefail3\n");
				return (set_exit_status(minishell, 1, NULL));
			}
			if (close(cur_pid[1]) == -1)
			{
				return (set_exit_status(minishell, 1, NULL));
				ft_printf(2, "closefail4\n");
			}

		}
		if (close(cur_pid[0]) == -1)
		{
			ft_printf(2, "closefail5\n");
			return (set_exit_status(minishell, 1, NULL));
		}
	}
	cur_dir = get_pwd(minishell);
	if (!cur_dir)
		return (set_exit_status(minishell, 1, NULL));
	if (chdir(cur_dir) == -1)
		return (set_exit_status(minishell, 1, NULL));
	return (true);
}
*/

/**
 * Read input using readline until delimiter is found and write it to pipe
 *
 * @param	command		Current command
 * @param	write_pid	PID to write to
 */
t_bool	read_input_write(t_cmd_data *cmd_data, int old_pid[2], int cur_pid[2],
						t_minishell *minishell)
{
	char		*input;

	g_signal.heredoc = true;
	if (old_pid[0] != -1) //these can be under on function
	{
		if (close(old_pid[0]) == -1)
			return (set_exit_status(minishell, 1, NULL));
		if (close(old_pid[1]) == -1)
			return (set_exit_status(minishell, 1, NULL));
	}
	if (pipe(old_pid) == -1)
		return (set_exit_status(minishell, 1, NULL));
	input = readline("heredoc> ");
	if (signal_check(input, minishell) == false)
		return (set_exit_status(minishell, 1, NULL));
	while (input != NULL && !ft_streq(input, cmd_data->input.file))
	{
		ft_putstr_fd(ft_strjoin(input, "\n"), old_pid[1]);
		//check if ftputstr etc was succesful?
		input = readline("heredoc> ");
		if (signal_check(input, minishell) == false)
			return (set_exit_status(minishell, 1, NULL));
	}
	g_signal.heredoc = false;
	if (close(old_pid[1]) == -1)
		return (set_exit_status(minishell, 1, NULL));
	return (true);
}

/**
 * Redirects the output to a location pointed by cmd_data->output.file
 *
 * @param	cmd_data		Current command and it's attributes
 * @param	minishell		Data for minishell
 */
t_bool	redirect_output(t_cmd_data *cmd_data, t_minishell *minishell)
{
	char	*path;
	int		fd;

	path = get_pwd(minishell);
	if (path == NULL)
		return (set_exit_status(minishell, 1, NULL));
	if (chdir(path) == -1)
		return (set_exit_status(minishell, 1, NULL));
	fd = open(cmd_data->output.file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
	{
		char *message = ft_strdup("no such file or directory");
		return (set_exit_status(minishell, 1, message));
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (set_exit_status(minishell, 1, NULL));
	return (true);

}

t_bool	append_output(t_cmd_data *cmd_data, t_minishell *minishell)
{
	char	*path;
	int		fd;

	path = get_pwd(minishell);
	if (path == NULL)
		return (set_exit_status(minishell, 1, NULL));
	if (chdir(path) == -1)
		return (set_exit_status(minishell, 1, NULL));
	fd = open(cmd_data->output.file, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (fd < 0)
	{
		char *message = ft_strdup("no such file or directory");
		return (set_exit_status(minishell, 1, message));
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (set_exit_status(minishell, 1, NULL));
	return (true);
}

/**
 * Redirect the contents of a file to a pipe
 *
 * @param	command		Current command
 * @param	minishell	Data for minishell
 */
t_bool	redirect_file(t_cmd_data *cmd_data, int *old_pid, int *cur_pid,
						t_minishell *minishell)
{
	char	buffer[1000];
	char	*path;
	int		fd;
	int		len;

	path = get_pwd(minishell);
	if (path == NULL)
		return (set_exit_status(minishell, 1, NULL));
	if (chdir(path) == -1)
		return (set_exit_status(minishell, 1, NULL));
	if (old_pid[0] > -1)
	{
		if (close(old_pid[0]) == -1)
			return (set_exit_status(minishell, 1, NULL));
		if (close(old_pid[1]) == -1)
			return (set_exit_status(minishell, 1, NULL));
	}
	if (pipe(old_pid) == -1)
		return (set_exit_status(minishell, 1, NULL));
	fd = open(cmd_data->input.file, O_RDONLY);
	if (fd < 0)
	{
		char *message = ft_strjoin("some shell: ", ft_strjoin(cmd_data->input.file, ": No such file or directory\n"));
		return (set_exit_status(minishell, 1, message));
	}
	len = 1000;
	while (len == 1000)
	{
		ft_bzero(buffer, sizeof(char) * 1000);
		len = read(fd, buffer, 1000);
		if (len == -1)
			return (set_exit_status(minishell, 1, NULL));
		if (write(old_pid[1], buffer, len) == -1)
			return (set_exit_status(minishell, 1, NULL));
	}
	if (close(old_pid[1]) == -1)
		return (set_exit_status(minishell, 1, NULL));
	return (true);
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
	if (execute_builtin(command, minishell) == false) //take a look at the src code in exetuce builtin
	{
		ft_printf(2, "Unable to execute command: %s\n", command->command);
		exit(g_signal.exit_status);
	}
	else
		exit(0);
}

t_bool	check_input_pipes(t_cmd_data *cmd_data, int *old_pid, int *cur_pid,
							t_minishell *minishell)
{
	if (cmd_data->input.type == REDIRECT_INPUT)
	{
		return (redirect_file(cmd_data, old_pid, cur_pid, minishell));
	}
	else if (cmd_data->input.type == DELIMITER_INPUT)
	{
		return (read_input_write(cmd_data, old_pid, cur_pid, minishell));
	}
	return (true);
}

void	close_pipes(int *pid1, int *pid2)
{
	if (pid1 != NULL)
		close(*pid1);
	if (pid2 != NULL)
		close(*pid2);
}

t_bool	control_pipes(t_cmd_data *cmd_data, int *old_pid, int *cur_pid,
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
		return (redirect_output(cmd_data, minishell));
	}
	if (cmd_data->output.type == APPEND_OUTPUT)
	{
		return (append_output(cmd_data, minishell));
	}
	cur_dir = get_pwd(minishell);
	if (!cur_dir)
		return (false);
	if (chdir(cur_dir) == -1)
	{
		ft_printf(2, "couldnt enter dir\n");
		return (false);
	}

	return (true);
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

	struct stat	sb;
	t_command	*command;
	t_bool		success;

	init_child(old_pid, cur_pid, cmd_data->output.type, minishell);
	command = cmd_data->command;
	if (control_pipes(cmd_data, (int *)old_pid, (int *)cur_pid, minishell) == false)
	{
		exit(1);
	}
	if (cmd_data->executable_found == false) //at this point it just means what
	{
		ft_printf(2, "some shell: %s: No such file or directory\n", command->command);
		exit(127);
	}
	if (g_signal.minishell_exec_found == 1) //if the executable is minishell
	{
		g_signal.minishell_exec_found = 0;
		char *increased_level = ft_itoa(ft_atoi(getenv("SHLVL"), &success) + 1);
		ft_set_env("SHLVL", increased_level, minishell->env, true);
	}
	if (access(command->command, (X_OK | F_OK)) == 0) 	//check with access(X_OK), if its executable, this has also accesses
	{
		if (execve(command->command, command->args,
			get_envp(minishell->env)) < 0)
		{
			close(old_pid[0]);
			exit(126);
		}
	}
	else
	{
		char *message = ft_strjoin("some shell: ", ft_strjoin(cmd_data->input.file, ": No such file or directory\n"));
		ft_printf(2, "permission denied\n"); //name of the command, without /
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
		if (WIFSIGNALED(status)) 
		{
			g_signal.exit_status = WTERMSIG(status) + 128;
		}
		else
		{
			g_signal.exit_status = WEXITSTATUS(status);
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
	//if (ft_streq(command->command, "export") == 1 && command->args_len == 1)
	//	return (true);
	if (env_variable_found(command) == true)
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
	free(*command->args);
	*command->args = ft_strdup(command->command);
	if (!command->args)
	{
		return (false);
	}
	if (should_path_extend == true)
	{
		command->command = ft_strdup(executable);
		free(executable);
		if (!command->command)
		{
			return (false);
		}
	}
	return (true);

}

static t_bool	search_executable(t_cmd_data *cmd_data,
				t_minishell *minishell)
{
	char		*executable;
	t_command	*command;

	executable = NULL;
	command = cmd_data->command;
	if (command->command != NULL && command->command[0] == '/')
	{
		return (assign_path_to_command(executable, false, command));
	}
	executable = search_folder(command->command, minishell);
	if (executable != NULL)
	{
		return (assign_path_to_command(executable, true, command));
	}
	executable = search_in_path(command->command, minishell);
	if (executable != NULL)
	{
		return (assign_path_to_command(executable, true, command));
	}
	else
	{
		return (false);
	}
}


//check for exit

//check for input redirection

//if builtin false, search for executables

//if it should not be forked


/**
 * Execute a command
 *
 * @param	command		Command to execute
 * @param	old_pid		PIDs used by the previous process
 * @param	cur_pid		PIDs used by the current process
 * @param	is_built_in	If a command is a built in command or not
 * @param	minishells	Data for minishell
 */
void	exec_command(t_cmd_data *cmd_data, int *old_pid, int *cur_pid,
			t_bool is_built_in, t_minishell *minishell)
{
	pid_t		c_pid;
	t_command	*command;

	command = cmd_data->command;
	if (ft_streq(command->command, "exit"))
	{
		if (cmd_data->input.type == NONE && cmd_data->output.type == NONE)
			exit(0);
		else
			return ;
	}
	if (check_input_pipes(cmd_data, old_pid, cur_pid, minishell) == false)
	{
		return ;
	}
	if (is_built_in == false)
	{
		cmd_data->executable_found = search_executable(cmd_data, minishell);
	}

	//this should be one function
	if (should_be_child(command) == false)
	{
		if (execute_non_forked_builtin(command, minishell) == false && g_signal.print_basic_error == true)
		{
			ft_printf(2, "command not found: %s\n", command->command);
		}
		if (g_signal.print_basic_error == true)
		{
			g_signal.print_basic_error = false;
		}
		return ;
	}
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
