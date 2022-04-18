/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   run_commands_internal.h                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/18 14:20:29 by sappunn       #+#    #+#                 */
/*   Updated: 2022/04/18 14:20:29 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RUN_COMMANDS_INTERNAL_H
# define RUN_COMMANDS_INTERNAL_H

typedef enum e_exit_state
{
	ERROR,
	CONTINUE,
	RETURN,
	BREAK
}	t_exit_state;

#endif
