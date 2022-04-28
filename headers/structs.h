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
# include "bool.h"

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
	t_bool		export_found;
}	t_command;

typedef struct s_redirect
{
	t_pipe_type	type;
	char		*file;
}	t_redirect;

typedef struct s_cmd_data
{
	t_command	*command;
	t_redirect	input;
	t_redirect	output;
	t_bool		executable_found;
	t_bool		has_error;
}	t_cmd_data;
#endif
