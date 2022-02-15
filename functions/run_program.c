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
	t_command	*command;
	t_list		*entry;

	entry = *head;
	cur_pid[0] = 0;
	old_pid[0] = 0;
	while (entry)
	{
		command = entry->content;
		if (cur_pid[0])
			copy_pid(cur_pid, old_pid);
		if (command->type)
			pipe(cur_pid);
		exec_command(command, old_pid, cur_pid,
			is_builtin(command), minishell);
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
	while (*input && ft_iswhite_space(*input))
		input++;
	return ((*input) != '\0');
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
	char		**args;
	t_list		**head;

	input = readline("some shell>");
	while (input)
	{
		args = NULL;
		if (input != NULL && should_use(input))
		{
			add_history(input);
			args = get_args(input);
			head = find_commands(args);
			if (head == NULL)
			{
				ft_printf("Error\n");
				exit(0);
			}
			run_commands(head, minishell);
			//free_commands(head); causing double free?
			free_char_arr(args);
		}
		free(input);
		input = readline("some shell>");
	}
}
