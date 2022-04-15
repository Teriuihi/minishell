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
#include "precheck.h"
#include "control_child_and_pipes.h"

/**
 * Code to be ran by parent after fork
 * 	closes unused PIDs
 * 	waits for child to finish
 *
 * @param	command		command struct
 * @param	minishell	minishell struct
 * @param	old_pid		the previous piped pid
 */
void	execute_with_access_check(t_command *command, t_minishell *minishell,
			char *old_pid)
{
	if (access(command->command, (F_OK)) == 0)
	{
		if (access(command->command, X_OK) == -1) //close(old_pid[0]); do we need this? inside?
		{
			if (ft_printf(2, "%s%s%s\n", "some shell: ", command->command, ": Permission denied") == -1)
			{
				exit(1);
			}
			else
			{
				exit(126);
			}
		}
		else if (execve(command->command, command->args,
						get_envp(minishell->env)) < 0) //close(old_pid[0]); //do we need this inside?
		{
			exit(126);
		}
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
	{
		ft_printf(1, "some error while trying to close the pipes in parent\n");
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
	control_pipes(cmd_data, (int *)old_pid, (int *)cur_pid, minishell);
	
	//if no slash before then 127
	if (cmd_data->executable_found == false)
	{
		ft_printf(1, "it was false cuz init child in child execute non builtin\n");
		exit(1);
	}
	if (control_pipes(cmd_data, old_pid, cur_pid, minishell) == false)
	{
		ft_printf(1, "it was false cuz control pipes in child execute non builtin\n");
		exit(1);
	}
	if (cmd_data->executable_found == false)
	{
		ft_printf(2, "some shell: %s: No such file or directory\n", command->command);
		exit(127);
	}
	else
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
 * Execute a build in command
 * 	should only be called from child process
 *
 * @param	command		Current command
 * @param	old_pid		PIDs used by the previous process
 * @param	cur_pid		PIDs used by the current process
 * @param	minishell	Data for minishell
 */
t_bool	should_be_child(t_command *command)
{
	if (ft_streq(command->command, "export") == 1 && command->args_len == 1)
		return (true);
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

	command = cmd_data->command;
	if (init_child(old_pid, cur_pid, cmd_data->output.type, minishell) == false)
	{
		ft_printf(1, "it was false cuz init child in child_execute_built_in\n");
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
	if (ft_streq(command->command, "exit"))
	{
		if (cmd_data->input.type == NONE) // && cmd_data->output.type == NONE)
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
	if (should_be_child(command) == false)
	{
		if (execute_non_forked_builtin(command, minishell) == false && g_signal.print_basic_error == true)
			ft_printf(2, "command not found: %s\n", command->command);
		if (g_signal.print_basic_error == true)
			g_signal.print_basic_error = false;
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
