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
		if (cur_pid[0])
			copy_pid(cur_pid, old_pid);
		if (cmd_data->output.type)
			pipe(cur_pid); //now this gets two new fds
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
	{
		return (false);
	}
	while (*input && ft_iswhite_space(*input))
		input++;
	return ((*input) != '\0');
}

//static int interrupted = false;
static int interruptible_getc(void)
{	
	int		r;
	char	c;

	if (g_signal.interrupted == true)
		return EOF;
	r = read(0, &c, 1); // read from stdin, will return -1 when interrupted by a signal
	if (r == -1 && errno == EINTR) //then this is sigint (crtl c here)
	{
		if (errno == EINTR && g_signal.sigint != 1) //we have to check which signal was which it interrupted
		{
			g_signal.interrupted = true;
		}
	}
	return (r == 1 ? c : EOF); //if we use signals this we always return EOF
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

	rl_getc_function = interruptible_getc;
	input = "";
	while (input && g_signal.sigint != 1 && g_signal.veof != 1)
	{
		input = readline("some shell>");
		//print_splitted(get_envp(minishell->env));
		signal_check(input);
		if (should_use(input))
		{
			add_history(input);
			parse_results = parse(input, minishell); //TODO free
			if (parse_results == NULL)
			{
				ft_printf("Error\n");
				exit(0);
			}
			head = find_commands(parse_results); //TODO free
			if (head == NULL)
			{
				signal_check(NULL);
				return ;
			}
			run_commands(head, minishell);
			free_commands(head);
		}
	}
	//if (g_signal.veof != 1) //not sure about this yet
	//{
	//	ft_printf("\n");
	//}
	if (input != NULL)
	{
		free(input);
	}
}

//was above the input != NULL
//if (g_signal.veof != 1) //not sure about this yet
	//{
	//	ft_printf("\n");
	//}
/*
if (g_signal.sigint != 1)
{
	//ft_printf("setting sigquit to 1\n"); //delete the prev char apostrophe D
	//int stdout_copy = dup(STDOUT_FILENO);
	//close(STDOUT_FILENO);
	//dup2(stdout_copy, STDOUT_FILENO);
	//close(stdout_copy);
	g_signal.sigquit = 1;
}
*/