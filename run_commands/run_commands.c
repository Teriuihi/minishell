/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   run_commands.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/14 18:54:39 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/14 18:54:39 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "run_commands.h"

static t_bool	heredoc_no_output(t_cmd_data *cmd_data, int old_pid[2])
{
	char		*input;

	g_signal.heredoc = true;
	if (old_pid[0] != -1)
	{
		if (close(old_pid[0]) == -1)
			return (new_set_exit_status(1, NULL));
		if (close(old_pid[1]) == -1)
			return (new_set_exit_status(1, NULL));
	}
	input = readline("heredoc> ");
	if (signal_check(input) == false)
		return (new_set_exit_status(1, NULL));
	while (input != NULL && !ft_streq(input, cmd_data->input.file))
	{
		free(input);
		input = readline("heredoc> ");
		if (signal_check(input) == false)
			return (new_set_exit_status(1, NULL));
	}
	free(input);
	g_signal.heredoc = false;
	return (true);
}

static t_exit_state	handle_null_command(t_cmd_data *cmd_data, int *old_pid)
{
	if (cmd_data->command->command == NULL)
	{
		if (cmd_data->input.type == DELIMITER_INPUT)
		{
			if (heredoc_no_output(cmd_data, old_pid) == false)
				return (BREAK);
		}
		return (CONTINUE);
	}
	return (NOTHING);
}

static t_exit_state	handle_command(t_cmd_data *cmd_data, int *cur_pid,
					int *old_pid, t_minishell *minishell)
{
	if (cur_pid[0] > -1)
		copy_pid(cur_pid, old_pid);
	if (cmd_data->output.type)
	{
		if (pipe(cur_pid) == -1)
			return (BREAK);
	}
	exec_command(cmd_data, old_pid, cur_pid, minishell);
	return (CONTINUE);
}

t_exit_state	run_commands_loop_function(t_list **entry, int cur_pid[2],
					int old_pid[2], t_minishell *minishell)
{
	t_cmd_data		*cmd_data;
	t_exit_state	exit_state;

	cmd_data = (t_cmd_data *)(*entry)->content;
	if (cmd_data->has_error == true)
	{
		new_set_exit_status(1, NULL);
		(*entry) = (*entry)->next;
		return (CONTINUE);
	}
	g_signal.cur_cmd = cmd_data;
	exit_state = handle_null_command(cmd_data, old_pid);
	if (exit_state == RET)
		return (RET);
	if (exit_state == CONTINUE)
	{
		(*entry) = (*entry)->next;
		return (CONTINUE);
	}
	exit_state = handle_command(cmd_data, cur_pid, old_pid, minishell);
	if (exit_state == BREAK)
		return (BREAK);
	(*entry) = (*entry)->next;
	return (CONTINUE);
}

/**
 * Run all commands in given list
 *
 * @param	head		Start of list of commands
 * @param	minishell	Data for minishell
 */
void	run_commands(t_list **head, t_minishell *minishell)
{
	int				cur_pid[2];
	int				old_pid[2];
	t_list			*entry;
	t_exit_state	exit_state;

	init_run_commands(&entry, head, cur_pid, old_pid);
	g_signal.stop_curr_execution = false;
	while (entry && g_signal.stop_curr_execution == false)
	{
		exit_state = run_commands_loop_function(&entry, cur_pid,
				old_pid, minishell);
		if (exit_state == RET)
			return ;
		if (exit_state == BREAK)
			break ;
	}
}
