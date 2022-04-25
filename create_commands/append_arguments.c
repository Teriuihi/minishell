/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   append_arguments.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/06 16:10:22 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/06 16:10:22 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/functions.h"
#include "internal_create_commands.h"

t_bool	set_command(t_command *command, t_list *entry)
{
	if (command->command != NULL)
		free(command->command);
	command->command = ft_strdup(((t_arg *)entry->content)->arg->s);
	if (command->command == NULL)
		return (false);
	return (true);
}

/**
 * Append len given arguments to given command
 *
 * @param	cmd		Command to append arguments to
 * @param	entry	Entry to start adding from
 * @param	len		Amount of entries to add
 *
 * @return	true on success false on failure
 */
t_bool	append_arguments_to_command(t_command *cmd, t_list *entry,
			t_cmd_get_struct *cmd_get)
{
	char	**new_args;

	if (cmd->command == NULL)
	{
		if (set_command(cmd, entry) == false)
			return (false);
	}
	new_args = ft_calloc(cmd->args_len + 2, sizeof(char *));
	if (!new_args)
		return (false);
	else
		ft_memcpy(new_args, cmd->args, cmd->args_len * sizeof(char **));
	free(cmd->args);
	cmd->args = new_args;
	cmd->args[cmd->args_len] = ft_strdup(((t_arg *)entry->content)->arg->s);
	if (cmd->args[cmd->args_len] == NULL)
		return (false);
	cmd->args_len++;
	cmd_get->cur_arg = entry->next;
	return (true);
}
