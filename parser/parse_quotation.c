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

t_string	*handle_env_variable(char *input, int *start, int *pos, t_string *arg)
{
	arg = append_content(input, *start, *pos, arg);
	if (arg == NULL)
		return (NULL);
	arg = parse_env_variable(input, ++(*pos), pos, arg);
	if (arg == NULL)
		return (NULL);
	*start = *pos;
	return (arg);
}

/**
 * Parse string between quotes
 *
 * @param	input	Input to append from
 * @param	start	Start pos in input
 * @param	quote	Quotation character that ends this quote
 * @param	arg		Current argument we're appending to
 *
 * @return	String we appended too (could have a different address now)
 */
t_string	*parse_quotation(char *input, int *start, char quote, t_string *arg)
{
	int		pos;

	pos = *start;
	while (input[pos])
	{
		if (quote == '"' && input[pos] == '$')
		{
			arg = handle_env_variable(input, start, &pos, arg);
			if (arg == NULL)
				return (NULL);
		}
		else if (input[pos] == quote)
		{
			if (pos != *start)
				arg = append_content(input, *start, pos, arg);
			if (arg == NULL)
				return (NULL);
			*start = pos + 1;
			return (arg);
		}
		else
			pos++;
	}
	return (arg);
}
