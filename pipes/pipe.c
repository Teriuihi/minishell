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
#include <errno.h>
#include "redirects.h"

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
t_bool	init_child(int *old_pid, int *cur_pid, t_pipe_type type,
					t_minishell *minishell)
{
	char	*cur_dir;

	if (old_pid[0] != -1)
	{
		if (dup2(old_pid[0], STDIN_FILENO) == -1)
			ft_printf(1, "DUP CHILD\n");
		close_pipes(&old_pid[0], &old_pid[1]);
	}
	if (cur_pid[0] != -1)
	{
		if (type != DELIMITER_INPUT)
		{
			if (cur_pid[1] != -1)
			{
				if (dup2(cur_pid[1], STDOUT_FILENO) == -1)
					ft_printf(STDOUT_FILENO, "DUP child2, %d is ERRNO\n", errno);
			}
			close_pipes(&cur_pid[1], NULL);
		}
		close_pipes(&cur_pid[0], NULL);
	}
	cur_dir = get_pwd(minishell);
	if (!cur_dir)
		return (set_exit_status(minishell, 1, NULL));
	if (chdir(cur_dir) == -1)
		return (set_exit_status(minishell, 1, NULL));
	else
		return (set_exit_status(minishell, 0, NULL));
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
void	child_execute_built_in(t_cmd_data *cmd_data, int *old_pid,
								int *cur_pid, t_minishell *minishell)
{
	t_command	*command;

	command = cmd_data->command;
	if (init_child(old_pid, cur_pid, cmd_data->output.type, minishell) == false)
	{
		exit(1);
	}
	if (execute_builtin(command, minishell) == false)
	{
		ft_printf(2, "Unable to execute command: %s\n", command->command);
		exit(g_signal.exit_status);
	}
	else
	{
		exit(0);
	}
}

t_bool	check_input_redir(t_cmd_data *cmd_data, int *old_pid, int *cur_pid,
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

t_bool	exec_dup_close_sequence(int *pid_to_dup, int fd, int *pid1_to_close,
									int *pid2_to_close)
{
	if (*pid_to_dup != -1)
	{
		if (dup2(pid_to_dup[0], fd) == -1)
		{
			return (false);
		}
	}
	return (close_pipes(pid1_to_close, pid2_to_close));
}

t_bool	control_pipes(t_cmd_data *cmd_data, int *old_pid, int *cur_pid,
						t_minishell *minishell)
{
	char	*cur_dir;
	t_bool	return_val;

	if (cmd_data->input.type == OUTPUT_TO_COMMAND)
	{
		if (exec_dup_close_sequence(&cur_pid[0], STDIN_FILENO, &cur_pid[0], &cur_pid[1]) == false)
			ft_printf(1, "OUTPUTCOMMANDproblem\n");
	}
	if (cmd_data->input.type == DELIMITER_INPUT)
	{
		if (exec_dup_close_sequence(&cur_pid[0], STDIN_FILENO, &cur_pid[0], &cur_pid[1]) == false)
			ft_printf(1, "DELIMETERproblem\n");
	}
	if (cmd_data->input.type == REDIRECT_INPUT)
	{
		if (exec_dup_close_sequence(&old_pid[0], STDIN_FILENO, &old_pid[0], NULL) == false)
			ft_printf(1, "REDIRECTproblem\n");
	}
	if (cmd_data->output.type == OUTPUT_TO_COMMAND)
	{
		if (old_pid[0] > -1)
		{
			if (exec_dup_close_sequence(&old_pid[0], STDIN_FILENO, &old_pid[0], &old_pid[1]) == false)
				ft_printf(1, "REDIRECTproblem\n");
		}
		if (cur_pid[0] > -1)
		{
			if (exec_dup_close_sequence(&cur_pid[1], STDOUT_FILENO, &cur_pid[0], &cur_pid[1]) == false)
				ft_printf(1, "REDIRECTproblem\n");
		}
	}
	if (cmd_data->output.type == REDIRECT_OUTPUT)
		return (redirect_output(cmd_data, minishell));
	if (cmd_data->output.type == APPEND_OUTPUT)
		return (append_output(cmd_data, minishell));
	cur_dir = get_pwd(minishell);
	if (!cur_dir)
	{
		ft_printf(1, "DIR\n");
		return (false);
	}
	if (chdir(cur_dir) == -1)
	{
		ft_printf(1, "curDIR\n");
		return (false);
	}
	return (true);
}

void	execute_with_access_check(t_command *command, t_minishell *minishell,
									char *old_pid)
{
	char	*message;

	if (access(command->command, (F_OK)) == 0)
	{
		if (access(command->command, X_OK) == -1)
		{
			message = ft_strjoin(command->command, ": Permission denied");
			if (!message)
				exit(126);
			message = ft_strjoin("some shell: ", message);
			if (!message)
				exit(126);
			ft_printf(2, "%s\n", message);
			free(message);
			exit(126);
		}
		else if (execve(command->command, command->args,
				get_envp(minishell->env)) < 0)
		{
			close(old_pid[0]);
			exit(126);
		}
	}
}

/**
 * Execute an external command
 * 	should only be called from child process
 *
 * @param	command		Current command
 * @param	old_pid		PIDs used by the previous process
 * @param	cur_pid		PIDs used by the current process
 * @param	minishell	Data foår minishell
 */
void	child_execute_non_builtin(t_cmd_data *cmd_data, int *old_pid,
								int *cur_pid, t_minishell *minishell)
{
	t_command	*command;

	command = cmd_data->command;
	if (init_child(old_pid, cur_pid, cmd_data->output.type, minishell) == false
		|| control_pipes(cmd_data, (int *)old_pid, (int *)cur_pid, minishell) == false)
	{
		ft_printf(1, "it was false cuz prob closed pipes it shouldnt but fine for now\n");
		exit(1);
	}
	if (cmd_data->executable_found == false)
	{
		ft_printf(2, "some shell: %s: No such file or directory\n", command->command);
		exit(127);
	}
	else
	{
		execute_with_access_check(command, minishell, (char *)old_pid);
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
void	parent(pid_t c_pid, int *old_pid, t_minishell *minishell)
{
	int	status;

	if (close_pipes(old_pid, (old_pid + 1)) == false)
		ft_printf(1, "some error while trying to close the pipes in parent\n");
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
	if (env_variable_found(command) == true)
		return (false);
	if (ft_streq(command->command, "cd"))
		return (false);
	if (ft_streq(command->command, "unset"))
		return (false);
	return (true);
}

static t_bool	assign_path_to_command(char *executable, t_bool should_path_extend,
										t_command *command)
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
	executable = search_in_folder(command->command, minishell);
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

t_bool	pre_fork_check(t_cmd_data *cmd_data, int *old_pid, int *cur_pid,
			t_bool is_built_in, t_minishell *minishell)
{
	t_command	*command;
	t_bool		succeeded;

	command = cmd_data->command;
	succeeded = true;
	if (check_input_redir(cmd_data, old_pid, cur_pid, minishell) == false)
	{
		return (false);
	}
	if (ft_streq(command->command, "exit"))
	{
		if (cmd_data->input.type == NONE && cmd_data->output.type == NONE)
			exit(0);
		else
			return (false);
	}
	if (is_built_in == false)
	{
		cmd_data->executable_found = search_executable(cmd_data, minishell);
	}
	if (should_be_child(command) == false)
	{
		succeeded = execute_non_forked_builtin(command, minishell);
		if (succeeded == false && g_signal.print_basic_error == true)
		{
			ft_printf(2, "command not found: %s\n", command->command);
		}
		if (g_signal.print_basic_error == true)
		{
			g_signal.print_basic_error = false;
		}
		return (false);
	}
	return (succeeded);
}

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
	t_command	*command;

	command = cmd_data->command;
	if (pre_fork_check(cmd_data, old_pid, cur_pid, is_built_in, minishell) == false)
	{
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
	else if (g_signal.pid == -1)
		exit(1);
	else
		parent(g_signal.pid, old_pid, minishell);
}
