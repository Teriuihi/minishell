/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_command.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/06 19:04:28 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/06 19:04:28 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */
#include "../headers/structs.h"
#include "internal_create_commands.h"

static t_bool	func(t_pipe_type pipe_type, t_cmd_get_struct *cmd_get,
			t_minishell *minishell)
{
	if (pipe_type == OUTPUT_TO_COMMAND)
	{
		cmd_get->cur_arg = get_command_start(cmd_get->cur_arg,
				cmd_get->cmd_len);
		return (output_pipe_command(cmd_get, pipe_type));
	}
	else if (pipe_type == NONE)
		cmd_get->cmd_len++;
	else
	{
		cmd_get->cur_arg = get_command_start(cmd_get->cur_arg,
				cmd_get->cmd_len);
		return (pipe_command(cmd_get, pipe_type, minishell));
	}
	return (true);
}

t_exit_state	parse_command(t_cmd_get_struct *cmd_get, t_minishell *minishell)
{
	t_pipe_type	pipe_type;
	t_bool		success;

	pipe_type = pipe_type_from_arg(cmd_get->cur_arg->content);
	success = func(pipe_type, cmd_get, minishell);
	update_last_command_input(cmd_get->head);
	if (success == false)
	{
		ft_lstremove_last(cmd_get->head);
		return (CONTINUE);
	}
	if (cmd_get->cur_arg == NULL || (cmd_get->cur_arg)->next == NULL)
		return (BREAK); //TODO in > < >> << command check if there is a next if we end on |
	cmd_get->cur_arg = (cmd_get->cur_arg)->next;
	return (CONTINUE);
}
