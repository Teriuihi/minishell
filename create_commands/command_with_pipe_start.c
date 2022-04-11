/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   command_with_pipe_start.c                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/11 17:01:29 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/11 17:01:29 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/functions.h"
#include "internal_create_commands.h"
#include <fcntl.h>

t_exit_state	ptr3(t_cmd_get_struct *cmd_get, t_list *entry,
					t_cmd_data *cmd_data, t_minishell *minishell)
{
	cmd_data->command->command = ft_strdup(((t_arg *)entry->content)->arg->s);
	if (cmd_data->command->command == NULL)
	{
		free(cmd_data);
		set_exit_status(minishell, 1, "some shell: Out of memory.", false);
		return (ERROR);
	}
	if (append_arguments_to_command(cmd_data->command, entry, 1, true) == false)
	{
		free(cmd_data->command->command);
		set_exit_status(minishell, 1, "some shell: Out of memory.", false);
		return (ERROR);
	}
	entry = entry->next;
	cmd_get->cur_arg = entry;
	while (entry != NULL && !pipe_type_from_arg(entry->content))
	{
		cmd_get->cmd_len++;
		entry = entry->next;
	}
	return (CONTINUE);
}

t_exit_state	ptr2(t_cmd_get_struct *cmd_get, t_list *entry,
					t_cmd_data *cmd_data, t_minishell *minishell)
{
	t_pipe_type		pipe_type;
	t_exit_state	exit_state;

	while (entry != NULL)
	{
		pipe_type = pipe_type_from_arg(entry->content);
		if (pipe_type == NONE)
			break ;
		cmd_get->cur_arg = entry;
		exit_state = update_pipe(cmd_data, cmd_get, minishell);
		if (exit_state == ERROR)
			return (ERROR);
		if (exit_state == RETURN)
			return (RETURN);
		entry = cmd_get->cur_arg;
	}
	if (entry == NULL)
	{
		cmd_get->cur_arg = entry;
		return (RETURN);
	}
	return (CONTINUE);
}

t_cmd_data	*command_with_pipe_start(t_cmd_get_struct *cmd_get, t_list *entry,
				t_minishell *minishell)
{
	t_cmd_data		*cmd_data;
	t_exit_state	exit_state;

	cmd_data = create_new_cmd(cmd_get->head, NULL);
	if (cmd_data == NULL)
	{
		{
			set_exit_status(minishell, 1, "some shell: Out of memory.", false);
			return (NULL);
		}
	}
	exit_state = ptr2(cmd_get, entry, cmd_data, minishell);
	if (exit_state == RETURN)
		return (cmd_data);
	else if (exit_state == ERROR)
		return (NULL);
	exit_state = ptr3(cmd_get, entry, cmd_data, minishell);
	if (exit_state == ERROR)
		return (NULL);
	return (cmd_data);
}
