/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cnotrol_child_and_pipes.h                         :+:    :+:             */
/*                                                     +:+                    */
/*   By: bmajor <bmajor@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:40:25 by bmajor        #+#    #+#                 */
/*   Updated: 2022/01/26 14:40:25 by bmajor        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTROL_CHILD_AND_PIPES_H
# define CONTROL_CHILD_AND_PIPES_H

# include "../libft/libft.h"
# include "../headers/functions.h"
# include <readline/readline.h>
# include <unistd.h>
# include <fcntl.h>
# include "../builtins/builtins.h"
# include "../run_commands/run_commands.h"
# include <sys/wait.h>
# include <errno.h>
# include "redirects.h"

t_bool	init_child(int *old_pid, int *cur_pid, t_pipe_type type,
			t_minishell *minishell);
t_bool	exec_dup_close(int *pid_to_dup, int *pid1_to_close,
			int *pid2_to_close);
t_bool	control_pipes(t_cmd_data *cmd_data, int *old_pid, int *cur_pid,
			t_minishell *minishell);

#endif