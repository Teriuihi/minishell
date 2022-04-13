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
#include <readline/history.h>
#include <readline/readline.h>
#include "run_commands.h"
#include "../buildins/buildins.h"
#include "../parser/parser.h"
#include <errno.h>

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

void	tmp_print_command(t_cmd_data *cmd_data)
{
	static int	count;
	int			i;

	i = 0;
	ft_printf(2, "\033[1;32mCommand %i: \033[1;31m%s\n", count, cmd_data->command->command);
	ft_printf(2, "\033[1;32mArgs: \033[1;31m%i\n", cmd_data->command->args_len);
	while (i < cmd_data->command->args_len)
		ft_printf(2, "  [%s]\n", cmd_data->command->args[i++]);
	ft_printf(2, "\033[1;32mInput type: \033[1;31m%i\033[0m - \033[1;32mInput file: \033[1;31m%s\n",
		cmd_data->input.type, cmd_data->input.file);
	ft_printf(2, "\033[1;32mOutput type: \033[1;31m%i\033[0m - \033[1;32mOutput file: \033[1;31m%s\n",
		cmd_data->output.type, cmd_data->output.file);
	ft_printf(2, "\033[0m\n");
	count++;
}

t_bool	heredoc_no_output(t_cmd_data *cmd_data, int old_pid[2], t_minishell *minishell)
{
	char		*input;

	g_signal.heredoc = true;
	(void)minishell;
	if (old_pid[0] != -1)
	{
		if (close(old_pid[0]) == -1)
			return (set_exit_status(minishell, 1, NULL));
		if (close(old_pid[1]) == -1)
			return (set_exit_status(minishell, 1, NULL));
	}
	input = readline("heredoc> ");
	if (signal_check(input, minishell) == false)
		return (set_exit_status(minishell, 1, NULL));
	while (input != NULL && !ft_streq(input, cmd_data->input.file))
	{
		free(input);
		input = readline("heredoc> ");
		if (signal_check(input, minishell) == false)
			return (set_exit_status(minishell, 1, NULL));
	}
	free(input);
	g_signal.heredoc = false;
	return (true);
}

/**
 * Run all commands in given list
 *
 * @param	head		Start of list of commands
 * @param	minishell	Data for minishell
 */
void	run_commands(t_list **head, t_minishell *minishell)
{
	int			cur_pid[2];
	int			old_pid[2];
	t_cmd_data	*cmd_data;
	t_list		*entry;

	entry = *head;
	cur_pid[0] = -1;
	old_pid[0] = -1;
	cur_pid[1] = -1;
	old_pid[1] = -1;
	while (entry)
	{
		cmd_data = (t_cmd_data *)entry->content;
		//tmp_print_command(cmd_data);
		if (cmd_data->command->command == NULL) //maybe separate checking condition?
		{
			if (cmd_data->input.type == DELIMITER_INPUT)
			{
				if (heredoc_no_output(cmd_data, old_pid, minishell) == false)
					break ;
			}
			entry = entry->next;
			continue ;
		}
		if (cur_pid[0] > -1)
			copy_pid(cur_pid, old_pid);
		if (cmd_data->output.type)
		{
			if (pipe(cur_pid) == -1) //should
				break ;
		}
		exec_command(cmd_data, old_pid, cur_pid,
			is_builtin(cmd_data->command), minishell);
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

	if (g_signal.sigquit == 1 && g_signal.exit_status != 0) //cuz it was registered in this process
	{
		if (g_signal.exit_status == 131)
			ft_printf(1, "Quit: 3\n");
		g_signal.sigquit = 0;
	}
	input = readline("some shell>");
	return (input);
}

/**
 * Starts (minishell) program loop,
 * 	reads from command line to receive commands
 *
 * @param	minishell	Data for minishell
 */
void	start_program_loop(t_minishell *minishell)
{
	char		*input;
	t_list		**head;
	t_list		**parse_results;

	while (g_signal.sigint != 1 && g_signal.veof != 1) //input
	{
		g_signal.command = true;
		input = prompt();
		if (signal_check(input, minishell) == false)
			break ; //reset stuff?
		if (should_use(input) == true)
		{
			add_history(input);
			parse_results = parse(input, minishell); //TODO free
			if (parse_results == NULL)
			{
				ft_printf(2, "some shell: Out of memory.");
				exit(1);
			}
			chdir(minishell->cur_wd);
			head = find_commands(parse_results, minishell); //TODO free
			if (head == NULL)
			{
				g_signal.command = false;
				if (signal_check(NULL, minishell) == false)
					break ;
				else
					return ; //y tho
			}
			g_signal.command = false;
			run_commands(head, minishell);
			free_commands(head);
		}
	}
	g_signal.command = false;
	if (input != NULL)
		free(input);
}