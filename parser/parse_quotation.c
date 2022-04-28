/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_quotation.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/01 19:06:20 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/01 19:06:20 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/arguments.h"
#include "internal_parser.h"
#include "../headers/functions.h"

/**
 * Parse an environment variable when pos is on a $
 * 	and we're in a part of the string where these variables
 * 	should be parsed
 *
 * @param	data	Data used for parsing
 * @param	head	List containing all previous arguments
 *
 * @return	Boolean indicating success
 */
t_bool	handle_env_variable(t_parse_data *data, t_list **head,
			t_minishell *minishell)
{
	if (append_content(data) == false)
		return (false);
	if (data->string == NULL)
		return (new_set_exit_status(1, "some shell: Out of memory.\n"));
	data->pos++;
	data->start = data->pos;
	if (parse_env_variable(data, head, minishell) == false)
		return (false);
	data->start = data->pos;
	return (true);
}

/**
 * Parse string between quotes
 *
 * @param	data	Data used for parsing
 * @param	quote	Quotation character that ends this quote
 * @param	head	List containing all previous arguments
 *
 * @return	Boolean indicating success
 */
t_bool	parse_quotation(t_parse_data *data, char quote, t_list **head,
			t_minishell *minishell)
{
	while (data->input[data->pos])
	{
		if (quote == '"' && data->input[data->pos] == '$')
		{
			if (handle_env_variable(data, head, minishell) == false)
				return (false);
		}
		else if (data->input[data->pos] == quote)
		{
			if (data->pos != data->start)
			{
				if (append_content(data) == false)
					return (false);
			}
			data->start = ++data->pos;
			return (true);
		}
		else
			data->pos++;
	}
	return (true);
}
