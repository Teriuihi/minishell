/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 17:33:00 by sappunn        #+#    #+#                */
/*   Updated: 2022/02/28 17:33:00 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/arguments.h"
#include "internal_parser.h"
#include "../headers/functions.h"

/**
 * Check if the parsed data is valid
 *
 * @param	entry		Entry to start checking from
 * @param	minishell	Data for minishell
 *
 * @return	Boolean indicating if the parsed data is valid
 */
t_bool	validate_parse(t_list *entry, t_minishell *minishell)
{
	t_pipe_type	prev_pipe;
	t_pipe_type	cur_pipe;

	prev_pipe = NONE;
	while (entry != NULL)
	{
		cur_pipe = pipe_type_from_arg(entry->content);
		if (prev_pipe != NONE && cur_pipe != NONE)
			return (parse_error(minishell, *((t_arg *)entry->content)->arg->s));
		prev_pipe = cur_pipe;
		entry = entry->next;
	}
	if (prev_pipe != NONE)
		return (false);
	return (true);
}

/**
 * Initialize data used for parsing
 *
 * @param	data	Data used for parsing
 * @param	input	The user input to be parsed
 * @param	head	Start of the list of arguments
 *
 * @return	A boolean indicating success
 */
t_bool	init_parse(t_parse_data *data, char *input, t_list ***head,
			t_minishell *minishell)
{
	data->pos = 0;
	data->input = input;
	data->has_data = false;
	data->is_literal = false;
	data->string = init_string(NULL);
	*head = ft_calloc(1, sizeof(t_list *));
	if (data->string == NULL)
		return (set_exit_status(minishell, 1,
				"some shell: Out of memory.", false));
	while (ft_iswhite_space(input[data->pos]))
		data->pos++;
	data->start = data->pos;
	return (true);
}

/**
 * Free's all data in given list
 *
 * @param	head	Start of the list
 */
void	free_argument_list(t_list **head)
{
	t_list	*cur;
	t_list	*last;
	t_arg	*arg;

	cur = *head;
	while (cur)
	{
		arg = cur->content;
		free_string(arg->arg);
		free(arg);
		last = cur;
		cur = cur->next;
		free(last);
	}
	free(head);
}

/**
 * Parse user input
 *
 * @param	input	Input to parse
 *
 * @return	Array of arguments the user entered
 */
t_list	**parse(char *input, t_minishell *minishell)
{
	t_list			**head;
	t_parse_data	data;

	if (init_parse(&data, input, &head, minishell) == false)
		return (NULL);
	if (parse_into_data(&data, head, minishell) == true)
	{
		if (validate_parse(*head, minishell) == true)
			return (head);
		else
			free_argument_list(head);
	}
	return (NULL);
}
