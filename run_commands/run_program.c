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
	ft_printf("\033[1;32mCommand %i: \033[1;31m%s\n", count, cmd_data->command->command);
	ft_printf("\033[1;32mArgs: \033[1;31m%i\n", cmd_data->command->args_len);
	while (i < cmd_data->command->args_len)
		ft_printf("  %s\n", cmd_data->command->args[i++]);
	ft_printf("\033[1;32mInput type: \033[1;31m%i\033[0m - \033[1;32mInput file: \033[1;31m%s\n",
		cmd_data->input.type, cmd_data->input.file);
	ft_printf("\033[1;32mOutput type: \033[1;31m%i\033[0m - \033[1;32mOutput file: \033[1;31m%s\n",
		cmd_data->output.type, cmd_data->output.file);
	ft_printf("\033[0m\n");
	count++;
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
		tmp_print_command(cmd_data);
		if (cur_pid[0] > -1)
			copy_pid(cur_pid, old_pid);
		if (cmd_data->output.type)
			pipe(cur_pid);
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
	{
		return EOF;
	}
	r = read(0, &c, 1); // read from stdin, will return -1 when interrupted by a signal
	if (r == -1 && errno == EINTR) //then this is sigint (crtl c here)
	{
		//ft_printf("%d is errno KKKK\n", errno);
		//if (errno == 4 && g_signal.sigint != 1)
		//{
		//	g_signal.sigquit == 1;
		//	ft_printf("%d is sigquit\n", g_signal.sigquit);
		//}
		if (errno == EINTR && g_signal.sigint != 1) //we have to check which signal was which it interrupted
		{
			//ft_printf("sigquit is right place\n");
			//write(1, "right place for sigquit\n", 25);
			//ft_printf("we are interrupted, %d is errno, %d is r, %d sigquit\n", errno, r, g_signal.sigquit);
			g_signal.interrupted = true;
			//return (1);
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
		//while we have the sigquit flag on?
		input = readline("some shell>");
		//print_splitted(get_envp(minishell->env));
		signal_check(input);
		/*
		while (g_signal.sigquit == 1)
		{
			input = readline("some shell>");
			ft_printf("INSIDE \b\b\b\b\b\b\b\b\b\b\b");
			signal_check(input);
			sleep(15);
		}
		*/
		if (should_use(input))
		{
			add_history(input);
			parse_results = parse(input, minishell); //TODO free
			if (parse_results == NULL)
			{
				ft_printf("Error\n");
				exit(1);
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