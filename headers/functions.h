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
# include "global.h"

t_list			**find_commands(char **args);
char			*get_pwd(t_minishell *minishell);
t_bool			set_pwd(char *path, t_minishell *minishell);
t_bool			cd(t_command *command, t_minishell *minishell);
char			*get_path(void);
char			**get_args(char *input);
char			*search_in_path(char *command);
size_t			ft_strlcat(char *dst, const char *src, size_t dstsize);
t_bool			execute_builtin(t_command *command, t_minishell *minishell);
void			ft_echo(t_command *command, int fd);
t_bool			is_builtin(t_command *command);
void			crtld_handler(int signum);
void			sigquit_handler(int signum);
void			sigint_handler(int signum);
char			*get_working_directory(char *path);
t_signal		*init_signal(void);
void			print_splitted(char **args);
t_pipe_type		command_separator_type(char *str);
void			crtld_handler(int signum);
void			sigquit_handler(int signum);
void			sigint_handler(int signum);
t_signal		*init_signal(void);
void			print_splitted(char **args);
t_hash_table	*get_hash_table(void);
void			exec_command(t_command *command, int *old_pid, int *cur_pid,
					t_bool is_built_in, t_minishell *minishell);
void			err_exit(char *err, int status);
int				err_int_return(char *err, int status);
void			*err_ptr_return(char *err, void *ptr);
t_bool			env_variable_found(char *command); //check if its not =hellothere
t_bool			child_execute_built_in_not_child(t_command *command, t_minishell *minishell);
void			free_commands(t_list **head);
void			free_char_arr(char **args);
void			start_program_loop(t_minishell *minishell);

/* hashtable */
t_hash_table	*init_hash_table(int size);
unsigned int	hash(const char *key, char *val, unsigned long int table_size);
t_entry			*create_hash_table_pair(char *key, char *val);
t_bool			succesful_insert(t_hash_table *h_table, char *key, char *val);
t_bool			all_args_inserted(t_hash_table *h_table, char *key, char *val);
t_hash_table	*duplicates_are_found_in_argv(void);
void			destroy_hash_table(t_hash_table *hash_table, int size);
char			*ft_get_env_val(char *key, t_hash_table *h_table);
void			ft_set_env(char *key, char *val, t_hash_table *h_table);
void			ft_remove_exported_var(char *key, t_hash_table *h_table);
void			free_key_value(t_entry *entry);
void			print_h_table(t_hash_table *h_table);

//
void			run_commands(t_list **head, t_minishell *minishell);
size_t			ft_strlcat(char *dst, const char *src, size_t dstsize);
char			**get_envp(t_hash_table *h_table);
void			free_splitted(char **splitted);

#endif
