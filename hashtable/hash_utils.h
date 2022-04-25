/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   hash_utils.h                                      :+:    :+:             */
/*                                                     +:+                    */
/*   By: bmajor <bmajor@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:40:25 by bmajor        #+#    #+#                 */
/*   Updated: 2022/01/26 14:40:25 by bmajor        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASH_UTILS_H
# define HASH_UTILS_H
# include "../pipes/redirects.h"
# include "../libft/libft.h"
# include "../headers/functions.h"
# include "../builtins/builtins.h"
# include "../hashtable/export.h"
# include "../run_commands/split_functions.h"
# include "destroy.h"

t_bool	env_var_added(t_command *command, t_minishell *minishell);
t_bool	create_key_val(t_entry *entry, char *key, char *val);

#endif