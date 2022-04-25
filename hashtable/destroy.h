/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   destroy.h                                         :+:    :+:             */
/*                                                     +:+                    */
/*   By: bmajor <bmajor@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:40:25 by bmajor        #+#    #+#                 */
/*   Updated: 2022/01/26 14:40:25 by bmajor        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef DESTROY_H
# define DESTROY_H
# include "../headers/structs.h"
# include "../headers/functions.h"
# include "hash_utils.h"

void	destroy_key_value(t_entry *entry);
void	destroy_entry(t_entry *entry);

#endif