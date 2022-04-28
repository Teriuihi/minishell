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

#include "destroy.h"

t_bool	ft_remove_exported_var(char *key, t_hash_table *h_table,
								t_minishell *minishell)
{
	unsigned int	hashkey;
	t_entry			*current;
	t_entry			*prev;

	if (!key || !h_table || !minishell)
		return (new_set_exit_status(1, NULL));
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
			destroy_key_value(current);
			return (new_set_exit_status(0, NULL));
		}
		prev = current;
		current = current->next;
	}
	return (new_set_exit_status(0, NULL));
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

t_bool	print_h_table(t_hash_table *h_table, int len)
{
	int		i;
	t_entry	*curr;

	if (len != 1)
		return (new_set_exit_status(127, NULL));
	if (!h_table)
		return (false);
	i = 0;
	while (i < h_table->size)
	{
		curr = h_table->entries[i];
		while (curr != NULL)
		{
			if (curr->key && curr->val && curr->is_exported)
			{
				ft_printf(1, "%s=%s\n", curr->key, curr->val);
			}
			curr = curr->next;
		}
		i++;
	}
	return (true);
}
