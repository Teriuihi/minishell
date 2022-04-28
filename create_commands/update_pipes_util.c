/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   update_pipes_util.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/28 17:17:13 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/28 17:17:13 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/functions.h"
#include "internal_create_commands.h"

/**
 * Send an error indicating the file specified in the command was not found
 *
 * @param	cmd_data	Current command and it's attributes
 * @param	cmd_get		Data needed to create commands
 * @param	minishell	Data for minishell
 *
 * @return	The exit state
 */
void	file_not_there(t_cmd_data *cmd_data,
					t_cmd_get_struct *cmd_get)
{
	if (cmd_data->has_error == true)
		return ;
	cmd_data->has_error = true;
	if (access(cmd_data->input.file, F_OK) != 0)
		ft_printf(2, "some shell: %s: No such file or directory\n",
			cmd_data->input.file);
	else
		ft_printf(2, "some shell: %s: Permission denied\n",
			cmd_data->input.file);
	while (cmd_get->cur_arg != NULL
		&& pipe_type_from_arg(cmd_get->cur_arg->content) != OUTPUT_TO_COMMAND)
		cmd_get->cur_arg = cmd_get->cur_arg->next;
}

/**
 * Checks if a file exists, and creates it if it doesn't,
 * 	checks if an existing file is writable, if not errors
 *
 * @param	cmd_data	Current command and it's attributes
 *
 * @return	ERROR if an issue is encountered, CONTINUE if all is good
 */
t_exit_state	valid_file_create_if_none(t_cmd_data *cmd_data)
{
	if (cmd_data->has_error == true)
		return (CONTINUE);
	if (access(cmd_data->output.file, F_OK) != 0)
	{
		if (create_file(cmd_data) == false)
		{
			cmd_data->has_error = true;
		}
	}
	else
	{
		if (cmd_data->has_error == false
			&& access(cmd_data->output.file, W_OK) != 0)
		{
			cmd_data->has_error = true;
			new_set_exit_status(1, "some shell: %s: Permission denied\n",
				cmd_data->output.file);
		}
	}
	return (CONTINUE);
}
