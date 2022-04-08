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
#include "../headers/arguments.h"
#include "internal_create_commands.h"
#include <fcntl.h>

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

void	go_to_end_cmd(t_cmd_get_struct *cmd_get)
{
	while (cmd_get->cur_arg != NULL
		&& pipe_type_from_arg(cmd_get->cur_arg->content) != OUTPUT_TO_COMMAND)
		cmd_get->cur_arg = cmd_get->cur_arg->next;
}

t_exit_state	update_pipe(t_cmd_data *cmd_data, t_cmd_get_struct *cmd_get,
							t_minishell *minishell)
{
	char		*message;
	t_pipe_type	pipe_type;
	t_list		*entry;

	entry = cmd_get->cur_arg;
	pipe_type = pipe_type_from_arg(entry->content);
	if (pipe_type == OUTPUT_TO_COMMAND)
	{
		cmd_get->cur_arg = entry;
		return (RETURN);
	}
	entry = entry->next;
	if (entry == NULL)
	{
		set_exit_status(minishell, 1,
			"some shell: syntax error near unexpected token.");
		return (ERROR);
	}
	if ((pipe_type == DELIMITER_INPUT || pipe_type == REDIRECT_INPUT))
	{
		cmd_data->input.type = pipe_type;
		if (cmd_data->input.file)
			free(cmd_data->input.file);
		cmd_data->input.file = ft_strdup(str_from_arg(entry));
		if (cmd_data->input.file == NULL)
		{
			set_exit_status(minishell, 1, "some shell: Out of memory.");
			return (ERROR);
		}
		if (pipe_type == REDIRECT_INPUT)
		{
			chdir(minishell->cur_wd);
			if (access(cmd_data->input.file, R_OK) != 0)
			{
				message = ft_strjoin("some shell: parse2: ", ft_strjoin(cmd_data->input.file, ": No such file or directory\n"));
				if (message == NULL)
					message = "some shell: Out of memory.";
				set_exit_status(minishell, 1, message);
				go_to_end_cmd(cmd_get);
				return (ERROR);
			}
		}
	}
	else if ((pipe_type == APPEND_OUTPUT || pipe_type == REDIRECT_OUTPUT))
	{
		cmd_data->output.type = pipe_type;
		if (cmd_data->output.file)
			free(cmd_data->output.file);
		cmd_data->output.file = ft_strdup(str_from_arg(entry));
		if (cmd_data->output.file == NULL)
		{
			set_exit_status(minishell, 1, "some shell: Out of memory.");
			return (ERROR);
		}
		chdir(minishell->cur_wd);
		if (access(cmd_data->output.file, F_OK) != 0)
			close(open(cmd_data->output.file, O_CREAT, 0777));
	}
	cmd_get->cur_arg = entry->next;
	return (CONTINUE);
}

t_cmd_data	*command_with_pipe_start(t_cmd_get_struct *cmd_get, t_list *entry, t_minishell *minishell)
{
	t_cmd_data		*cmd_data;
	t_pipe_type		pipe_type;
	t_exit_state	exit_state;

	cmd_data = create_new_cmd(cmd_get->head, NULL);
	if (cmd_data == NULL)
	{
		{
			set_exit_status(minishell, 1, "some shell: Out of memory.");
			return (NULL);
		}
	}
	while (entry != NULL)
	{
		pipe_type = pipe_type_from_arg(entry->content);
		if (pipe_type == NONE)
			break ;
		cmd_get->cur_arg = entry;
		exit_state = update_pipe(cmd_data, cmd_get, minishell);
		if (exit_state == ERROR)
		{
			//TODO ERROR free
			return (NULL);
		}
		if (exit_state == RETURN)
			return (cmd_data);
		entry = cmd_get->cur_arg;
	}
	if (entry == NULL)
	{
		cmd_get->cur_arg = entry;
		return (cmd_data);
	}
	cmd_data->command->command = ft_strdup(((t_arg *)entry->content)->arg->s);
	if (cmd_data->command->command == NULL)
		return (NULL); //TODO free
	if (append_arguments_to_command(cmd_data->command, entry, 1, true) == false)
		return (NULL); //TODO free
	entry = entry->next;
	cmd_get->cur_arg = entry;
	while (entry != NULL && !pipe_type_from_arg(entry->content))
	{
		cmd_get->cmd_len++;
		entry = entry->next;
	}
	return (cmd_data);
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
		entry = get_arg_at_pos(entry, cmd_get->cmd_len);
	}
	if (cmd_data == NULL)
		return (false);
	if (entry != NULL && cmd_get->cmd_len > 1
		&& append_arguments_to_command(cmd_data->command, entry->next,
			(cmd_get->cmd_len - 1), false) == false)
		return (false);
	success = pipe_part_2(entry, cmd_get, cmd_data, minishell);
	if (success && cmd_get->cur_arg != NULL)
		cmd_get->cur_arg = entry->prev;
	return (success);
}
