
#ifndef RUN_COMMANDS_H
# define RUN_COMMANDS_H
# include "../headers/minishell.h"
# include "../headers/bool.h"
void	exec_command(t_cmd_data *cmd_data, int *old_pid, int *cur_pid,
                             t_bool is_built_in, t_minishell *minishell);
t_bool	child_execute_built_in_not_child(t_command *command, t_minishell *minishell);
void	run_commands(t_list **head, t_minishell *minishell);
void	start_program_loop(t_minishell *minishell);
#endif
