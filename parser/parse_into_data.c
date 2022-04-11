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

static t_bool	parse_quotes(t_parse_data *data, t_minishell *minishell)
{
	if (data->pos != 0 && data->pos != data->start)
	{
		if (append_content(data, minishell) == false)
			return (false);
	}
	data->pos++;
	data->start = data->pos;
	if (parse_quotation(data, data->input[data->pos - 1], minishell) == false)
		return (false);
	data->has_data = true;
	data->start = data->pos;
	data->is_literal = true;
	return (true);
}

static t_bool	parse_variable(t_parse_data *data, t_minishell *minishell)
{
	if (data->pos != 0 && data->pos != data->start)
	{
		if (append_content(data, minishell) == false)
			return (false);
	}
	data->pos++;
	data->start = data->pos;
	if (parse_env_variable(data, minishell) == false)
		return (false);
	data->start = data->pos;
	data->has_data = true;
	return (true);
}

static t_bool	finalize(t_parse_data *data, t_list **head,
					t_minishell *minishell)
{
	if (data->has_data)
	{
		if (append_content(data, minishell) == false)
			return (false);
		if (safe_add_to_list(head, data->string, data->is_literal) == NULL)
			return (set_exit_status(minishell, 1,
					"some shell: Out of memory.", false));
	}
	return (true);
}

t_bool	parse_into_data(t_parse_data *data, t_list **head,
			t_minishell *minishell)
{
	while (data->input[data->pos])
	{
		if (data->input[data->pos] == '"' || data->input[data->pos] == '\'')
		{
			if (parse_quotes(data, minishell) == false)
				return (false);
		}
		else if (ft_iswhite_space(data->input[data->pos])
			|| is_pipe(data->input[data->pos]))
		{
			if (store_normal_arg(data, head, minishell) == false)
				return (false);
		}
		else if (data->input[data->pos] == '$')
		{
			if (parse_variable(data, minishell) == false)
				return (false);
		}
		else
		{
			data->pos++;
			data->has_data = true;
		}
	}
	return (finalize(data, head, minishell));
}
