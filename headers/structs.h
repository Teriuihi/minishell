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

#endif