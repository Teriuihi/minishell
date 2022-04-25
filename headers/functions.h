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
# include <unistd.h>
# include <sys/stat.h>
# include <stdlib.h>
# include "structs.h"
# include "../libft/libft.h"
# include "minishell.h"
# include "arguments.h"
# include "../functions/free_functions.h"

t_list			**find_commands(t_list **args, t_minishell *minishell);
char			*search_in_path(char *command, t_minishell *minishell);
char			*search_in_folder(char *command, t_minishell *minishell);
void			sigquit_handler(int signum);
t_pipe_type		command_separator_type(char *str);
void			init_signal(void);
void			start_program_loop(t_minishell *minishell);
t_bool			ft_remove_exported_var(char *key, t_hash_table *h_table,
					t_minishell *minishell);
t_bool			signal_check(const char *input);
t_bool			new_set_exit_status(int status, const char *str, ...);
void			check_status(void);
int				interruptible_getc(void);
t_pipe_type		pipe_type_from_arg(t_arg *arg);
t_bool			enter_curdir(t_minishell *minishell);
t_bool			should_be_child(t_command *command);

#endif
