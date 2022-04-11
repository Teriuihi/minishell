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
#include "../headers/functions.h"
#include "internal_parser.h"

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

	*success = false;
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
 * @param	data	Data used for parsing
 *
 * @return	They key's value or null if the key doesn't exist
 * 	set's success to false on failure
 */
static t_string	*get_string_from_var(t_parse_data *data)
{
	char		*key;
	t_string	*result;
	t_bool		success;

	if (data->pos == data->start)
		return (init_string(NULL));
	key = ft_calloc(data->pos - data->start + 1, sizeof(char));
	if (key == NULL)
		return (NULL);
	ft_strlcpy(key, data->input + data->start, data->pos - data->start + 1);
	result = get_value_from_key(key, &success);
	free(key);
	if (result == NULL)
		return (NULL);
	return (result);
}

/**
 * When the end of a string is reached this function parses the variable we were
 *	reading and ensures the pos and start are set past it
 *
 * @param	data		Data used for parsing
 * @param	minishell	Data for minishell
 *
 * @return	Boolean indicating success
 */
static t_bool	parse_env_variable_end_string(t_parse_data *data,
												t_minishell *minishell)
{
	t_string	*result;

	if (data->start == data->pos)
	{
		data->string = append_char_array(data->string, "$");
		if (data->string == NULL)
			return (set_exit_status(minishell, 1,
					"some shell: Out of memory."));
		return (true);
	}
	result = get_string_from_var(data);
	if (result == NULL)
		return (set_exit_status(minishell, 1, "some shell: Out of memory."));
	data->string = join_strings(data->string, result);
	if (data->string == NULL)
		return (set_exit_status(minishell, 1, "some shell: Out of memory."));
	return (true);
}

/**
 * When the end of a variable is reached this function parses it and ensures the
 * 	pos and start are set past it
 *
 * @param	data		Data used for parsing
 * @param	minishell	Data for minishell
 *
 * @return	Boolean indicating success
 */
static t_bool	handle_env_variable_in_string(t_parse_data *data,
												t_minishell *minishell)
{
	t_string	*result;

	if (data->pos == data->start && data->input[data->pos] == '?')
		data->pos++;
	else if (data->pos == data->start)
	{
		data->string = append_char_array(data->string, "$");
		if (data->string == NULL)
			return (set_exit_status(minishell, 1,
					"some shell: Out of memory."));
		return (true);
	}
	result = get_string_from_var(data);
	if (result == NULL)
		return (set_exit_status(minishell, 1, "some shell: Out of memory."));
	data->string = join_strings(data->string, result);
	if (data->string == NULL)
		return (set_exit_status(minishell, 1, "some shell: Out of memory."));
	return (true);
}

/**
 * Called when finding a $ indicating a variable should be parsed
 * Parse the variable and swap it with its value
 *
 * @param	data	A struct containing data related to current env variable
 *
 * @return	boolean to indicate success
 */
t_bool	parse_env_variable(t_parse_data *data, t_minishell *minishell)
{
	while (data->input[data->pos])
	{
		if (!ft_isalnum(data->input[data->pos]) && data->pos != '_')
		{
			return (handle_env_variable_in_string(data, minishell));
		}
		data->pos++;
	}
	return (parse_env_variable_end_string(data, minishell));
}
