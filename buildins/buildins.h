
#ifndef BUILDINS_H
# define BUILDINS_H
# include "../headers/minishell.h"
char	*get_pwd(t_minishell *minishell);
t_bool	set_pwd(char *path, t_minishell *minishell);
char	*get_path(void);
t_bool	cd(t_command *command, t_minishell *minishell);
t_bool	execute_builtin(t_command *command, t_minishell *minishell);
t_bool	ft_echo(t_command *command, int fd, t_minishell *minishell);
t_bool	is_builtin(t_cmd_data *cmd_data);
t_bool	env_variable_found(char *command); //check if its not =hellothere //good comment
t_bool	ft_pwd(char *cur_dir, t_minishell *minishell);
#endif
