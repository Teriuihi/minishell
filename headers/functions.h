/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   functions.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:40:25 by sappunn       #+#    #+#                 */
/*   Updated: 2022/01/26 14:40:25 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUNCTIONS_H
# define FUNCTIONS_H
#include <unistd.h>
#include <sys/stat.h>
#include "structs.h"

char	*get_path();
char	**get_args(char *input);
void	search_in_path(char **args);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
void	ft_echo(char **args);
t_bool	is_builtin(char **args);
void	execute_builtin(char **args);

#endif
