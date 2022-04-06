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
#include "../headers/arguments.h"

/**
 * Add new arguments to command after space has been made
 *
 * @param	end		Until what pos to add arguments
 * @param	pos		Position to start adding from
 * @param	cmd		Command to add arguments to
 * @param	entry	Entry to start adding from
 *
 * @return	Boolean indicating success
 */
static t_bool	add_arguments(int end, int pos, t_command *cmd, t_list *entry)
{
	while (pos != end)
	{
		cmd->args[pos] = ft_strdup(((t_arg *)entry->content)->arg->s);
		if (cmd->args[pos] == NULL)
			return (false);
		pos++;
		entry = entry->next;
	}
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
t_bool	append_arguments_to_command(t_command *cmd, t_list *entry, int len,
			t_bool prefix)
{
	char	**new_args;
	int		pos;

	new_args = ft_calloc(cmd->args_len + len + 1, sizeof(char *));
	if (!new_args)
		return (false);
	if (prefix)
		ft_memcpy(new_args + len, cmd->args, cmd->args_len * sizeof(char **));
	else
		ft_memcpy(new_args, cmd->args, cmd->args_len * sizeof(char **));
	pos = cmd->args_len;
	free(cmd->args);
	cmd->args = new_args;
	cmd->args_len += len;
	if (prefix)
		pos = 0;
	else
		len += pos;
	return (add_arguments(len, pos, cmd, entry));
}
