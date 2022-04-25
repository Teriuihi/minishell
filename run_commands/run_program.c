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
#include "run_program.h"
#include "run_program_internal.h"
#include "run_program_utils.h"

t_exit_state	handle_input(char *input, t_minishell *minishell)
{
	t_list	**commands;
	t_list	**parse_results;

	add_history(input);
	parse_results = parse(input, minishell);
	if (parse_results == NULL)
		return (CONTINUE);
	chdir(minishell->cur_wd);
	commands = find_commands(parse_results, minishell);
	if (commands == NULL)
	{
		g_signal.command = false;
		signal_check(NULL);
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
	if (signal_check(input) == false)
	{
		free(input);
		return (BREAK);
	}
	if (should_use(input) == true)
		exit_state = handle_input(input, minishell);
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
