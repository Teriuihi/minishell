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
# include <termios.h>

typedef struct s_signal
{
	t_bool					finished;
	t_bool					interrupted;
	pid_t					pid;
	pid_t					exit_status;
	int						shell_level;
	int						minishell_exec_found;
	int						terminal_descriptor;
	struct termios			old_termios;
	struct termios			new_termios;
	volatile sig_atomic_t	sigint;
	volatile sig_atomic_t	veof;
}				t_signal;

typedef struct s_minishell
{
	char			*cur_wd;
	int				exit_status;
	pid_t			current_pid;
	t_hash_table	*env;	
}				t_minishell;

extern t_signal	g_signal;
#endif
