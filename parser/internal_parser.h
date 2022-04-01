/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   internal_parser.h                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 18:23:43 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/28 18:23:43 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_PARSER_H
# define INTERNAL_PARSER_H
# include "../string/string.h"

t_string	*append_content(char *input, int start, int pos, t_string *arg);
t_string	*parse_env_variable(char *input, int start, int *pos,
				t_string *arg);
t_string	*parse_quotation(char *input, int *start, char quote,
				t_string *arg);
t_string	*safe_add_to_list(t_list **head, t_string *string, t_bool literal);

#endif
