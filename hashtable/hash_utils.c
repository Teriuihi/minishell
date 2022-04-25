/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   hash-utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 17:59:28 by sappunn       #+#    #+#                 */
/*   Updated: 2021/12/10 17:59:28 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/functions.h"
#include "get_envp.h"
#include "hash_utils.h"

t_bool	create_key_val(t_entry *entry, char *key, char *val)
{
	if (key != NULL)
	{
		entry->key = ft_strdup(key);
		if (entry->key == NULL)
		{
			free(entry);
			return (false);
		}
	}
	else
		entry->key = NULL;
	if (val != NULL)
	{
		entry->val = ft_strdup(val);
		if (entry->val == NULL)
		{
			destroy_entry(entry);
			return (false);
		}
	}
	else
		entry->val = NULL;
	return (true);
}

/**
 * Takes an array of arrays (user input)
 * Selects the function to execute based on the first argument (args[0])
 * @param	args	the input given by the user
 *
 * @return	void
 */

static t_bool	export_found(t_command *command, t_minishell *minishell)
{
	int		i;

	i = 0;
	if (!command->args || !command->args[i] || command->args_len <= 1)
	{
		return (false);
	}
	command->export_found = false;
	while (command->args[i] != NULL)
	{
		if (!ft_streq(command->args[i], "export"))
		{
			i++;
			continue ;
		}
		if (splitter(&i, command, minishell) == false)
			return (false);
		return (true);
	}
	return (false);
}

static t_bool	assign_new_val(char *env_val, t_minishell *minishell,
				char **splitted)
{
	free(env_val);
	if (succesful_insert(minishell->env, splitted[0], splitted[1], true)
		== true)
	{
		free_splitted(splitted);
		return (new_set_exit_status(0, NULL));
	}
	else
	{
		free_splitted(splitted);
		return (new_set_exit_status(1, NULL));
	}
}

t_bool	env_var_added(t_command *command, t_minishell *minishell)
{
	char	**splitted;
	t_bool	success;
	char	*env_val;

	if (!command || !minishell)
		return (new_set_exit_status(2, NULL));
	if (command->export_found == true)
		return (export_found(command, minishell));
	splitted = ft_split_first(command->command, '=');
	if (!splitted)
		return (new_set_exit_status(1, NULL));
	env_val = ft_get_env_val(splitted[0], minishell->env, &success);
	if (env_val != NULL)
		return (assign_new_val(env_val, minishell, splitted));
	else if (ft_set_env(splitted[0], splitted[1], minishell->env, false)
		== false)
	{
		free_splitted(splitted);
		return (new_set_exit_status(1, NULL));
	}
	free_splitted(splitted);
	return (new_set_exit_status(0, NULL));
}