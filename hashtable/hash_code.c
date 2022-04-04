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
#include "../hashtable/hashtable.h"
#include "../headers/minishell.h"
#include "../headers/functions.h"
#include <string.h>

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

//get only names from htable
t_list	*get_names(t_hash_table *h_table)
{
	t_entry	*curr;
	t_list	*head;
	char	*current_env;
	char	**envp;
	int		env_i;
	int		i;

	if (!h_table) //at this point this should not be possible
		return (NULL);
	head = (t_list *)malloc(sizeof(t_list));
	if (!head)
		return (NULL);
	i = 0;
	env_i = 0;
	while (i < h_table->size)
	{
		curr = h_table->entries[i];
		if (curr != NULL)
		{
			while (curr != NULL)
			{
				if (curr->key != NULL)
				{
					ft_lstadd_front(&head, ft_lstnew(curr->key));
				}
				curr = curr->next;
			}
		}
		i++;
	}
	return (head);
}

void	sort_by_name(t_list *names) //this sorts in place
{
	t_list	*curr;
	t_list	*index;
	void	*tmp;

	curr = names;
	while (curr->next != NULL)
	{
		t_list *index = curr->next;
		while (index != NULL)
		{
			if (index->content != NULL)
			{
				if (strcmp((char *)curr->content, (char *)index->content) > 0)
				{
					tmp = curr->content;
					curr->content = index->content;
					index->content = tmp;
				}
			}
			index = index->next;
		}
		curr = curr->next;
	}
}

void	export(t_hash_table *h_table)
{
	t_list	*names;
	t_list	*curr;
	char	*val;

	names = get_names(h_table);
	sort_by_name(names);	
	curr = names;
	val = NULL;
	while (curr != NULL)
	{
		if (curr->content != NULL)
		{
			val = ft_get_env_val((char *)curr->content, h_table);
			if (val != NULL)
			{
				ft_printf(1, "declare -x %s=\"%s\"\n",(char *)curr->content, val);
			}
			//else //check if its null or 0
			//{
			//	ft_printf(1, "declare -x %s=\"\"\n",(char *)curr->content, val);
			//}
		}
		curr = curr->next;
	}

}