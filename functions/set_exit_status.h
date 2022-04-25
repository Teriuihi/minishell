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
# include "../builtins/builtins.h"

t_bool	is_builtin(t_command *command);
t_bool	env_variable_found(t_command *command);
t_bool	ft_pwd(char *cur_dir);

#endif
