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

extern char	**environ;

t_bool	ft_remove_exported_var(char *key, t_hash_table *h_table,
								t_minishell *minishell)
{
	unsigned int	hashkey;

	if (!key || !h_table || !minishell)
	{
		return (set_exit_status(minishell, 1, NULL));
	}
	hashkey = hash(key, "", h_table->size);
	while (h_table->entries[hashkey] != NULL)
	{
		if (ft_strncmp(key, h_table->entries[hashkey]->key,
				ft_strlen(key)) == 0)
		{
			free(h_table->entries[hashkey]->val);
			free(h_table->entries[hashkey]->key);
			//h_table->entries[hashkey]->val = ft_strdup("\n");
			return (set_exit_status(minishell, 0, NULL));
		}
		h_table->entries[hashkey] = h_table->entries[hashkey]->next;
	}
	return (set_exit_status(minishell, 0, NULL));
}

t_bool	ft_set_env(char *key, char *val, t_hash_table *h_table,
								t_bool is_exported)
{
	t_bool			insert_succeeded;

	insert_succeeded = false;
	if (!key || !h_table)
	{
		return (false);
	}
	insert_succeeded = succesful_insert(h_table, key, val, is_exported);
	return (insert_succeeded);
}

//CHECK THIS BECAUSE IT RETURNS INCORRECTLY
char	*ft_get_env_val(char *key, t_hash_table *h_table)
{
	unsigned int	slot;
	char			*env_val;
	t_entry			*current;

	if (!key || !h_table)
	{
		return (NULL);
	}
	slot = hash(key, "", h_table->size);
	current = h_table->entries[slot];
	while (current != NULL)
	{
		if (ft_strncmp(key, current->key,
				ft_strlen(key)) == 0)
		{
			env_val = ft_strdup(current->val);
			return (env_val);
		}
		current = current->next;
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
	envp = (char **)ft_calloc(h_table->size + 1, sizeof(char *));
	if (!envp)
	{
		return (NULL);
	}
	envp[h_table->size] = NULL;
	i = 0;
	env_i = 0;
	while (i < h_table->size)
	{
		curr = h_table->entries[i];
		if (curr != NULL)
		{
			while (curr != NULL)
			{
				current_env = (char *)malloc((ft_strlen(curr->key)
							+ ft_strlen(curr->val) + 2) * sizeof(char));
				if (!current_env)
					exit(1);
				if (curr->key)
					current_env = ft_strjoin(curr->key, "=");
				if (curr->val)
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
