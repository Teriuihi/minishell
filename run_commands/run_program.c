/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   run_program.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/14 18:54:39 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/14 18:54:39 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/functions.h"
#include <readline/readline.h>
#include "run_commands.h"
#include "../buildins/buildins.h"
#include "../parser/parser.h"
#include "run_commands_internal.h"
#include "../functions/free_functions.h"

/**
 * Copies cur_pid to old_pid
 *
 * @param	cur_pid	Current PIDs
 * @param	old_pid	Old PIDs
 */
void	copy_pid(const int *cur_pid, int *old_pid)
{
	old_pid[0] = cur_pid[0];
	old_pid[1] = cur_pid[1];
}

t_bool	heredoc_no_output(t_cmd_data *cmd_data, int old_pid[2],
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

void	init_run_commands(t_list **entry, t_list **head, int *cur_pid,
			int *old_pid)
{
	*entry = *head;
	cur_pid[0] = -1;
	old_pid[0] = -1;
	cur_pid[1] = -1;
	old_pid[1] = -1;
}

t_exit_state	handle_null_command(t_cmd_data *cmd_data, int *old_pid,
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

t_exit_state	handle_command(t_cmd_data *cmd_data, int *cur_pid, int *old_pid,
					t_minishell *minishell)
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
		exit_state = handle_null_command(cmd_data, old_pid, minishell);
		if (exit_state == RETURN)
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

/**
 * Checks if the input should be used
 * 	if it's only white space it shouldn't
 *
 * @param	input	Input to check
 *
 * @return	true if the input should be used, false if not
 */
t_bool	should_use(char *input)
{
	if (input == NULL)
		return (false);
	while (*input && ft_iswhite_space(*input))
		input++;
	return ((*input) != '\0');
}

char	*prompt(void)
{
	char	*input;

	if (g_signal.sigquit == 1 && g_signal.exit_status != 0)
	{
		if (g_signal.exit_status == 131)
			ft_printf(1, "Quit: 3\n");
		g_signal.sigquit = 0;
	}
	input = readline("some shell>");
	return (input);
}

t_exit_state	handle_input(char *input, t_minishell *minishell)
{
	t_list	**commands;
	t_list	**parse_results;

	add_history(input);
	parse_results = parse(input, minishell);
	free(input);
	if (parse_results == NULL)
		return (CONTINUE);
	chdir(minishell->cur_wd);
	commands = find_commands(parse_results, minishell);
	if (commands == NULL)
	{
		g_signal.command = false;
		signal_check(NULL, minishell);
		free_parse_and_commands(NULL, parse_results);
		return (BREAK);
	}
	g_signal.command = false;
	run_commands(commands, minishell);
	free_parse_and_commands(commands, parse_results);
	return (NOTHING);
}

t_exit_state	program_loop(t_minishell *minishell)
{
	char			*input;
	t_exit_state	exit_state;

	g_signal.command = true;
	input = prompt();
	exit_state = CONTINUE;
	if (signal_check(input, minishell) == false)
	{
		free(input);
		return (BREAK);
	}
	if (should_use(input) == true)
		exit_state = handle_input(input, minishell);
	else
		free(input);
	return (exit_state);
}

/**
 * Starts (minishell) program loop,
 * 	reads from command line to receive commands
 *
 * @param	minishell	Data for minishell
 */
void	start_program_loop(t_minishell *minishell)
{
	t_exit_state	exit_state;

	while (g_signal.sigint != 1 && g_signal.veof != 1)
	{
		exit_state = program_loop(minishell);
		if (exit_state == BREAK)
			break ;
	}
	g_signal.command = false;
}
