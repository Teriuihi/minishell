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

t_list			**find_commands(t_list **args, t_minishell *minishell);
char			*search_in_path(char *command, t_minishell *minishell);
size_t			ft_strlcat(char *dst, const char *src, size_t dstsize);
void			crtld_handler(int signum);
void			sigquit_handler(int signum);
void			sigint_handler(int signum);
void			print_splitted(char **args);
t_pipe_type		command_separator_type(char *str);
void			crtld_handler(int signum);
void			sigquit_handler(int signum);
void			sigint_handler(int signum);
void			init_signal(void);
void			print_splitted(char **args);
void			err_exit(char *err, int status);
int				err_int_return(char *err, int status);
void			*err_ptr_return(char *err, void *ptr);
void			free_commands(t_list **head);
void			free_char_arr(char **args);
void			start_program_loop(t_minishell *minishell);
size_t			ft_strlcat(char *dst, const char *src, size_t dstsize);
void			free_splitted(char **splitted);
t_bool			ft_remove_exported_var(char *key, t_hash_table *h_table,
					t_minishell *minishell);
t_bool			signal_check(char *input, t_minishell *minishell);
t_bool			set_exit_status(t_minishell *minishell, int status, char *message);
void			check_status(t_minishell *minishell);
char			*search_folder(char *command, t_minishell *minishell);
int 			interruptible_getc(void);

#endif
