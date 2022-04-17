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

extern char	**environ;

t_hash_table	*init_hash_table(int size)
{
	t_hash_table	*hash_table;
	int				i;

	hash_table = (t_hash_table *)ft_calloc(1, sizeof(t_hash_table));
	if (!hash_table)
		exit(1);
	hash_table->entries = (t_entry **)ft_calloc(size, sizeof(t_entry *));
	if (!hash_table->entries)
	{
		free(hash_table);
		hash_table = NULL;
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
		exit(1);
	if (key != NULL)
	{
		entry->key = ft_strdup(key);
		if (entry->key == NULL)
			exit(1);
	}
	else
		entry->key = NULL;
	if (val != NULL)
	{
		entry->val = ft_strdup(val);
		if (entry->val == NULL)
			exit(1);
	}
	else
		entry->val = NULL;
	entry->is_exported = is_exported;
	entry->next = NULL;
	return (entry);
}

t_bool	succesful_insert(t_hash_table *h_table, char *key, char *val,
						t_bool is_exported)
{
	unsigned int	slot;
	t_entry			*entry;
	t_entry			*prev;
	int				i;

	i = 0;
	slot = hash(key, "", h_table->size);
	entry = h_table->entries[slot];
	if (entry == NULL)
	{
		h_table->entries[slot] = create_hash_table_pair(key, val, is_exported);
		if (!h_table->entries[slot])
			return (false);
		return (true);
	}
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
			return (true);
		}
		prev = entry;
		entry = prev->next;
		i++;
	}
	prev->next = create_hash_table_pair(key, val, is_exported);
	if (!prev->next)
		return (false);
	return (true);
}

t_hash_table	*create_env_h_table(void)
{
	t_hash_table	*h_table;
	char			**environs;
	int				i;
	int				size;

	size = 0;
	i = 0;
	while (environ[size])
		size++;
	h_table = init_hash_table(size);
	if (h_table == NULL)
		exit(1);
	i = 0;
	while (environ[i])
	{
		environs = ft_split(environ[i], '=');
		if (!environs)
			return (NULL);
		if (succesful_insert(h_table, environs[0], environs[1], true) == false)
			return (NULL);
		free(environs);
		i++;
	}
	return (h_table);
}

t_bool	print_h_table(t_hash_table *h_table)
{
	int		i;
	t_entry	*curr;

	if (!h_table)
	{
		return (false);
	}
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
