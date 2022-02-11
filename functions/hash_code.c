/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_hash_code.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 17:59:28 by sappunn       #+#    #+#                 */
/*   Updated: 2021/12/10 17:59:28 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/structs.h"
/**
 * Checks if a string ends with a specific suffix
 *
 * @param	key   key part of the hashtable
 * @param	base	val part of the hashtable
 * @param tablesize hashtable size
 *
 * @return	an unsigned int, which represents an entry in the hashtable
 */

static int	strhash(const char *str)
{
	const char	*p;
	int			g;
	int			h;

	h = 0;
	p = str;
	while (*p != '\0')
	{
		h = (h << 4) + (int)(*p);
		g = h & 0xF0000000L;
		if (g != 0)
		{
			h = h ^ (g >> 24);
		}
		h = h & ~g;
		p++;
	}
	return (h);
}

unsigned int	hash(const char *key, char *val, unsigned long int table_size)
{
	unsigned long int	value;
	int					key_len;

	(void)val;
	key_len = strhash(key);
	value = key_len % table_size;
	return (value);
}

void	print_h_table(t_hash_table *h_table)
{
	t_entry	*curr;
	int		i;

	if (!h_table)
	{
		return ;
	}
	i = 0;
	while (i < h_table->size) //size is essentially should be constant
	{
		curr = h_table->entries[i];
		while (curr != NULL)
		{
			ft_printf("%s=%s\n", curr->key, curr->val);
			curr = curr->next;
		}
		i++;
	}
}
