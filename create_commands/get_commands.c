/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_commands.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/09 19:14:49 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/09 19:14:49 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/functions.h"
#include "internal_create_commands.h"

/**
 * Starts looping through all arguments to find the commands
 *
 * @param	cmd_get		Data needed to create commands
 * @param	minishell	Data for minishell
 *
 * @return	A boolean indicating success
 */
static t_bool	find_commands_in_args(t_cmd_get_struct *cmd_get,
					t_minishell *minishell)
{
	t_pipe_type		pipe_type;
	t_bool			success;

	success = true;
	while (true)
	{
		pipe_type = pipe_type_from_arg(cmd_get->cur_arg->content);
		if (parse_command(cmd_get, minishell) == BREAK)
			break ;
	}
	if (cmd_get->cmd_len != 0 && cmd_get->cur_arg != NULL)
	{
		success = output_pipe_command(cmd_get, pipe_type);
		update_last_command_input(cmd_get->head);
	}
	return (success);
}

/**
 * Finds all the commands and stores them in t_list
 *
 * @param	args	Arguments to create the commands from
 * @param	minishell	Data for minishell
 *
 * @return	NULL on error, command data success
 */
t_list	**find_commands(t_list **args, t_minishell *minishell)
{
	t_cmd_get_struct	cmd_get;

	if (args == NULL)
		return (NULL);
	cmd_get.cur_arg = *args;
	cmd_get.cmd_len = 0;
	cmd_get.head = ft_calloc(1, sizeof(t_list *));
	cmd_get.cur_cmd = NULL;
	if (!cmd_get.head)
		return (NULL);
	if (find_commands_in_args(&cmd_get, minishell) == false)
	{
		free_parse_and_commands(cmd_get.head, NULL);
		return (NULL);
	}
	return (cmd_get.head);
}
