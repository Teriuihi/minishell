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

#include "../headers/arguments.h"
#include "../headers/functions.h"

/**
 * Get the entry i positions ahead of the current one
 *
 * @param	entry	t_list entry to start from
 * @param	i		Amount of positions to move forward
 *
 * @return	The t_list entry at the given position from the given start
 */
t_list	*get_arg_at_pos(t_list *entry, int i)
{
	if (i < 0)
	{
		while (entry && i++)
			entry = entry->prev;
	}
	else
	{
		while (entry && i--)
			entry = entry->next;
	}
	return (entry);
}

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
 * Get the start of a command
 *
 * @param	cur		Current argument
 * @param	cmd_len	How far backwards the start of the command is
 * @return
 */
t_list	*get_command_start(t_list *cur, int cmd_len)
{
	while (cmd_len--)
		cur = cur->prev;
	return (cur);
}

/**
 * Safely get pipe type from argument
 *
 * @param	arg	arg to get pipe type from
 *
 * @return	pipe_type of arg or NONE if arg is literal
 */
t_pipe_type	pipe_type_from_arg(t_arg *arg)
{
	t_pipe_type	pipe_type;

	if (arg->literal)
		return (NONE);
	pipe_type = command_separator_type(arg->arg->s);
	if (pipe_type == NONE)
		return (NONE);
	return (pipe_type);
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
