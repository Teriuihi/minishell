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
		return (set_exit_status(minishell, 1, NULL, false));
	if (chdir(cur_dir) == -1)
		return (set_exit_status(minishell, 1, NULL, false));
	else
		return (set_exit_status(minishell, 0, NULL, false));
}

t_bool	exec_dup_close_sequence(int *pid_to_dup, int fd, int *pid1_to_close,
									int *pid2_to_close)
{
	if (*pid_to_dup != -1)
	{
		if (dup2(pid_to_dup[0], fd) == -1)
		{
			ft_printf(1, "exec dup close problem\n");
			return (false);
		}
	}
	return (close_pipes(pid1_to_close, pid2_to_close));
}
