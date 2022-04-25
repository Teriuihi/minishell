/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/04 20:24:34 by sappunn       #+#    #+#                 */
/*   Updated: 2022/03/04 20:24:34 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include "../libft/libft.h"
# include "../headers/minishell.h"
# include "../hashtable/manipulate_env.h"

t_list	**parse(char *input, t_minishell *minishell);

#endif
