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
char	**get_names(t_hash_table *h_table)
{
	t_entry	*curr;
	char	*current_env;
	char	**envp;
	int		env_i;
	int		i;

	if (!h_table)
	{
		return (NULL);
	}
	envp = (char **)ft_calloc(h_table->size + 1, sizeof(char *)); //get the amount of distinct key value pairs created, htable size will be less than the slots
	if (!envp)
	{
		return (NULL);
	}
	envp[h_table->size] = NULL;
	i = 0;
	env_i = 0;
	while (i < h_table->size)
	{
		curr = h_table->entries[i];
		if (curr != NULL)
		{
			while (curr != NULL)
			{
				envp[env_i] = ft_strdup(curr->key);
				env_i++;
				curr = curr->next;
			}
		}
		i++;
	}
	return (envp);
}

void	export(t_hash_table *h_table)
{
	char	**sorted;
	char	*tmp;
	int		i;
	int		j;
	int		sorted_len;

	//sort a double pointer?
	//allocate enough memory
	sorted = get_names(h_table);
	for (sorted_len = 0; sorted[sorted_len] != NULL; sorted_len++)
	{
		;
	}
	//ft_bzero(tmp, 1100);
	tmp = (char *)ft_calloc(1000, 1);

	//sort sorted based on key sorted[0]
	i = 0;
	while (i < sorted_len)
	{
		j = i + 1;
		while (j < sorted_len)
		{
			if (ft_strncmp(sorted[i], sorted[j], ft_strlen(sorted[i]) > 0))
			{
				tmp = (char *)ft_calloc(ft_strlen(sorted[i]), 1);
				ft_strncpy(tmp, sorted[i], ft_strlen(sorted[i]));
				//ft_printf(1,"%s is tmp now\n", tmp);
				
				free(sorted[i]);
				sorted[i] = (char *)ft_calloc(ft_strlen(sorted[j]), 1);
				ft_strncpy(sorted[i], sorted[j], ft_strlen(sorted[j]));
				//ft_printf(1,"%s is sorted[i] now\n", sorted[i]);
				
				free(sorted[j]);
				sorted[j] = (char *)ft_calloc(ft_strlen(tmp), 1);
				ft_strncpy(sorted[j], tmp, ft_strlen(tmp));
				//ft_printf(1,"%s is sorted[j] now\n", sorted[j]);
			}
			j++;
		}
		i++;
	}
	//print_splitted(sorted);
}