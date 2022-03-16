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
# include <signal.h>
#include <termios.h>


typedef struct	s_signal
{
	volatile sig_atomic_t	sigint;
	volatile sig_atomic_t	sigquit;
	t_bool					finished;
	pid_t					pid;
	pid_t					exit_status; //this to alter when we quit?
	int						keep_running;
	int						terminal_descriptor;
	struct termios			terminal_original;
	struct termios			terminal_settings;

}				t_signal;

typedef struct s_minishell
{
	char            *cur_wd;
	int             exit_status;
	pid_t           current_pid;
	t_hash_table	*env;
	t_hash_table	*current_env;
	t_bool			are_we_still_running;
	
}	t_minishell;

//extern	t_signal *global_signal;

extern  t_signal g_signal;

#endif
