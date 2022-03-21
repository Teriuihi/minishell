/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   hashtable.h                                       :+:    :+:             */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:40:25 by sappunn       #+#    #+#                 */
/*   Updated: 2022/01/26 14:40:25 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASHTABLE_H
# define HASHTABLE_H
# include "../headers/bool.h"

typedef struct entry
{
	t_bool			is_exported;
	char			*val;
	char			*key;
	struct entry	*next;
}	t_entry;

typedef struct entry_table
{
	t_entry		**entries;
	int			size;
}	t_hash_table;

t_hash_table	*get_hash_table(void);
t_hash_table	*init_hash_table(int size);
unsigned int	hash(const char *key, char *val, unsigned long int table_size);
t_entry			*create_hash_table_pair(char *key, char *val,
					t_bool is_exported);
t_bool			succesful_insert(t_hash_table *h_table, char *key, char *val,
					t_bool is_exported);
t_bool			all_args_inserted(t_hash_table *h_table, char *key, char *val);
t_hash_table	*create_env_h_table(void);
void			destroy_hash_table(t_hash_table *hash_table, int size);
char			*ft_get_env_val(char *key, t_hash_table *h_table);
t_bool			ft_set_env(char *key, char *val, t_hash_table *h_table,
					t_bool is_exported);
void			free_key_value(t_entry *entry);
void			destroy_entry(t_entry *entry);
t_bool			print_h_table(t_hash_table *h_table);
char			**get_envp(t_hash_table *h_table);
void			set_to_exported(t_hash_table *h_table);

#endif
