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
		minishell->exit_status = 1;
		return (false);
	}
	hashkey = hash(key, "", h_table->size);
	while (h_table->entries[hashkey] != NULL)
	{
		if (ft_strncmp(key, h_table->entries[hashkey]->key,
				ft_strlen(key)) == 0)
		{
			free_key_value(h_table->entries[hashkey]);
			minishell->exit_status = 0;
			return (false);
		}
		h_table->entries[hashkey] = h_table->entries[hashkey]->next;
	}
	minishell->exit_status = 1;
	return (true);
}

void	ft_set_env(char *key, char *val, t_hash_table *h_table)
{
	t_bool			insert_succeeded;

	if (!key || !val  || !h_table)
	{
		return ;
	}
	insert_succeeded = succesful_insert(h_table, key, val);
	//free(key);
	
	//free(val);
}

char	*ft_get_env_val(char *key, t_hash_table *h_table)
{
	unsigned int	hashkey;
	char			*env_val;

	//if !key !table?
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
