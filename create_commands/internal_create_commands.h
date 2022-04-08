/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   internal_create_commands.h                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/06 16:16:01 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/06 16:16:01 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_CREATE_COMMANDS_H
# define INTERNAL_CREATE_COMMANDS_H
# include "../headers/structs.h"
# include "../headers/minishell.h"
# include "../headers/arguments.h"

typedef enum e_exit_state
{
	ERROR,
	CONTINUE,
	RETURN,
	BREAK
}	t_exit_state;

typedef struct s_cmd_get_struct
{
	t_list	**head;
	t_list	*cur_arg;
	int		cmd_len;
}	t_cmd_get_struct;

t_bool			append_arguments_to_command(t_command *cmd, t_list *entry,
					int len, t_bool prefix);
t_list			*get_command_start(t_list *cur, int cmd_len);
t_bool			pipe_command(t_cmd_get_struct *cmd_get, t_list *entry,
					t_minishell *minishell);
char			*str_from_arg(t_list *entry);
t_list			*get_arg_at_pos(t_list *entry, int i);
t_cmd_data		*create_new_cmd(t_list **head, char *arg);
t_bool			output_pipe_command(t_cmd_get_struct *cmd_get,
					t_pipe_type pipe_type);
t_exit_state	parse_command(t_cmd_get_struct *cmd_get,
					t_minishell *minishell);
void			update_last_command_input(t_list **head);
#endif
