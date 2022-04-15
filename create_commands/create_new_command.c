/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   create_new_command.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/06 19:21:58 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/06 19:21:58 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/functions.h"
#include "internal_create_commands.h"

/**
 * Stores a command, on error command is freed
 * 	all other command data isn't freed
 *
 * @param	cmd_data	Command to store
 * @param	head		List to store the command in
 *
 * @return	The command data that was stored or NULL on failure
 */
static t_cmd_data	*store_command(t_cmd_data *cmd_data, t_list **head)
{
	t_list	*new;

	new = ft_lstnew(cmd_data);
	if (new == NULL)
	{
		free_cmd(cmd_data);
		return (NULL);
	}
	ft_lstadd_back(head, new);
	return (cmd_data);
}

/**
 * Creates an empty command
 *
 * @param	head	List containing all commands
 *
 * @return	The command data for the new command or NULL on failure
 */
t_cmd_data	*create_new_cmd(t_list **head)
{
	t_cmd_data	*cmd_data;

	cmd_data = ft_calloc(1, sizeof(t_cmd_data));
	if (!cmd_data)
		return (NULL);
	cmd_data->command = ft_calloc(1, sizeof(t_command));
	if (!cmd_data->command)
		return (NULL);
	cmd_data->command->command = NULL;
	return (store_command(cmd_data, head));
}
