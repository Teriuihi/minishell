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
# include "../headers/minishell.h"
# include "../libft/libft.h"
# include "../headers/functions.h"
# include "../buildins/buildins.h"
# include "../hashtable/export.h"

t_bool	ft_pwd(char *cur_dir, t_minishell *minishell);

#endif