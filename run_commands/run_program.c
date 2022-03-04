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
#include "../create_commands/create_commands.h"

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


t_cmd_data	*init_cmd()
{
	t_cmd_data *cmd_data;

	//cmd_data->command->command needs malloc?
	//cmd_data->command->args needs malloc?
	cmd_data = (t_cmd_data *)malloc(sizeof(t_cmd_data));
	if (!cmd_data)
		return (NULL);
	cmd_data->command = (t_command *)malloc(sizeof(t_command));
	if (!cmd_data->command)
		return (NULL);
	cmd_data->command = NULL;
	//cmd_data->input = malloc(sizeof(t_redirect));
	//if (!cmd_data->input)
	//	return (NULL);
	//cmd_data->output = malloc(sizeof(t_redirect));
	//if (!cmd_data->output)
	//	return (NULL);
	cmd_data->input.type = NONE; // did we actually set space for these?
	cmd_data->output.type = NONE;
	return (cmd_data);
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
	t_cmd_data	*cmd_data; //could be maybe simple struct and paass the & of it
	t_list		*entry;

	//cmd data init?
	//cmd_data = init_cmd(); //should we init all the other stuff?
	
	cmd_data = init_cmd();
	entry = *head;
	cur_pid[0] = -1;
	old_pid[0] = -1;
	cur_pid[1] = -1;
	old_pid[1] = -1;
	while (entry)
	{
		//ft_printf("%s is entry content\n", entry->content);
		cmd_data = (t_cmd_data *)entry->content; //cmd_data = entry->content; this was //command = entry->content before
		//maybe use this cmd_data to copy from entry?
		//ft_printf("%d is CMD_DATA INPUT TYPE, %d is CMD_DATA OUTPUT TYPE\n", cmd_data->input.type, cmd_data->output.type);
		//cmd_data->output.type = cmd_data->output.type;
		if (cur_pid[0])
			copy_pid(cur_pid, old_pid);
		if (cmd_data->output.type) //non initialized yet, if it has to put smth out?
		{
			//ft_printf("%s is command, %d is output type\n", cmd_data->command->command, cmd_data->output.type);
			pipe(cur_pid); //now this gets two new fds
		}
		exec_command(cmd_data, old_pid, cur_pid,
			is_builtin(cmd_data), minishell);
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


	
	input = readline("some shell>"); //TODO free
	while (input)
	{
		//print_splitted(get_envp(minishell->env));

		args = NULL;
		if (should_use(input))
		{
			add_history(input);
			args = get_args(input); //TODO free
			head = find_commands(args); //TODO free
			if (head == NULL)
			{
				ft_printf("Error\n");
				exit(0);
			}
			//print_splitted(get_envp(minishell->env));
			//print_splitted(get_envp(minishell->env));
			//ft_printf("\n\n\n\n");
			run_commands(head, minishell);
			free_commands(head);
			free_char_arr(args);
		}
		free(input);
		input = readline("some shell>");
	}
}
