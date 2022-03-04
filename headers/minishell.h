/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   global.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/14 15:24:19 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/14 15:24:19 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBAL_H
# define GLOBAL_H
# include "structs.h"
# include "../hashtable/hashtable.h"

typedef struct s_minishell
{
	char	*cur_wd;
    t_hash_table	*env;
    t_hash_table	*current_env;
}	t_minishell;
#endif
