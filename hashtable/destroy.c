/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_destroy.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 17:59:28 by sappunn       #+#    #+#                 */
/*   Updated: 2021/12/10 17:59:28 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/functions.h"
#include "destroy.h"

/**
 * Checks if a string ends with a specific suffix
 *
 * Destroys hashtable with all its entries
 *
 * @return	void
 */
void	destroy_key_value(t_entry *entry)
{
	if (entry == NULL)
	{
		return ;
	}
	if (entry->key != NULL)
	{
		free(entry->key);
		entry->key = NULL;
	}
	if (entry->val != NULL)
	{
		free(entry->val);
		entry->val = NULL;
	}
}

void	destroy_entry(t_entry *entry)
{
	if (entry != NULL)
	{
		destroy_key_value(entry);
		free(entry);
		entry = NULL;
	}
}

void	destroy_hash_table(t_hash_table *hash_table, int size)
{
	t_entry	*current;
	t_entry	*next;
	int		i;

	i = 0;
	if (!hash_table)
		return ;
	while (i < size)
	{
		current = hash_table->entries[i];
		while (current != NULL)
		{
			next = current->next;
			destroy_entry(current);
			current = next;
		}
		i++;
	}
	free(hash_table->entries);
	hash_table->entries = NULL;
	free(hash_table);
	hash_table = NULL;
}
