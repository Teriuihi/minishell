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
 * Set's the output for a command to pipe type and skips to the next argument
 *
 * @param	cmd_get		All data for getting commands
 * @param	pipe_type	Pipe type to set the command to
 *
 * @return	A boolean indicating success
 */
t_bool	output_pipe_command(t_cmd_get_struct *cmd_get, t_pipe_type pipe_type)
{
	t_cmd_data	*cmd_data;
	t_list		*entry;

	entry = cmd_get->cur_arg;
	cmd_data = cmd_get->cur_cmd;
	if (!cmd_data)
		return (false);
	cmd_data->output.type = pipe_type;
	cmd_get->cur_cmd = NULL;
	cmd_get->cur_arg = entry->next;
	return (true);
}

static t_bool	is_assignment(char *str)
{
	if (str == NULL)
		return (false);
	while (*str)
	{
		if (*str == '=')
			return (true);
		str++;
	}
	return (false);
}

/**
 * Loop through arguments until the end of the command is
 * 	reached and add them to the command
 *
 * @param	entry		Entry to start from
 * @param	cmd_get		Data needed to create commands
 * @param	cmd_data	Current command and it's attributes
 * @param	minishell	Data for minishell
 *
 * @return	A boolean indicating success
 */
static t_bool	loop_through_entries(t_list *entry, t_cmd_get_struct *cmd_get,
			t_cmd_data *cmd_data, t_minishell *minishell)
{
	t_exit_state	exit_state;
	t_pipe_type		pipe_type;

	while (entry != NULL)
	{
		pipe_type = pipe_type_from_arg(entry->content);
		if (pipe_type == NONE)
		{
			if (is_assignment(cmd_get->cur_cmd->command->command) == true)
				cmd_get->cur_cmd = create_new_cmd(cmd_get->head);
			if (append_arguments_to_command(cmd_data->command, entry, cmd_get)
				== false)
				return (false);
			entry = cmd_get->cur_arg;
			continue ;
		}
		cmd_get->cur_arg = entry;
		exit_state = update_pipe(cmd_data, cmd_get, minishell);
		if (exit_state == ERROR)
			return (false);
		else if (exit_state == RET)
			return (true);
		entry = cmd_get->cur_arg;
	}
	return (true);
}

/**
 * Handles creating commands for types that need to create input and forward it
 * 	to the next command
 *
 * @param	cmd_get		All data for getting commands
 * @param	entry		Entry to get pipe type for
 * @param	minishell	Data for minishell
 *
 * @return	A boolean indicating success
 */
t_bool	pipe_command(t_cmd_get_struct *cmd_get, t_list *entry,
			t_minishell *minishell)
{
	t_cmd_data		*cmd_data;

	cmd_data = cmd_get->cur_cmd;
	if (cmd_data == NULL)
		return (false);
	return (loop_through_entries(entry, cmd_get, cmd_data, minishell));
}
