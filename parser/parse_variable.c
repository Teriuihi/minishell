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
#include "parser.h"
/**
 * Get the value from a key from the hash table
 *
 * @param	key			Key to get the value for
 * @param	success		Boolean to mark if there was an error
 * @param	minishell	Data for minishell
 *
 * @return	Value or null if key wasn't in the hashtable
 */
static t_string	*get_value_from_key(char *key, t_bool *success,
					t_minishell *minishell)
{
	char			*value;
	t_string		*result;
	t_hash_table	*table;

	*success = false;
	table = minishell->env;
	if (ft_strlen(key) == 1 && *key == '?')
	{
		value = ft_itoa(g_signal.exit_status);
		if (value == NULL)
			return (NULL);
	}
	else
	{
		value = ft_get_env_val(key, table, success);
		if (*success == false)
			return (NULL);
	}
	result = init_string(value);
	free(value);
	*success = true;
	return (result);
}

/**
 * Gets the value of a variable
 *
 * @param	data	Data used for parsing
 * @param	minishell	Data for minishell
 *
 * @return	They key's value or null if the key doesn't exist
 * 	set's success to false on failure
 */
static t_string	*get_string_from_var(t_parse_data *data, t_minishell *minishell)
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
	result = get_value_from_key(key, &success, minishell);
	free(key);
	if (result == NULL)
		return (NULL);
	return (result);
}

/**
 * When the end of a variable is reached this function parses it and ensures the
 * 	pos and start are set past it
 *
 * @param	data	Data used for parsing
 * @param	head	List containing previous arguments
 * @param	minishell	Data for minishell
 *
 * @return	Boolean indicating success
 */
static t_bool	handle_env_variable_in_string_prt2(t_parse_data *data,
					t_list **head, t_minishell *minishell)
{
	t_string	*result;
	t_list		*tmp;

	tmp = ft_lstlast(*head);
	if (tmp != NULL && pipe_type_from_arg(tmp->content) == DELIMITER_INPUT)
	{
		data->start--;
		if (append_content(data) == false)
			return (false);
		return (true);
	}
	else
		result = get_string_from_var(data, minishell);
	if (result == NULL)
		return (new_set_exit_status(1, "some shell: Out of memory.\n"));
	data->string = join_strings(data->string, result);
	if (data->string == NULL)
		return (new_set_exit_status(1, "some shell: Out of memory.\n"));
	return (true);
}

/**
 * When the end of a variable is reached this function parses it and ensures the
 * 	pos and start are set past it
 *
 * @param	data	Data used for parsing
 * @param	head	List containing previous arguments
 * @param	minishell	Data for minishell
 *
 * @return	Boolean indicating success
 */
static t_bool	handle_env_variable_in_string(t_parse_data *data, t_list **head,
					t_minishell *minishell)
{
	if (data->pos == data->start && data->input[data->pos] == '?')
		data->pos++;
	else if (data->pos == data->start)
	{
		data->string = append_char_array(data->string, "$");
		if (data->string == NULL)
			return (new_set_exit_status(1, "some shell: Out of memory.\n"));
		return (true);
	}
	return (handle_env_variable_in_string_prt2(data, head, minishell));
}

/**
 * Called when finding a $ indicating a variable should be parsed
 * Parse the variable and swap it with its value
 *
 * @param	data	A struct containing data related to current env variable
 * @param	head	List containing previous arguments
 *
 * @return	boolean to indicate success
 */
t_bool	parse_env_variable(t_parse_data *data, t_list **head,
			t_minishell *minishell)
{
	while (data->input[data->pos])
	{
		if (!ft_isalnum(data->input[data->pos]) && data->pos != '_')
		{
			return (handle_env_variable_in_string(data, head, minishell));
		}
		data->pos++;
	}
	return (handle_env_variable_in_string(data, head, minishell));
}
