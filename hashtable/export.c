/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export.c.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 17:59:28 by sappunn       #+#    #+#                 */
/*   Updated: 2021/12/10 17:59:28 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "export.h"
#include "../hashtable/hashtable.h"
#include "../headers/minishell.h"
#include "../headers/functions.h"
#include <string.h>

static t_list	*get_names(t_hash_table *h_table)
{
	t_entry	*curr;
	t_list	*head;
	int		env_i;
	int		i;

	head = (t_list *)ft_calloc(1, sizeof(t_list));
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
				if (curr->key != NULL && curr->is_exported == true)
					ft_lstadd_front(&head, ft_lstnew(curr->key));
				curr = curr->next;
			}
		}
		i++;
	}
	return (head);
}

static void	sort_by_name(t_list *names)
{
	t_list	*curr;
	t_list	*index;
	void	*tmp;

	curr = names;
	while (curr->next != NULL)
	{
		index = curr->next;
		while (index->content != NULL)
		{
			if (index->content != NULL && curr->content != NULL)
			{
				if (ft_strcmp((char *)curr->content,
						(char *)index->content) > 0)
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

static void	print_exported(t_list *curr, t_hash_table *h_table, t_bool *status)
{
	char			*val;

	while (curr != NULL)
	{
		if (curr->content != NULL)
		{
			val = ft_get_exported_env((char *)curr->content, h_table, status);
			if (val != NULL)
				ft_printf(1, "declare -x %s=\"%s\"\n", (char *)curr->content, val);
			else
				ft_printf(1, "declare -x %s\n", (char *)curr->content);
		}
		curr = curr->next;
	}
}

t_bool	export_cmd(void *minishell)
{
	t_hash_table	*h_table;
	t_list			*names;
	t_list			*curr;
	t_bool			status;

	h_table = ((t_minishell *)minishell)->env;
	names = get_names(h_table);
	if (!names)
		return (set_exit_status((t_minishell *)minishell, 1, NULL, false));
	sort_by_name(names);
	curr = names;
	if (!curr)
		return (set_exit_status(minishell, 1, NULL, false));
	print_exported(curr, h_table, &status);
	if (status == false)
		return (set_exit_status(minishell, 1, NULL, false));
	else
		return (set_exit_status(minishell, 0, NULL, false));
}
