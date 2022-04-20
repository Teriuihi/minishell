/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   set_exit_status.h                                 :+:    :+:             */
/*                                                     +:+                    */
/*   By: bmajor <bmajor@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:40:25 by bmajor        #+#    #+#                 */
/*   Updated: 2022/01/26 14:40:25 by bmajor        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SET_EXIT_STATUS_H
# define SET_EXIT_STATUS_H

# include <unistd.h>
# include "../headers/structs.h"
# include "../headers/minishell.h"
# include "../headers/functions.h"
# include <stdio.h>
# include <errno.h>
# include <stdarg.h>
# include "../buildins/buildins.h"

char	*get_pwd(t_minishell *minishell);
char	*get_path(void);
t_bool	set_pwd(char *path, t_minishell *minishell);
t_bool	cd(t_command *command, t_minishell *minishell);
t_bool	execute_builtin(t_command *command, t_minishell *minishell);
t_bool	ft_echo(t_command *command, int fd, t_minishell *minishell);
t_bool	is_builtin(t_command *command);
t_bool	env_variable_found(t_command *command);
t_bool	ft_pwd(char *cur_dir, t_minishell *minishell);

#endif
