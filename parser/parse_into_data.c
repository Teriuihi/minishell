/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_into_data.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/11 13:59:02 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/11 13:59:02 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/arguments.h"
#include "internal_parser.h"
#include "../headers/functions.h"

/**
 * Parse data found between quotes
 *
 * @param	data	Data for parsing
 * @param	head	List containing all previous arguments
 *
 * @return	A boolean indicating success
 */
static t_bool	parse_quotes(t_parse_data *data, t_list **head,
					t_minishell *minishell)
{
	if (data->pos != 0 && data->pos != data->start)
	{
		if (append_content(data) == false)
			return (false);
	}
	data->pos++;
	data->start = data->pos;
	if (parse_quotation(data, data->input[data->pos - 1],
			head, minishell) == false)
		return (false);
	data->has_data = true;
	data->start = data->pos;
	data->is_literal = true;
	return (true);
}

/**
 * Parse an environment variable
 *
 * @param	data	Data for parsing
 * @param	head	List containing all previous arguments
 *
 * @return	A boolean indicating success
 */
static t_bool	parse_variable(t_parse_data *data, t_list **head,
					t_minishell *minishell)
{
	if (data->pos != 0 && data->pos != data->start)
	{
		if (append_content(data) == false)
			return (false);
	}
	data->pos++;
	data->start = data->pos;
	if (parse_env_variable(data, head, minishell) == false)
		return (false);
	data->start = data->pos;
	data->has_data = true;
	return (true);
}

/**
 * Finalize parsing through the user input (store the leftovers)
 *
 * @param	data	Data for parsing
 * @param	head	Start of the argument list
 *
 * @return	A boolean indicating success
 */
static t_bool	finalize(t_parse_data *data, t_list **head)
{
	t_string	*string;

	if (data->has_data)
	{
		if (append_content(data) == false)
			return (false);
		string = safe_add_to_list(head, data->string, data->is_literal);
		if (string == NULL)
			return (new_set_exit_status(1, "some shell: Out of memory.\n"));
		free_string(string);
	}
	return (true);
}

/**
 * Read through the input and store each argument in the parse data
 *
 * @param	data	Data for parsing
 * @param	head	Start of the argument list
 *
 * @return	A boolean indicating success
 */
t_bool	parse_into_data(t_parse_data *data, t_list **head,
			t_minishell *minishell)
{
	while (data->input[data->pos])
	{
		if (data->input[data->pos] == '"' || data->input[data->pos] == '\'')
		{
			if (parse_quotes(data, head, minishell) == false)
				return (false);
		}
		else if (ft_iswhite_space(data->input[data->pos])
			|| is_pipe(data->input[data->pos]))
		{
			if (store_normal_arg(data, head) == false)
				return (false);
		}
		else if (data->input[data->pos] == '$')
		{
			if (parse_variable(data, head, minishell) == false)
				return (false);
		}
		else
		{
			data->pos++;
			data->has_data = true;
		}
	}
	return (finalize(data, head));
}
