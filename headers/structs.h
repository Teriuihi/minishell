/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   structs.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:40:25 by sappunn       #+#    #+#                 */
/*   Updated: 2022/01/26 14:40:25 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H
# include <signal.h>
# include "../libft/libft.h"

typedef enum s_bool
{
	true,
	false
}				t_bool;

typedef struct entry
{
	char			*val;
	char			*key;
	struct entry	*next;
}				t_entry;

typedef struct entry_table
{
	t_entry		**entries;
	int			size;
}				t_hash_table;

typedef struct signal
{
	pid_t	pid;
	int		exit_status;
}				t_signal;

typedef enum e_pipe_type
{
	NONE,
	OUTPUT_TO_COMMAND,
	REDIRECT_INPUT,
	REDIRECT_OUTPUT,
	DELIMITER_INPUT,
	APPEND_OUTPUT
}	t_pipe_type;

typedef struct s_command
{
	char		*command;
	char		**args;
	int			args_len;
	t_pipe_type	type;
}	t_command;

typedef struct s_command_data
{
	t_list		**commands;
	int			pipes;
}	t_command_data;

#endif