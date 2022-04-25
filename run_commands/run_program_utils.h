/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   run_program_utils.h                               :+:    :+:             */
/*                                                     +:+                    */
/*   By: bmajor <bmajor@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:40:25 by bmajor        #+#    #+#                 */
/*   Updated: 2022/01/26 14:40:25 by bmajor        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RUN_PROGRAM_UTILS_H
# define RUN_PROGRAM_UTILS_H
# include "../headers/functions.h"
# include <readline/readline.h>
# include <readline/history.h>
# include "run_program.h"
# include "../builtins/builtins.h"
# include "../parser/parser.h"
# include "run_program_internal.h"
# include "../functions/free_functions.h"

void	copy_pid(const int *cur_pid, int *old_pid);
void	init_run_commands(t_list **entry, t_list **head, int *cur_pid,
			int *old_pid);
t_bool	should_use(char *input);
char	*prompt(void);

#endif