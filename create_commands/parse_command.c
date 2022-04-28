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
#include "../headers/functions.h"

static t_bool	is_assignment(char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (false);
	while (str[i])
	{
		if (str[i] == '=')
		{
			if (i == 0)
				return (false);
			if (ft_iswhite_space(str[i - 1]))
				return (false);
			return (true);
		}
		i++;
	}
	return (false);
}

/**
 * Check if we find a pipe type, if not increase command length
 * 	if we do find it create the command
 *
 * @param	pipe_type	Pipe type to check
 * @param	cmd_get		Data needed to create commands
 * @param	minishell	Data for minishell
 *
 * @return	Boolean indicating success
 */
static t_bool	check_pipe_make_command(t_pipe_type pipe_type,
					t_cmd_get_struct *cmd_get, t_minishell *minishell)
{
	if (cmd_get->cur_cmd == NULL)
		cmd_get->cur_cmd = create_new_cmd(cmd_get->head);
	if (cmd_get->cur_cmd == NULL)
		return (false);
	if (pipe_type == OUTPUT_TO_COMMAND)
		return (output_pipe_command(cmd_get, pipe_type));
	else if (pipe_type == NONE)
	{
		if (is_assignment(cmd_get->cur_cmd->command->command) == true)
			cmd_get->cur_cmd = create_new_cmd(cmd_get->head);
		return (append_arguments_to_command(cmd_get->cur_cmd->command,
				cmd_get->cur_arg, cmd_get));
	}
	else
		return (pipe_command(cmd_get, cmd_get->cur_arg, minishell));
}

/**
 * Parse the current argument and create the command if needed
 *
 * @param	cmd_get		Data needed to create commands
 * @param	minishell	Data for minishell
 *
 * @return	Enum indicating to continue or break the loop
 */
t_exit_state	parse_command(t_cmd_get_struct *cmd_get, t_minishell *minishell)
{
	t_pipe_type	pipe_type;
	t_bool		success;

	pipe_type = pipe_type_from_arg(cmd_get->cur_arg->content);
	success = check_pipe_make_command(pipe_type, cmd_get, minishell);
	update_last_command_input(cmd_get->head);
	if (success == false)
	{
		ft_lstremove_last(cmd_get->head);
		cmd_get->cur_cmd = (t_cmd_data *) ft_lstlast(*cmd_get->head);
		if (cmd_get->cur_arg == NULL)
			return (BREAK);
		return (CONTINUE);
	}
	if (cmd_get->cur_arg == NULL)
		return (BREAK);
	return (CONTINUE);
}
