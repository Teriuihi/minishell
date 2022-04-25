/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   control_child_and_pipes.c                         :+:    :+:             */
/*                                                     +:+                    */
/*   By: bmajor <bmajor@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:40:25 by bmajor        #+#    #+#                 */
/*   Updated: 2022/01/26 14:40:25 by bmajor        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "control_child_and_pipes.h"

static t_bool	input_pipes(t_cmd_data *cmd_data, int *old_pid, int *cur_pid)
{
	if (cmd_data->input.type == OUTPUT_TO_COMMAND)
	{
		if (exec_dup_close(&cur_pid[0], &cur_pid[0], &cur_pid[1]) == false)
		{
			return (false);
		}
	}
	if (cmd_data->input.type == DELIMITER_INPUT)
	{
		if (exec_dup_close(&cur_pid[0], &cur_pid[0], &cur_pid[1]) == false)
		{
			return (false);
		}
	}
	if (cmd_data->input.type == REDIRECT_INPUT)
	{
		if (exec_dup_close(&old_pid[0], &old_pid[0], NULL) == false)
		{
			return (false);
		}
	}
	return (true);
}

static t_bool	output_pipes(t_cmd_data *cmd_data, int *old_pid, int *cur_pid,
				t_minishell *minishell)
{
	if (cmd_data->output.type == OUTPUT_TO_COMMAND)
	{
		if (old_pid[0] != -1)
		{
			if (exec_dup_close(&old_pid[0], &old_pid[0], &old_pid[1]) == false)
			{
				return (false);
			}
		}
		if (cur_pid[0] != -1)
		{
			if (exec_dup_close(&cur_pid[1], &cur_pid[0], &cur_pid[1]) == false)
			{
				return (false);
			}
		}
	}
	if (cmd_data->output.type == REDIRECT_OUTPUT)
		return (redirect_output(cmd_data, minishell));
	if (cmd_data->output.type == APPEND_OUTPUT)
		return (append_output(cmd_data, minishell));
	return (true);
}

t_bool	control_pipes(t_cmd_data *cmd_data, int *old_pid, int *cur_pid,
						t_minishell *minishell)
{
	if (input_pipes(cmd_data, old_pid, cur_pid) == false)
		return (false);
	if (output_pipes(cmd_data, old_pid, cur_pid, minishell) == false)
		return (false);
	return (enter_curdir(minishell));
}

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
	if (old_pid[0] != -1)
	{
		if (dup2(old_pid[0], STDIN_FILENO) == -1)
			g_signal.exit_status = 1;
		close_pipes(&old_pid[0], &old_pid[1]);
	}
	if (cur_pid[0] != -1)
	{
		if (type != DELIMITER_INPUT)
		{
			if (cur_pid[1] != -1)
			{
				if (dup2(cur_pid[1], STDOUT_FILENO) == -1)
					g_signal.exit_status = 1;
			}
			close_pipes(&cur_pid[1], NULL);
		}
		close_pipes(&cur_pid[0], NULL);
	}
	return (enter_curdir(minishell));
}

t_bool	exec_dup_close(int *pid_to_dup, int *pid1_to_close,
									int *pid2_to_close)
{
	if (*pid_to_dup != -1)
	{
		if (dup2(pid_to_dup[0], STDIN_FILENO) == -1)
		{
			return (false);
		}
	}
	return (close_pipes(pid1_to_close, pid2_to_close));
}
