/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirects.h                                       :+:    :+:             */
/*                                                     +:+                    */
/*   By: bmajor <bmajor@student.codam.nl>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:40:25 by bmajor       #+#    #+#                 */
/*   Updated: 2022/01/26 14:40:25 by bmajor       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTS_H
# define REDIRECTS_H
# include "../libft/libft.h"
# include "../headers/functions.h"
# include <readline/readline.h>
# include <unistd.h>
# include <fcntl.h>
# include "../builtins/builtins.h"
# include "../run_commands/run_commands.h"
# include <sys/wait.h>
# include <errno.h>

t_bool	read_input_write(t_cmd_data *cmd_data, int old_pid[2]);
t_bool	redirect_output(t_cmd_data *cmd_data, t_minishell *minishell);
t_bool	append_output(t_cmd_data *cmd_data, t_minishell *minishell);
t_bool	redirect_file(t_cmd_data *cmd_data, int *old_pid,
			t_minishell *minishell);
t_bool	close_pipes(int *pid1, int *pid2);

#endif