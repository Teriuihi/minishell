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

extern char	**environ;

t_bool	ft_remove_exported_var(char *key, t_hash_table *h_table,
								t_minishell *minishell)
{
	unsigned int	hashkey;
	t_entry			*current;
	t_entry			*prev;

	if (!key || !h_table || !minishell)
		return (set_exit_status(minishell, 1, NULL, false));
	hashkey = hash(key, "", h_table->size);
	current = h_table->entries[hashkey];
	prev = NULL;
	while (current != NULL)
	{
		if (ft_strlen(key) == ft_strlen(current->key)
			&& ft_strncmp(key, current->key, ft_strlen(key)) == 0)
		{
			if (prev == NULL)
				h_table->entries[hashkey] = current->next;
			else
				prev->next = current->next;
			free_key_value(current);
			return (set_exit_status(minishell, 0, NULL, false));
		}
		prev = current;
		current = current->next;
	}
	return (set_exit_status(minishell, 0, NULL, false));
}

t_bool	ft_set_env(char *key, char *val, t_hash_table *h_table,
								t_bool is_exported)
{
	if (!key || !h_table)
		return (false);
	return (succesful_insert(h_table, key, val, is_exported));
}

char	*ft_get_env_val(char *key, t_hash_table *h_table, t_bool *success)
{
	unsigned int	slot;
	char			*env_val;
	t_entry			*current;

	*success = false;
	if (!key || !h_table)
		return (NULL);
	*success = true;
	slot = hash(key, "", h_table->size);
	current = h_table->entries[slot];
	while (current != NULL)
	{
		if (current->key != NULL)
		{
			if (ft_strncmp(key, current->key,
					ft_strlen(key)) == 0)
			{
				env_val = ft_strdup(current->val);
				return (env_val);
			}
		}
		current = current->next;
	}
	return (NULL);
}

char	*ft_get_exported_env(char *key, t_hash_table *h_table, t_bool *success)
{
	unsigned int	slot;
	char			*env_val;
	t_entry			*current;

	*success = false;
	if (!key || !h_table)
		return (NULL);
	*success = true;
	slot = hash(key, "", h_table->size);
	current = h_table->entries[slot];
	while (current != NULL)
	{
		if (current->key != NULL && current->is_exported == true)
		{
			if (ft_strncmp(key, current->key,
				ft_strlen(key)) == 0)
			{
				env_val = ft_strdup(current->val);
				return (env_val);
			}
		}
		current = current->next;
	}
	return (NULL);
}

static char	**loop_and_concat(char **envp, t_hash_table *h_table)
{
	int		i;
	char	*current_env;
	size_t	key_size;
	size_t	val_size;
	t_entry	*curr;

	i = 0;
	while (i < h_table->size)
	{
		curr = h_table->entries[i];
		while (curr != NULL)
		{
			key_size = ft_strlen(curr->key);
			val_size = ft_strlen(curr->val);
			current_env = (char *)ft_calloc(key_size
					+ val_size + 2, sizeof(char));
			if (!current_env)
				return (NULL);
			if (curr->key)
			{	
				ft_strlcpy(current_env, curr->key, ft_strlen(curr->key) + 1);
				ft_strlcpy(current_env + ft_strlen(curr->key), "=", 2);
			}
			if (curr->val)
			{
				ft_strlcpy(current_env + ft_strlen(curr->key) + 1, curr->val,
					ft_strlen(curr->val) + 1);
			}
			*envp = current_env;
			envp++;
			curr = curr->next;
		}
		i++;
	}
    return (envp);
}

char	**get_envp(t_hash_table *h_table)
{
	t_entry	*curr;
	int		i;
	int		current_size;
	char	*current_env;
	char	**envp;

	if (!h_table)
		return (NULL);
	i = 0;
	current_size = 0;
	while (i < h_table->size)
	{
		curr = h_table->entries[i];
		while (curr != NULL)
		{
			current_size++;
			curr = curr->next;
		}
		i++;
	}
	envp = (char **)ft_calloc(current_size + 1, sizeof(char *));
	if (!envp)
		return (NULL);
	if (loop_and_concat(envp, h_table) == NULL)
		free(envp);
	return (envp);
}
