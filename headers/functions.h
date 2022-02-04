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
# include "structs.h"
# include "../libft/libft.h"

t_list		**find_commands(char **args);
char		*get_pwd(char *path);
int			pwd(char *path);
int			cd(char *dir);
char		*get_path(void);
char		**get_args(char *input);
char		*search_in_path(char *command);
size_t		ft_strlcat(char *dst, const char *src, size_t dstsize);
int			execute_builtin(t_command *command, int fd_read);
void		ft_echo(t_command *command, int fd);
t_bool		is_builtin(char *command);
void		crtld_handler(int signum);
void		sigquit_handler(int signum);
void		sigint_handler(int signum);
char		*get_working_directory(char *path);
t_signal	*init_signal(void);
void		print_splitted(char **args);
t_pipe_type	command_separator_type(char *str);

#endif
