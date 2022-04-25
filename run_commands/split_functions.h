/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   split_functions.h                                 :+:    :+:             */
/*                                                     +:+                    */
/*   By: bmajor <bmajor@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:40:25 by bmajor        #+#    #+#                 */
/*   Updated: 2022/01/26 14:40:25 by bmajor        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPLIT_FUNCTIONS_H
# define SPLIT_FUNCTIONS_H
# include "../libft/libft.h"
# include "../headers/functions.h"
# include "../builtins/builtins.h"
# include "run_program_internal.h"
# include "../hashtable/export.h"

int				split_len(char **splitted);
t_exit_state	splitter2(t_command *command, int i, char **splitted,
					t_minishell *minishell);
t_bool			splitter(int *i, t_command *command, t_minishell *minishell);

#endif
