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
/**
 * Checks if a string ends with a specific suffix
 *
 * Destroys hashtable with all its entries
 *
 * @return	void
 */
void	free_key_value(t_entry *entry)
{
	if (entry->key != NULL)
	{
		free(entry->key);
	}
	if (entry->val != NULL)
	{
		free(entry->val);
	}
	ft_printf("%p is entry next\n", entry->next);
	entry = entry->next;
	ft_printf("%p is entry now\n", entry);

}

void	destroy_entry(t_entry *entry)
{
	if (entry != NULL)
	{
		free_key_value(entry);
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
