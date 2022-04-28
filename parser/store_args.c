/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   store_args.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/11 13:55:01 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/11 13:55:01 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/arguments.h"
#include "internal_parser.h"
#include "../headers/functions.h"

/**
 * Move the pos and start past any current spaces
 * @param data
 */
static void	skip_space(t_parse_data *data)
{
	while (data->pos && ft_iswhite_space(data->input[data->pos]))
		data->pos++;
	data->start = data->pos;
}

/**
 * Skip through pipes to the next start of a command
 *
 * @param	data		Data for parsing
 * @param	minishell	Data for minishell
 *
 * @return	A boolean indicating success
 */
static t_bool	pipe_in_string_skip(t_parse_data *data)
{
	int		len;

	if (ft_strlen(data->input + data->pos) < 2)
	{
		return (parse_error(*data->input));
	}
	if (!ft_strncmp(data->input + data->pos, "<<", 2)
		|| !ft_strncmp(data->input + data->pos, ">>", 2))
	{
		if (!data->input[data->pos + 2] || is_pipe(data->input[data->pos + 2]))
			return (parse_error(*data->input));
		len = 2;
	}
	else
	{
		if (is_pipe(data->input[data->pos + 1]))
			return (parse_error(data->input[data->pos + 1]));
		len = 1;
	}
	data->pos += len;
	if (append_content(data) == false)
		return (false);
	skip_space(data);
	return (true);
}

t_bool	skip_space_and_return_true(t_parse_data *data)
{
	skip_space(data);
	return (true);
}

/**
 * Store a normal argument separated by spaces
 *
 * @param	data		Data for parsing
 * @param	head		Start of the argument list
 * @param	minishell	Data for minishell
 *
 * @return	A boolean indicating success
 */
t_bool	store_normal_arg(t_parse_data *data, t_list **head)
{
	if (data->has_data)
	{
		if (append_content(data) == false)
			return (false);
		data->string = safe_add_to_list(head, data->string, data->is_literal);
		if (data->string == NULL)
			return (false);
		data->is_literal = false;
	}
	skip_space(data);
	if (is_pipe(data->input[data->pos]))
	{
		if (pipe_in_string_skip(data) == false)
			return (false);
		data->string = safe_add_to_list(head, data->string, data->is_literal);
		if (data->string == NULL)
			return (false);
	}
	data->has_data = false;
	data->is_literal = false;
	skip_space(data);
	if (is_pipe(data->input[data->pos]) && pipe_type_from_arg(((t_arg *)
				ft_lstlast(*head)->content)) != OUTPUT_TO_COMMAND)
		return (parse_error(data->input[data->pos]));
	return (skip_space_and_return_true(data));
}
