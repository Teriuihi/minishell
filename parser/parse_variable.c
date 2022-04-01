/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_variable.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/01 19:02:44 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/01 19:02:44 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/arguments.h"
#include "../hashtable/hashtable.h"
#include "../headers/minishell.h"

/**
 * Get the value from a key from the hash table
 *
 * @param	key		Key to get the value for
 * @param	success	Boolean to mark if there was an error
 *
 * @return	Value or null if key wasn't in the hashtable
 */
static t_string	*get_value_from_key(char *key, t_bool *success)
{
	char			*value;
	t_string		*result;
	t_hash_table	*table;

	table = get_hash_table();
	if (ft_strlen(key) == 1 && *key == '?')
	{
		value = ft_itoa(g_signal.exit_status);
		if (value == NULL)
			return (NULL);
	}
	else
		value = ft_get_env_val(key, table); //TODO add boolean flag?
	result = init_string(value);
	free(value);
	*success = true;
	return (result);
}

/**
 * Gets the value of a variable
 *
 * @param	end		End of key
 * @param	start	Start of key
 * @param	input	String containing key
 *
 * @return	They key's value or null if the key doesn't exist
 * 	set's success to false on failure
 */
static t_string	*get_string_from_var(int end, int start, char *input,
				t_bool *success)
{
	char		*key;
	t_string	*result;

	*success = false;
	if (end == start)
		return (init_string(NULL));
	key = ft_calloc(end - start + 1, sizeof(char));
	if (key == NULL)
		return (NULL);
	ft_strlcpy(key, input + start, end - start + 1);
	result = get_value_from_key(key, success);
	free(key);
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
t_string	*parse_env_variable(char *input, int start, int *pos, t_string *arg)
{
	t_string	*result;
	t_bool		success;

	while (input[*pos])
	{
		if (!ft_isalnum(input[*pos]) && *pos != '_')
		{
			if (*pos == start && input[*pos] == '?')
				(*pos)++;
			else if (*pos == start)
				return (append_char(arg, "$"));
			result = get_string_from_var(*pos, start, input, &success);
			if (result == NULL)
				return (NULL);
			return (join_strings(arg, result));
		}
		(*pos)++;
	}
	if (start == *pos)
		return (append_char(arg, "$"));
	result = get_string_from_var(*pos, start, input, &success);
	if (result == NULL)
		return (NULL);
	return (join_strings(arg, result));
}
