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
# include <stdarg.h>

void	free_splitted(char **splitted);
void	free_cmd(t_cmd_data *cmd_data);
void	free_command(void *content);
void	free_parse_and_commands(t_list **commands, t_list **parse_results);

#endif
