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

static t_bool	heredoc_no_output(t_cmd_data *cmd_data, int old_pid[2],
			t_minishell *minishell)
{
	char		*input;

	g_signal.heredoc = true;
	if (old_pid[0] != -1)
	{
		if (close(old_pid[0]) == -1)
			return (set_exit_status(minishell, 1, NULL, false));
		if (close(old_pid[1]) == -1)
			return (set_exit_status(minishell, 1, NULL, false));
	}
	input = readline("heredoc> ");
	if (signal_check(input, minishell) == false)
		return (set_exit_status(minishell, 1, NULL, false));
	while (input != NULL && !ft_streq(input, cmd_data->input.file))
	{
		free(input);
		input = readline("heredoc> ");
		if (signal_check(input, minishell) == false)
			return (set_exit_status(minishell, 1, NULL, false));
	}
	free(input);
	g_signal.heredoc = false;
	return (true);
}

static t_exit_state	handle_null_command(t_cmd_data *cmd_data, int *old_pid,
					t_minishell *minishell)
{
	if (cmd_data->command->command == NULL)
	{
		if (cmd_data->input.type == DELIMITER_INPUT)
		{
			if (heredoc_no_output(cmd_data, old_pid, minishell) == false)
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
	t_cmd_data		*cmd_data;
	t_list			*entry;
	t_exit_state	exit_state;

	init_run_commands(&entry, head, cur_pid, old_pid);
	while (entry)
	{
		cmd_data = (t_cmd_data *)entry->content;
		g_signal.cur_cmd = cmd_data;
		exit_state = handle_null_command(cmd_data, old_pid, minishell);
		if (exit_state == RET)
			return ;
		if (exit_state == CONTINUE)
		{
			entry = entry->next;
			continue ;
		}
		exit_state = handle_command(cmd_data, cur_pid, old_pid, minishell);
		if (exit_state == BREAK)
			break ;
		entry = entry->next;
	}
}
