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
#include "../hashtable/hashtable.h"
#include "../headers/minishell.h"

static t_bool	is_pipe(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static void	skip_space(char *input, int *pos)
{
	while (*pos && ft_iswhite_space(input[*pos]))
		(*pos)++;
}

t_bool	pipe_in_string_skip(char *input, int start, int *pos, t_string **string)
{
	int	len;

	if (ft_strlen(input + *pos) < 2)
		return (false); //TODO Parsing error since it has to be at least 2 chars (ex: <a)
	if (!ft_strncmp(input + *pos, "<<", 2) || !ft_strncmp(input + *pos, ">>", 2))
	{
		if (!input[*pos + 2] || is_pipe(input[*pos + 2]))
			return (false); //TODO Parsing error since it has to be at least 3 chars (ex: >>a) and there can't be another pipe after
		len = 2;
	}
	else
	{
		if (is_pipe(input[*pos + 1]))
			return (false); //TODO Parsing error since there can't be a pipe after
		len = 1;
	}
	*pos += len;
	*string = append_content(input, start, *pos, *string);
	skip_space(input, pos);
	return (true);
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
	int			pos;
	int			start;
	t_string	*string;
	t_list		**head;
	t_bool		literal;
	t_bool		has_data;

	has_data = false;
	literal = false;
	head = ft_calloc(1, sizeof(t_list *));
	pos = 0;
	string = init_string(NULL);
	if (string == NULL)
		return (NULL);
	while (ft_iswhite_space(input[pos]))
		pos++;
	start = pos;
	while (input[pos])
	{
		if (input[pos] == '"' || input[pos] == '\'')
		{
			if (pos != 0 && pos != start)
				string = append_content(input, start, pos, string);
			pos++;
			string = parse_quotation(input, &pos, input[pos - 1], string);
			has_data = true;
			start = pos;
			literal = true;
		}
		else if (ft_iswhite_space(input[pos]) || is_pipe(input[pos]))
		{
			if (has_data)
			{
				string = append_content(input, start, pos, string);
				string = safe_add_to_list(head, string, literal);
				if (string == NULL)
					return (NULL);
			}
			skip_space(input, &pos);
			if (is_pipe(input[pos]))
			{
				if (pipe_in_string_skip(input, pos, &pos, &string) == false)
				{
					//TODO parsing error
				}
				string = safe_add_to_list(head, string, literal);
				if (string == NULL)
					return (NULL);
			}
			has_data = false;
			literal = false;
			skip_space(input, &pos);
			if (is_pipe(input[pos]))
			{
				//TODO ERROR cus there can't be a pipe right after another pipe
			}
			skip_space(input, &pos);
			start = pos;
		}
		else if (input[pos] == '$')
		{
			if (pos != 0 && pos != start)
				string = append_content(input, start, pos, string);
			if (string == NULL)
				return (NULL);
			string = parse_env_variable(input, ++pos, &pos, string);
			if (string == NULL)
				return (NULL);
			start = pos;
			has_data = true;
		}
		else
		{
			pos++;
			has_data = true;
		}
	}
	if (has_data)
	{
		string = append_content(input, start, pos, string);
		if (safe_add_to_list(head, string, literal) == NULL)
			return (NULL);
	}
	return (head);
}
