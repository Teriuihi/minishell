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
		new_set_exit_status(1, "some shell: Out of memory.\n");
		return (ERROR);
	}
	chdir(minishell->cur_wd);
	if (valid_file_create_if_none(cmd_data) == ERROR)
		return (ERROR);
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
		new_set_exit_status(1, "some shell: Out of memory.\n");
		return (ERROR);
	}
	if (pipe_type == REDIRECT_INPUT)
	{
		chdir(minishell->cur_wd);
		if (access(cmd_data->input.file, R_OK) != 0)
			file_not_there(cmd_data, cmd_get);
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
		if (cmd_data->output.type == NONE
			|| (ft_strlen(cmd_data->command->command) == 4
				&& ft_strncmp(cmd_data->command->command, "exit", 4)))
			cmd_data->output.type = OUTPUT_TO_COMMAND;
		cmd_get->cur_arg = entry->next;
		cmd_get->cur_cmd = NULL;
		return (RET);
	}
	entry = entry->next;
	if (entry == NULL)
	{
		new_set_exit_status(1,
			"some shell: syntax error near unexpected token.");
		return (ERROR);
	}
	cmd_get->cur_arg = entry;
	return (update_pipe_logic(cmd_data, cmd_get, minishell, pipe_type));
}
