/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   arguments.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 17:29:14 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/28 17:29:14 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGUMENTS_H
# define ARGUMENTS_H
# include "bool.h"
# include "../string/string.h"

typedef struct s_arg
{
	t_string	*arg;
	t_bool		literal;
}	t_arg;

#endif
