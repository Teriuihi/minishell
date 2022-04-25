/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   manipulate_env.h                                  :+:    :+:             */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:40:25 by sappunn       #+#    #+#                 */
/*   Updated: 2022/01/26 14:40:25 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANIPULATE_ENV_H
# define MANIPULATE_ENV_H
# include "../headers/minishell.h"
# include "hashtable.h"

t_bool	print_h_table(t_hash_table *h_table, int len);
char	*ft_get_exported_env(char *key, t_hash_table *h_table, t_bool *success);
char	*ft_get_env_val(char *key, t_hash_table *h_table, t_bool *success);
t_bool	ft_set_env(char *key, char *val, t_hash_table *h_table,
			t_bool is_exported);
t_bool	ft_remove_exported_var(char *key, t_hash_table *h_table,
			t_minishell *minishell);
#endif
