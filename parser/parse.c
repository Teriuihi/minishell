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
#include "../hashtable/hashtable.h"
#include "../headers/minishell.h"

/** TODO FINISH COMMENTS
 * Does not need a null check for value cus NULL is valid
 * @param pos
 * @param start
 * @param input
 * @return
 */
t_string	*get_string_from_var(int pos, int start, char *input, t_minishell *minishell)
{
	char			*key;
	char			*value;
	t_hash_table	*table;
	t_string		*result;

	if (pos == start)
		return (init_string(NULL));
	table = get_hash_table();
	key = ft_calloc(pos - start + 1, sizeof(char));
	if (key == NULL)
		return (NULL);
	ft_strlcpy(key, input + start, pos - start + 1);
	if (ft_strlen(key) == 1 && *key == '?')
	{
		value = ft_itoa(g_signal.exit_status); //TODO NULL check?
		ft_printf("%s is the exit value which we get here\n", value);
	}
	else
		value = ft_get_env_val(key, table);
	free(key);
	result = init_string(value);
	free(value);
	return (result);
}

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
t_string	*parse_env_variable(char *input, int start, int *pos, t_string *arg, t_minishell *minishell)
{
	t_string	*result;

	while (input[*pos])
	{
		if (!ft_isalnum(input[*pos]) && *pos != '_')
		{
			if (*pos == start && input[*pos] == '?')
				(*pos)++;
			result = get_string_from_var(*pos, start, input, minishell);
			if (result == NULL)
				return (NULL);
			return (join_strings(arg, result));
		}
		(*pos)++;
	}
	if (start == *pos)
		return (append_char(arg, "$"));
	result = get_string_from_var(*pos, start, input, minishell);
	if (result == NULL)
		return (NULL);
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
t_string	*parse_quotation(char *input, int *start, char quote, t_string *arg, t_minishell *minishell)
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
			arg = parse_env_variable(input, ++pos, &pos, arg, minishell);
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

t_bool	is_pipe(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

void	skip_space(char *input, int *pos)
{
	while (*pos && ft_iswhite_space(input[*pos]))
		(*pos)++;
}

t_bool	pipe_in_string_skip(char *input, int start, int *pos, t_string **string)
{
	int	len;

	if (ft_strlen(input + *pos) < 2)
		return (false); //Parsing error since it has to be at least 2 chars (ex: <a)
	if (!ft_strncmp(input + *pos, "<<", 2) || !ft_strncmp(input + *pos, ">>", 2))
	{
		if (!input[*pos + 2] || is_pipe(input[*pos + 2]))
			return (false); //Parsing error since it has to be at least 3 chars (ex: >>a) and there can't be another pipe after
		len = 2;
	}
	else
	{
		if (is_pipe(input[*pos + 1]))
			return (false); //Parsing error since there can't be a pipe after
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
	start = 0;
	string = init_string(NULL);
	if (string == NULL)
		return (NULL);
	while (input[pos])
	{
		if (input[pos] == '"' || input[pos] == '\'')
		{
			if (pos != 0 && pos != start)
				string = append_content(input, start, pos, string);
			pos++;
			string = parse_quotation(input, &pos, input[pos - 1], string, minishell);
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
				pipe_in_string_skip(input, pos, &pos, &string);
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
			string = parse_env_variable(input, ++pos, &pos, string, minishell);
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
