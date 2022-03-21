/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   manipulate_env.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 17:59:28 by sappunn       #+#    #+#                 */
/*   Updated: 2021/12/10 17:59:28 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/functions.h"
#include "../headers/structs.h"
#include "../libft/libft.h"

/**
 * Manipulates env in the hashtable
*
*/

extern char	**environ;

t_bool	ft_remove_exported_var(char *key, t_hash_table *h_table, t_minishell *minishell)
{
	unsigned int	hashkey;

	if (!key || !h_table || !minishell)
	{
		return (set_exit_status(minishell, 1)); //All builtins return an exit status of 2 to indicate incorrect usage, generally invalid options or missing arguments.
	}
	hashkey = hash(key, "", h_table->size); //no need for val here, hashcode being calculated without it
	while (h_table->entries[hashkey] != NULL)
	{
		if (ft_strncmp(key, h_table->entries[hashkey]->key,
				ft_strlen(key)) == 0)
		{
			free_key_value(h_table->entries[hashkey]);
			return (set_exit_status(minishell, 0)); //All builtins return an exit status of 2 to indicate incorrect usage, generally invalid options or missing arguments.
		}
		h_table->entries[hashkey] = h_table->entries[hashkey]->next;
	}
	return (set_exit_status(minishell, 0)); //All builtins return an exit status of 2 to indicate incorrect usage, generally invalid options or missing arguments.
}

t_bool	ft_set_env(char *key, char *val, t_hash_table *h_table, t_bool is_exported)
{
	t_bool			insert_succeeded;

	if (!key || !val  || !h_table)
	{
		return (false);
	}
	insert_succeeded = succesful_insert(h_table, key, val, is_exported);
	return (insert_succeeded);
}

char	*ft_get_env_val(char *key, t_hash_table *h_table)
{
	unsigned int	hashkey;
	char			*env_val;

	if (!key || !h_table)
	{
		return (NULL);
	}
	hashkey = hash(key, "", h_table->size);
	while (h_table->entries[hashkey] != NULL)
	{
		if (ft_strncmp(key, h_table->entries[hashkey]->key,
				ft_strlen(key)) == 0)
		{
			env_val = ft_strdup(h_table->entries[hashkey]->val);
			return (env_val);
		}
		h_table->entries[hashkey] = h_table->entries[hashkey]->next;
	}
	return (NULL);
}

char	**get_envp(t_hash_table *h_table)
{
	t_entry	*curr;
	char	*current_env;
	char	**envp;
	int		env_i;
	int		i;

	if (!h_table)
	{
		return (NULL);
	}
	envp = (char **)ft_calloc(h_table->size, sizeof(char *));
	if (!envp)
	{
		return (NULL);
	}
	i = 0;
	env_i = 0;
	while (i < h_table->size)
	{
		curr = h_table->entries[i];
		if (curr != NULL)
		{
			while (curr != NULL)
			{
				current_env = (char *)malloc((ft_strlen(curr->key) + ft_strlen(curr->val) + 2) * sizeof(char));
				if (!current_env)
				{
					return (NULL);
				}
				current_env = ft_strjoin(curr->key, "=");
				current_env = ft_strjoin(current_env, curr->val);
				envp[env_i] = current_env;
				env_i++;
				curr = curr->next;
			}
		}
		i++;
	}
	return (envp);
}
