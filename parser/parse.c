/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 17:33:00 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/28 17:33:00 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/arguments.h"
#include "internal_parser.h"

/**
 * Called when finding a $ indicating a variable should be parsed
 * Parse the variable and swap it with its value
 *
 * @param	input	Input from user
 * @param	start	Start pos (location of $)
 * @param	pos		Current pos while iterating over input
 * @param	arg		String containing the current argument that we're appending too
 *
 * @return	String we appended too (could have a different address now)
 */
t_string	*parse_env_variable(char *input, int start, int *pos, t_string *arg)
{
	char		*var;
	t_string	*result;

	while (input[*pos])
	{
		if (ft_iswhite_space(input[*pos]) || input[*pos] == '$'
			|| input[*pos] == '"' || input[*pos] == '\'')
		{
			var = ft_calloc(*pos - start, sizeof(char));
			ft_strlcpy(var, input + start, *pos - start);
			//get_var_from_str(var);
			result = init_string(var); //Should use the variable not the string
			free(var);
			return (join_strings(arg, result));
		}
		(*pos)++;
	}
	if (start == (*pos - 1))
		return (append_char(arg, "$"));
	var = ft_calloc(*pos - start, sizeof(char));
	if (var == NULL)
		return (NULL);
	ft_strlcpy(var, input + start, *pos - start);
	//get_var_from_str(var);
	result = init_string(var); //Should use the variable not the string
	free(var);
	return (join_strings(arg, result));
}

/** TODO handle " in ' and vice versa
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
			arg = append_content(input, *start, pos, arg);
			if (arg == NULL)
				return (NULL);
			arg = parse_env_variable(input, pos, &pos, arg);
			if (arg == NULL)
				return (NULL);
			*start = pos;
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
	return (arg); //todo add quote at the beginning
}

t_string	*safe_add_to_list(t_list **head, t_string *string, t_bool literal)
{
	if (add_to_list(head, string, literal) == false)
	{
		free_string(string);
		ft_lstclear(head, free_t_arg);
		return (NULL);
	}
	return (init_string(NULL));
}

/**
 * Parse user input
 *
 * @param	input	Input to parse
 *
 * @return	Array of arguments the user entered
 */
t_list	**parse(char *input)
{
	int			pos;
	int			start;
	t_string	*string;
	t_list		**head;
	t_bool		literal;

	literal = false;
	head = ft_calloc(1, sizeof(t_list *));
	pos = 0;
	start = 0;
	string = init_string(NULL);
	if (string == NULL)
		return (NULL);
	while (input[pos])
	{
		if (input[pos] == '"' || input[pos] == '\'')
		{
			if (pos != 0 && pos != start)
				string = append_content(input, start, pos - 1, string);
			pos++;
			parse_quotation(input, &pos, input[pos - 1], string);
			start = pos;
			literal = true;
		}
		else if (ft_iswhite_space(input[pos]))
		{
			if (pos == 0 || pos == start)
			{
				if (string->len != 0)
				{
					string = append_content(input, start, pos, string);
					string = safe_add_to_list(head, string, literal);
					if (string == NULL)
						return (NULL);
					literal = false;
				}
				pos++;
				start = pos;
				continue ;
			}
			string = append_content(input, start, pos, string);
			string = safe_add_to_list(head, string, literal);
			if (string == NULL)
				return (NULL);
			literal = false;
			pos++;
			start = pos;
		}
		else
			pos++;
	}
	if (pos != start || string->len != 0)
	{
		string = append_content(input, start, pos, string);
		if (safe_add_to_list(head, string, literal) == NULL)
			return (NULL);
	}
	return (head);
}
