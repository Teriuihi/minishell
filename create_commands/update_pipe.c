/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   update_pipe.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/11 16:28:01 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/11 16:28:01 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/functions.h"
#include "internal_create_commands.h"
#include <fcntl.h>

/**
 * Send an error indicating the file specified in the command was not found
 *
 * @param	cmd_data	Current command and it's attributes
 * @param	cmd_get		Data needed to create commands
 * @param	minishell	Data for minishell
 *
 * @return	The exit state
 */
static t_exit_state	file_not_there(t_cmd_data *cmd_data,
						t_cmd_get_struct *cmd_get, t_minishell *minishell)
{
	char	*message;
	char	*message1;

	message1 = ft_strjoin(cmd_data->input.file,
			": No such file or directory\n");
	if (message1 == NULL)
	{
		set_exit_status(minishell, 1, "some shell: Out of memory.", true);
		return (ERROR);
	}
	message = ft_strjoin("some shell: parse: ", message1);
	free(message1);
	if (message == NULL)
	{
		set_exit_status(minishell, 1, "some shell: Out of memory.", true);
		return (ERROR);
	}
	else
		set_exit_status(minishell, 1, message, true);
	while (cmd_get->cur_arg != NULL
		&& pipe_type_from_arg(cmd_get->cur_arg->content) != OUTPUT_TO_COMMAND)
		cmd_get->cur_arg = cmd_get->cur_arg->next;
	return (ERROR);
}

/**
 * Add the file output to the current command
 * 	and create the file if it doesn't exist
 *
 * @param	cmd_data	Current command and it's attributes
 * @param	cmd_get		Data needed to create commands
 * @param	minishell	Data for minishell
 *
 * @return	The exit state
 */
static t_exit_state	file_output(t_cmd_data *cmd_data, t_cmd_get_struct *cmd_get,
					t_minishell *minishell)
{
	t_list	*entry;

	entry = cmd_get->cur_arg;
	if (cmd_data->output.file)
		free(cmd_data->output.file);
	cmd_data->output.file = ft_strdup(str_from_arg(entry));
	if (cmd_data->output.file == NULL)
	{
		set_exit_status(minishell, 1, "some shell: Out of memory.", false);
		return (ERROR);
	}
	chdir(minishell->cur_wd);
	if (access(cmd_data->output.file, F_OK) != 0)
	{
		if (create_file(cmd_data, minishell) == false)
			return (ERROR);
	}
	cmd_get->cur_arg = entry;
	return (CONTINUE);
}

/**
 * Add the file input to the current command and validate that the file exists
 *
 * @param	cmd_data	Current command and it's attributes
 * @param	cmd_get		Data needed to create commands
 * @param	minishell	Data for minishell
 * @param	pipe_type	Pipe type for this input
 *
 * @return	The exit state
 */
static t_exit_state	file_input(t_cmd_data *cmd_data, t_cmd_get_struct *cmd_get,
						t_minishell *minishell, t_pipe_type pipe_type)
{
	t_list	*entry;

	entry = cmd_get->cur_arg;
	if (cmd_data->input.file)
		free(cmd_data->input.file);
	cmd_data->input.file = ft_strdup(str_from_arg(entry));
	if (cmd_data->input.file == NULL)
	{
		set_exit_status(minishell, 1, "some shell: Out of memory.", false);
		return (ERROR);
	}
	if (pipe_type == REDIRECT_INPUT)
	{
		chdir(minishell->cur_wd);
		if (access(cmd_data->input.file, R_OK) != 0)
			return (file_not_there(cmd_data, cmd_get, minishell));
	}
	cmd_get->cur_arg = entry;
	return (CONTINUE);
}

/**
 * Check the current pipe and add the files needed based on that
 *
 * @param	cmd_data	Current command and it's attributes
 * @param	cmd_get		Data needed to create commands
 * @param	minishell	Data for minishell
 * @param	pipe_type	Pipe type for this input
 *
 * @return	The exit state
 */
static t_exit_state	update_pipe_logic(t_cmd_data *cmd_data,
						t_cmd_get_struct *cmd_get, t_minishell *minishell,
						t_pipe_type pipe_type)
{
	t_exit_state	exit_state;

	if ((pipe_type == DELIMITER_INPUT || pipe_type == REDIRECT_INPUT))
	{
		cmd_data->input.type = pipe_type;
		exit_state = file_input(cmd_data, cmd_get, minishell, pipe_type);
	}
	else if ((pipe_type == APPEND_OUTPUT || pipe_type == REDIRECT_OUTPUT))
	{
		cmd_data->output.type = pipe_type;
		exit_state = file_output(cmd_data, cmd_get, minishell);
	}
	else
		exit_state = ERROR;
	if (exit_state != ERROR)
		cmd_get->cur_arg = cmd_get->cur_arg->next;
	return (exit_state);
}

/**
 * Update the pipe input or output data for a command
 *
 * @param	cmd_data	Current command and it's attributes
 * @param	cmd_get		Data needed to create commands
 * @param	minishell	Data for minishell
 *
 * @return	The exit state
 */
t_exit_state	update_pipe(t_cmd_data *cmd_data, t_cmd_get_struct *cmd_get,
					t_minishell *minishell)
{
	t_pipe_type	pipe_type;
	t_list		*entry;

	entry = cmd_get->cur_arg;
	pipe_type = pipe_type_from_arg(entry->content);
	if (pipe_type == OUTPUT_TO_COMMAND)
	{
		cmd_get->cur_arg = entry->next;
		return (RETURN);
	}
	entry = entry->next;
	if (entry == NULL)
	{
		set_exit_status(minishell, 1,
			"some shell: syntax error near unexpected token.", false);
		return (ERROR);
	}
	cmd_get->cur_arg = entry;
	return (update_pipe_logic(cmd_data, cmd_get, minishell, pipe_type));
}
