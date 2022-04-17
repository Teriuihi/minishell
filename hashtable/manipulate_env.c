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

//CHECK THIS BECAUSE IT RETURNS INCORRECTLY
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

/* var1=hi var2=hoi var3=hello, clear doesnt yet work */
static void	loop_and_concat(char **envp, t_hash_table *h_table)
{
	int		i;
	int		env_i;
	char	*current_env;
	t_entry	*curr;

	i = 0;
	env_i = 0;
	while (i < h_table->size)
	{
		curr = h_table->entries[i];
		while (curr != NULL)
		{
			current_env = (char *)ft_calloc((ft_strlen(curr->key)
						+ ft_strlen(curr->val) + 2), sizeof(char));
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
		i++;
	}
}

char	**get_envp(t_hash_table *h_table)
{
	t_entry	*curr;
	char	*current_env;
	char	**envp;

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
	loop_and_concat(envp, h_table);
	return (envp);
}
