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

static t_bool	is_pipe(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static void	skip_space(t_parse_data *data)
{
	while (data->pos && ft_iswhite_space(data->input[data->pos]))
		data->pos++;
}

t_bool	pipe_in_string_skip(int start, t_parse_data *data, t_minishell *minishell)
{
	int	len;

	if (ft_strlen(data->input + data->pos) < 2)
		return (false); //TODO Parsing error since it has to be at least 2 chars (ex: <a)
	if (!ft_strncmp(data->input + data->pos, "<<", 2) || !ft_strncmp(data->input + data->pos, ">>", 2))
	{
		if (!data->input[data->pos + 2] || is_pipe(data->input[data->pos + 2]))
			return (false); //TODO Parsing error since it has to be at least 3 chars (ex: >>a) and there can't be another pipe after
		len = 2;
	}
	else
	{
		if (is_pipe(data->input[data->pos + 1])) //TODO check further than just the next character (check to end of space)
			return (false); //TODO Parsing error since there can't be a pipe after
		len = 1;
	}
	data->pos += len;
	if (append_content(data, minishell) == false)
		return (false);
	skip_space(data);
	return (true);
}

static t_bool	func_3(t_parse_data *data, t_minishell *minishell)
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

static t_bool	func_2(t_parse_data *data, t_list **head,
					t_minishell *minishell)
{
	if (data->has_data)
	{
		if (append_content(data, minishell) == false)
			return (false);
		data->string = safe_add_to_list(head, data->string, data->is_literal);
		if (data->string == NULL)
			return (false);
	}
	skip_space(data);
	data->start = data->pos;
	if (is_pipe(data->input[data->pos]))
	{
		if (pipe_in_string_skip(data->pos, data, minishell) == false)
		{
			//TODO parsing error
		}
		data->string = safe_add_to_list(head, data->string, data->is_literal);
		if (data->string == NULL)
			return (false);
	}
	data->has_data = false;
	data->is_literal = false;
	skip_space(data);
	if (is_pipe(data->input[data->pos]))
	{
		//TODO ERROR cus there can't be a pipe right after another pipe
	}
	skip_space(data);
	data->start = data->pos;
	return (true);
}

static t_bool	func_1(t_parse_data *data, t_minishell *minishell)
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
			return (false);
	}
	return (true);
}

static t_bool	func(t_parse_data *data, t_list **head, t_minishell *minishell)
{
	while (data->input[data->pos])
	{
		if (data->input[data->pos] == '"' || data->input[data->pos] == '\'')
		{
			if (func_3(data, minishell) == false)
				return (false);
		}
		else if (ft_iswhite_space(data->input[data->pos])
			|| is_pipe(data->input[data->pos]))
		{
			if (func_2(data, head, minishell) == false)
				return (false);
		}
		else if (data->input[data->pos] == '$')
		{
			if (func_1(data, minishell) == false)
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

	data.pos = 0;
	data.input = input;
	data.has_data = false;
	data.is_literal = false;
	data.string = init_string(NULL);
	head = ft_calloc(1, sizeof(t_list *));
	if (data.string == NULL)
		return (NULL);
	while (ft_iswhite_space(input[data.pos]))
		data.pos++;
	data.start = data.pos;
	if (func(&data, head, minishell) == true)
		return (head);
	else
		return (NULL);
}
