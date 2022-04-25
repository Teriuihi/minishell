/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipe_utils.h                                      :+:    :+:             */
/*                                                     +:+                    */
/*   By: bmajor <bmajor@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:40:25 by bmajor        #+#    #+#                 */
/*   Updated: 2022/01/26 14:40:25 by bmajor        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPE_UTILS_H
# define PIPE_UTILS_H
# include "redirects.h"

t_bool	close_pipes(int *pid1, int *pid2);
void	check_is_dir(t_command *command);

#endif