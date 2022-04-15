/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   hashtable.h                                       :+:    :+:             */
/*                                                     +:+                    */
/*   By: bmajor <bmajor@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:40:25 by bmajor        #+#    #+#                 */
/*   Updated: 2022/01/26 14:40:25 by bmajor        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FREE_FUNCTIONS_H
# define FREE_FUNCTIONS_H
# include <unistd.h>
# include "../headers/structs.h"
# include "../headers/minishell.h"
# include "../headers/functions.h"
# include <stdio.h>
# include <errno.h>
# include <stdarg.h>

void	free_splitted(char **splitted);
void	free_cmd(t_cmd_data *cmd_data);
void	free_command(void *content);
void	free_commands(t_list **head);
void	free_char_arr(char **args);

#endif
