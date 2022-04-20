/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   precheck.h                                        :+:    :+:             */
/*                                                     +:+                    */
/*   By: bmajor <bmajor@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:40:25 by bmajor        #+#    #+#                 */
/*   Updated: 2022/01/26 14:40:25 by bmajor        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRECHECK_H
# define PRECHECK_H
# include "../libft/libft.h"
# include "../headers/functions.h"
# include <readline/readline.h>
# include <unistd.h>
# include <fcntl.h>
# include "../buildins/buildins.h"
# include "../run_commands/run_commands.h"
# include <sys/wait.h>
# include <errno.h>

t_bool	pre_fork_check(t_cmd_data *cmd_data, int *old_pid,
			t_minishell *minishell);
t_bool	check_input_redir(t_cmd_data *cmd_data, int *old_pid,
			t_minishell *minishell);
t_bool	should_be_child(t_command *command);
void	child_execute_non_builtin(t_cmd_data *cmd_data, int *old_pid,
			int *cur_pid, t_minishell *minishell);
#endif