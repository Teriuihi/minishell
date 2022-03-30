/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   buildins.h                                        :+:    :+:             */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:40:25 by sappunn       #+#    #+#                 */
/*   Updated: 2022/01/26 14:40:25 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILDINS_H
# define BUILDINS_H

# include "../headers/minishell.h"

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
