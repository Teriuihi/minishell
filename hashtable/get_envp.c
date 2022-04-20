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

#include "get_envp.h"

static char	*contact_curr_entry(t_entry *curr)
{
	size_t	key_size;
	size_t	val_size;
	char	*current_env;

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
	return (current_env);
}

static char	**loop_and_concat(char **envp, t_hash_table *h_table)
{
	int		i;
	char	*current_env;
	t_entry	*curr;

	i = 0;
	while (i < h_table->size)
	{
		curr = h_table->entries[i];
		while (curr != NULL)
		{
			current_env = contact_curr_entry(curr);
			if (current_env == NULL)
				return (NULL);
			*envp = current_env;
			envp++;
			curr = curr->next;
		}
		i++;
	}
	return (envp);
}

char	**get_envp(t_hash_table *h_table, int i, int current_size)
{
	t_entry	*curr;
	char	**envp;

	if (!h_table)
		return (NULL);
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
	{
		free(envp);
		return (NULL);
	}
	return (envp);
}
