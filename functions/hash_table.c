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
/**
 * Creates a hashtable where we will store env and other variables
*/

t_hash_table	*init_hash_table(int size)
{
	t_hash_table	*hash_table;
	int				i;

	hash_table = (t_hash_table *)malloc(sizeof(t_hash_table));
	if (!hash_table)
		return (NULL);
	hash_table->entries = (t_entry **)malloc(size * sizeof(t_entry *));
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

t_entry	*create_hash_table_pair(char *key, char *val)
{
	t_entry	*entry;

	entry = (t_entry *)malloc(sizeof(t_entry));
	if (!entry)
		return (NULL);
	entry->key = (char *)ft_calloc((ft_strlen(key) + 1), 1); //key is path
	if (!entry->key)
	{
		free(entry);
		entry = NULL;
	}
	entry->val = (char *)ft_calloc((ft_strlen(val) + 1), 1);
	entry->key = ft_strncpy(entry->key, (char *)key, ft_strlen((char *)key));
	entry->next = NULL;
	entry->val = ft_strncpy(entry->val, (char *)val, ft_strlen((char *)val));
	return (entry);
}

t_bool	succesful_insert(t_hash_table *h_table, char *key, char *val)
{
	unsigned int	slot;
	t_entry			*entry;
	t_entry			*prev;

	slot = hash(key, val, h_table->size);
	entry = h_table->entries[slot];
	if (entry == NULL)
	{
		h_table->entries[slot] = create_hash_table_pair(key, val);
		if (!h_table->entries[slot])
			return (false);
		return (true);
	}
	while (entry != NULL)
	{
		if (ft_strncmp(entry->key, key, ft_strlen(entry->key)) == 0)
			return (false);
		prev = entry;
		entry = prev->next;
	}
	prev->next = create_hash_table_pair(key, val);
	if (!prev->next)
		return (false);
	return (true);
}

t_bool	all_args_inserted(t_hash_table *h_table, char *key, char *val)
{
	if (succesful_insert(h_table, key, val) == false
		|| ft_strncmp("", key, 1) == 0)
	{
		return (false);
	}
	return (true);
}

t_hash_table	*duplicates_are_found_in_argv(void)
{
	t_hash_table	*h_table;
	char			**environs;
	int				i;
	int				size;

	size = 0;
	i = 0;
	while (environ[size])
	{
		size++;
	}
	h_table = init_hash_table(size);
	if (h_table == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (environ[i])
	{
		environs = ft_split(environ[i], '=');
		if (!environs[0] || !environs[1])
			return (h_table); //TODO might need to be NULL instead?
		if (succesful_insert(h_table, environs[0], environs[1]) == false)
		{
			return (NULL);
		}
		free(environs);
		i++;
	}
	return (h_table);
}
