/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_pwd.h                                          :+:    :+:             */
/*                                                     +:+                    */
/*   By: bmajor <bmajor@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:40:25 by bmajor        #+#    #+#                 */
/*   Updated: 2022/01/26 14:40:25 by bmajor        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PWD_H
# define FT_PWD_H
# include "../builtins/builtins.h"
# include "../hashtable/export.h"

char	*get_pwd(t_minishell *minishell);
t_bool	ft_pwd(char *cur_dir);
t_bool	set_pwd(char *path, t_minishell *minishell);

#endif