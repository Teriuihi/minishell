/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   create_command.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/06 19:17:57 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/06 19:17:57 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/functions.h"
#include "internal_create_commands.h"

/**
 * Handles command creation for all pipe_type's not handled by pipe_command
 *
 * @param	t_list		All data related to commands
 * @param	args		All arguments
 * @param	start_pos	Start of arguments for this command in args
 * @param	len			Amount of arguments after start that belong to
 * 	this command
 *
 * @return	non zero on error, 0 on success
 */
t_bool	output_pipe_command(t_cmd_get_struct *cmd_get, t_pipe_type pipe_type)
{
	t_cmd_data	*cmd_data;
	t_list		*entry;

	entry = cmd_get->cur_arg;
	if (cmd_get->cmd_len == 0)
		cmd_data = create_new_cmd(cmd_get->head, NULL);
	else
		cmd_data = create_new_cmd(cmd_get->head,
				((t_arg *)entry->content)->arg->s);
	if (!cmd_data)
		return (false);
	if (cmd_get->cmd_len > 1)
		if (append_arguments_to_command(cmd_data->command, entry->next,
				cmd_get->cmd_len - 1, false) == false)
			return (false);
	cmd_data->output.type = pipe_type;
	entry = get_arg_at_pos(cmd_get->cur_arg, cmd_get->cmd_len);
	cmd_get->cmd_len = 0;
	cmd_get->cur_arg = entry;
	return (true);
}

t_bool	pipe_part_2(t_list *entry, t_cmd_get_struct *cmd_get,
			t_cmd_data *cmd_data, t_minishell *minishell)
{
	t_exit_state	exit_state;
	t_pipe_type		pipe_type;

	while (entry != NULL)
	{
		pipe_type = pipe_type_from_arg(entry->content);
		if (pipe_type == NONE)
		{
			if (append_arguments_to_command(cmd_data->command,
					entry, 1, false) == false)
				return (false);
			entry = entry->next;
			continue ;
		}
		cmd_get->cur_arg = entry;
		exit_state = update_pipe(cmd_data, cmd_get, minishell);
		if (exit_state == ERROR)
			return (false);
		else if (exit_state == RETURN)
			return (true);
		entry = cmd_get->cur_arg;
	}
	cmd_get->cur_arg = entry;
	return (true);
}

/**
 * Handles creating commands for types that need to create input and forward it
 * 	to the next command
 *
 * @param	t_list		All data related to commands
 * @param	args		All arguments
 * @param	start_pos	Start of arguments for this command in args
 * @param	len			Amount of arguments after start that belong to
 * 	this command
 *
 * @return	non zero on error, 0 on success
 */
t_bool	pipe_command(t_cmd_get_struct *cmd_get, t_list *entry,
			t_minishell *minishell)
{
	t_cmd_data		*cmd_data;
	t_bool			success;

	if (cmd_get->cmd_len == 0)
	{
		cmd_data = command_with_pipe_start(cmd_get, entry, minishell);
		entry = cmd_get->cur_arg;
	}
	else
	{
		cmd_data = create_new_cmd(cmd_get->head,
				((t_arg *)entry->content)->arg->s);
		entry = get_arg_at_pos(entry, cmd_get->cmd_len - 1);
	}
	if (cmd_data == NULL)
		return (false);
	if (entry != NULL && cmd_get->cmd_len > 1
		&& append_arguments_to_command(cmd_data->command, entry->next,
			(cmd_get->cmd_len - 1), false) == false)
		return (false);
	success = pipe_part_2(entry->next, cmd_get, cmd_data, minishell);
	return (success);
}
