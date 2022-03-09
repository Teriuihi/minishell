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

#ifndef MINISHELL_H
# define MINISHELL_H
# include "structs.h"
# include "../hashtable/hashtable.h"
# include <sys/types.h>


typedef struct	s_signal
{
	int			sigint;
	int			sigquit
	pid_t		pid;
	pid_t		exit_status; //this to alter when we quit?
}				t_signal;

typedef struct s_minishell
{
	char            *cur_wd;
	int             exit_status;
	pid_t           current_pid;
	t_hash_table	*env;
	t_hash_table	*current_env;
}	t_minishell;

extern	t_signal global_signal;

#endif
