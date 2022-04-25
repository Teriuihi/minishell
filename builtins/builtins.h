/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.h                                        :+:    :+:             */
/*                                                     +:+                    */
/*   By: bmajor <bmajor@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:40:25 by bmajor        #+#    #+#                 */
/*   Updated: 2022/01/26 14:40:25 by bmajor        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../headers/minishell.h"
# include "../libft/libft.h"
# include "../headers/functions.h"
# include "ft_pwd.h"
# include "ft_cd.h"

t_bool	execute_builtin(t_command *command, t_minishell *minishell);
t_bool	is_builtin(t_command *command);
t_bool	env_variable_found(t_command *command);

#endif
