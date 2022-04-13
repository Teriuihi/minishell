/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   util.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/06 19:06:02 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/06 19:06:02 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "../headers/arguments.h"
#include "../headers/functions.h"

/**
 * Get the string from a t_list entry which contains a t_arg
 *
 * @param	entry	Entry to get string from
 *
 * @return	String from entry
 */
char	*str_from_arg(t_list *entry)
{
	t_arg	*arg;

	arg = entry->content;
	return (arg->arg->s);
}

/**
 * Set the input type for a command if it doesn't have one yet but
 * 	does have a previous command
 *
 * @param	head	List containing all commands
 */
void	update_last_command_input(t_list **head)
{
	t_list		*entry;
	t_cmd_data	*cmd_data;

	if (head == NULL || *head == NULL)
		return ;
	entry = ft_lstlast(*head);
	cmd_data = entry->content;
	if (cmd_data->input.type != NONE || entry->prev == NULL)
		return ;
	cmd_data->input.type = OUTPUT_TO_COMMAND;
}

/**
 * Create the output file for a command
 *
 * @param	cmd_data	Current command and it's attributes
 * @param	minishell	Data for minishell
 *
 * @return	Boolean indicating success
 */
t_bool	create_file(t_cmd_data *cmd_data)
{
	int	fd;

	fd = open(cmd_data->output.file, O_CREAT, 0777);
	if (fd < 0)
		return (new_set_exit_status(1,
				"some shell: %s: Permission denied", cmd_data->output.file));
	else
	{
		if (close(fd) != 0)
			return (new_set_exit_status(1,
					"some shell: %s: Unable to close file",
					cmd_data->output.file));
	}
	return (true);
}
