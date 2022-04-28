/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   hash_table.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 17:59:28 by sappunn       #+#    #+#                 */
/*   Updated: 2021/12/10 17:59:28 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/structs.h"
#include "../headers/functions.h"
#include "hash_utils.h"

t_hash_table	*init_hash_table(int size)
{
	t_hash_table	*hash_table;
	int				i;

	hash_table = (t_hash_table *)ft_calloc(1, sizeof(t_hash_table));
	if (!hash_table)
		return (NULL);
	hash_table->entries = (t_entry **)ft_calloc(size, sizeof(t_entry *));
	if (!hash_table->entries)
	{
		free(hash_table);
		return (NULL);
	}
	hash_table->size = size;
	i = 0;
	while (i < size)
	{
		hash_table->entries[i] = NULL;
		i++;
	}
	return (hash_table);
}

t_entry	*create_hash_table_pair(char *key, char *val, t_bool is_exported)
{
	t_entry	*entry;

	entry = (t_entry *)ft_calloc(1, sizeof(t_entry));
	if (!entry)
		return (NULL);
	if (create_key_val(entry, key, val) == false)
		return (NULL);
	entry->is_exported = is_exported;
	entry->next = NULL;
	return (entry);
}

static t_entry	*overwrite_if_exists_else_get_last(t_entry *entry,
					t_bool is_exported, char *key, char *val)
{
	t_entry	*prev;
	int		i;

	prev = NULL;
	i = 0;
	while (entry != NULL)
	{
		if (ft_strlen(entry->key) == ft_strlen(key)
			&& ft_strncmp(entry->key, key, ft_strlen(entry->key)) == 0)
		{
			if (entry->val != NULL)
				free(entry->val);
			entry->val = ft_strdup(val);
			if (!entry->is_exported)
				entry->is_exported = is_exported;
			return (NULL);
		}
		prev = entry;
		entry = prev->next;
		i++;
	}
	return (prev);
}

t_bool	succesful_insert(t_hash_table *h_table, char *key, char *val,
			t_bool is_exported)
{
	unsigned int	slot;
	t_entry			*entry;

	slot = hash(key, "", h_table->size);
	entry = h_table->entries[slot];
	if (entry == NULL)
	{
		h_table->entries[slot] = create_hash_table_pair(key, val, is_exported);
		if (!h_table->entries[slot])
			return (false);
		return (true);
	}
	entry = overwrite_if_exists_else_get_last(entry, is_exported, key, val);
	if (entry == NULL)
		return (true);
	entry->next = create_hash_table_pair(key, val, is_exported);
	if (!entry->next)
		return (false);
	return (true);
}

t_hash_table	*create_env_h_table(char **envp)
{
	t_hash_table	*h_table;
	char			**environs;
	int				i;
	int				size;

	size = 0;
	i = 0;
	while (envp[size])
		size++;
	h_table = init_hash_table(size);
	if (h_table == NULL)
		return (NULL);
	while (envp[i])
	{
		environs = ft_split(envp[i], '=');
		if (!environs)
			return (NULL);
		if (succesful_insert(h_table, environs[0], environs[1], true) == false)
			return (NULL);
		free_splitted(environs);
		i++;
	}
	return (h_table);
}
