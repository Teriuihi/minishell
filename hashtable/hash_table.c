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
//
extern char	**environ;

//export TERM=linux has to be added still

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

t_entry	*create_hash_table_pair(char *key, char *val, t_bool is_exported)
{
	t_entry	*entry;

	entry = (t_entry *)malloc(sizeof(t_entry));
	if (!entry)
		exit(1);
	entry->key = (char *)ft_calloc((ft_strlen(key) + 1), 1);
	entry->val = (char *)ft_calloc((ft_strlen(val) + 1), 1);
	if (entry->val == NULL || entry->key == NULL)
		exit(1);
	entry->key = ft_strncpy(entry->key, (char *)key, ft_strlen((char *)key));
	entry->val = ft_strncpy(entry->val, (char *)val, ft_strlen((char *)val));
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
		if (ft_strncmp(entry->key, key, ft_strlen(entry->key)) == 0)
		{
			if (entry->val == NULL)
			{
				entry->val = (char *)ft_calloc((ft_strlen(val) + 1), 1);
				entry->val = ft_strncpy(entry->val, (char *)val, ft_strlen((char *)val));
				if (!entry->val)
				{	
					exit(1);
				}
				return (true);
			}
			free(entry->val);
			entry->val = ft_strdup(val);
			return (true);
		}
		prev = entry;
		entry = prev->next;
	}
	//ft_printf("|%s| is prev -> next, %s is prev->key\n", key, prev->key);

	prev->next = create_hash_table_pair(key, val, is_exported);
//	ft_printf("%s is prev -> next\n", prev->next->key);
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
		//if (ft_strncmp(environ[i], "PATH", 4) == 0)
		//	ft_printf("%s is environs 0, %s is environs 1, %d is i\n", environs[0], environs[1], i);
		if (!environs)
			return (NULL);
		if (succesful_insert(h_table, environs[0], environs[1], true) == false)
			return (NULL);
		free(environs);
		i++;
		//ft_printf("%d is i in create env table\n", i);
	}
	if (ft_set_env("TERM", "linux", h_table, true) == false) //export TERM=linux has to be added still for clear, sometimes it doesnt work otherwise
		exit(1);
	return (h_table);
}
