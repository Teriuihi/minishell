/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   run_commands.h                                           :+:    :+:      */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:36:43 by sappunn       #+#    #+#                 */
/*   Updated: 2022/01/26 14:36:43 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RUN_PROGRAM_H
# define RUN_PROGRAM_H
# include "../headers/minishell.h"
# include "../headers/bool.h"

void	exec_command(t_cmd_data *cmd_data, int *old_pid, int *cur_pid,
			t_minishell *minishell);
t_bool	execute_non_forked_builtin(t_command *command, t_minishell *minishell);
void	run_commands(t_list **head, t_minishell *minishell);
void	start_program_loop(t_minishell *minishell);

#endif
