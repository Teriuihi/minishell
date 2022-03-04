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

/**
 * Appends everything from input between start and pos
 *
 * @param	input	Input to append from
 * @param	start	Start pos in input
 * @param	pos		End in input (current pos)
 * @param	arg		String to append to
 *
 * @return	String we appended too (could have a different address now)
 */
t_string	*append_content(char *input, int start, int pos, t_string *arg)
{
	char		*to_add;
	t_string	*result;

	if (input == NULL)
	{
		free_string(arg);
		return (NULL);
	}
	to_add = ft_calloc((pos - start) + 2, sizeof(char));
	if (to_add == NULL)
	{
		free_string(arg);
		return (NULL);
	}
	ft_strlcpy(to_add, input + start, pos - start + 1);
	result = append_char(arg, to_add);
	if (result == NULL)
		free_string(arg);
	return (result);
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
			arg = append_content(input, *start, pos, arg);
			if (arg == NULL)
				return (NULL);
		}
		else
			pos++;
	}
	return (arg);
}

/**
 * Parse user input
 *
 * @param	input	Input to parse
 *
 * @return	Array of arguments the user entered
 */
t_arg	**parse(char *input)
{
	int			pos;
	int			start;
	t_string	*string;

	start = 0;
	pos = 0;
	string = init_string(NULL);
	while (input[pos])
	{
		if (input[pos] == '"')
			parse_quotation(input, &start, input[pos]);
			pos++;
	}
}