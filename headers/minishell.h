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
	int						exit_status;
	int						shell_level;
	pid_t					pid;
	t_bool					print_basic_error;
	t_bool					heredoc;
	t_bool					command;
	t_bool					stop_curr_execution;
	t_cmd_data				*cur_cmd;
	volatile sig_atomic_t	sigint;
	volatile sig_atomic_t	veof;
	volatile sig_atomic_t	sigquit;
}				t_signal;

typedef struct s_minishell
{
	char			*cur_wd;
	char			*home;
	t_hash_table	*env;
}				t_minishell;

extern t_signal	g_signal;

#endif
