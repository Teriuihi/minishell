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
#include "../headers/structs.h"
#include "../libft/libft.h"

/**
 * Manipulates env in the hashtable
*
*/

extern char	**environ;

void	ft_remove_env(char *key, t_hash_table *h_table)
{
	unsigned int	hashkey;

	hashkey = hash(key, "", h_table->size);
	while (h_table->entries[hashkey] != NULL)
	{
		if (ft_strncmp(key, h_table->entries[hashkey]->key,
				ft_strlen(key)) == 0)
		{
			free_key_value(h_table->entries[hashkey]);
			return ;
		}
		h_table->entries[hashkey] = h_table->entries[hashkey]->next;
	}
}

void	ft_set_env(char *key, char *val, t_hash_table *h_table)
{
	t_bool			insert_succeeded;

	insert_succeeded = succesful_insert(h_table, key, val);
}

char	*ft_get_env_val(char *key, t_hash_table *h_table)
{
	unsigned int	hashkey;
	char			*env_val;

	hashkey = hash(key, "", h_table->size);
	while (h_table->entries[hashkey] != NULL)
	{
		if (ft_strncmp(key, h_table->entries[hashkey]->key,
				ft_strlen(key)) == 0)
		{
			env_val = ft_strdup(h_table->entries[hashkey]->val);
			return (env_val);
		}
		h_table->entries[hashkey] = h_table->entries[hashkey]->next;
	}
	return (NULL);
}
