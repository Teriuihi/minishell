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
#include "precheck.h"
#include "control_child_and_pipes.h"
#include "pipe_utils.h"

/**
 * Code to be ran by parent after fork
 * 	closes unused PIDs
 * 	waits for child to finish
 *
 * @param	command		command struct
 * @param	minishell	minishell struct
 * @param	old_pid		the previous piped pid
 */
void	execute_with_access_check(t_command *command, t_minishell *minishell)
{
	char	**env;

	if (access(command->command, (F_OK)) == 0)
	{
		if (access(command->command, X_OK) == -1)
		{
			if (ft_printf(2, "%s%s%s\n", "some shell: ", command->command,
					": Permission denied") == -1)
				exit(1);
			else
				exit(126);
		}
		env = get_envp(minishell->env, 0, 0);
		if (env == NULL)
			exit(1);
		else if (execve(command->command, command->args, env) < 0)
		{
			check_is_dir(command);
			exit(126);
		}
		exit(g_signal.exit_status);
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
void	parent(pid_t c_pid, int *old_pid)
{
	int	status;

	if (close_pipes(old_pid, (old_pid + 1)) == false)
	{
		g_signal.exit_status = 1;
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
			g_signal.stop_curr_execution = g_signal.exit_status == 127;
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
	if (cmd_data->has_error == true)
	{
		new_set_exit_status(1, NULL);
		return ;
	}
	if (cmd_data->executable_found == false)
	{
		ft_printf(2, "some shell: %s: command not found\n",
			command->command);
		exit(127);
	}
	if (init_child(old_pid, cur_pid, cmd_data->output.type, minishell) == false)
	{
		exit(1);
	}
	if (control_pipes(cmd_data, old_pid, cur_pid, minishell) == false)
	{
		exit(1);
	}
	execute_with_access_check(command, minishell);
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
	if (control_pipes(cmd_data, old_pid, cur_pid, minishell) == false)
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
			t_minishell *minishell)
{
	if (pre_fork_check(cmd_data, old_pid, minishell) == false)
	{
		return ;
	}
	g_signal.pid = fork();
	if (g_signal.pid == 0)
	{
		if (is_builtin(cmd_data->command) == true)
			child_execute_built_in(cmd_data, old_pid, cur_pid, minishell);
		else
			child_execute_non_builtin(cmd_data, old_pid, cur_pid, minishell);
	}
	else if (g_signal.pid == -1)
		exit(1);
	else
		parent(g_signal.pid, old_pid);
}
