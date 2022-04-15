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
# include "../headers/minishell.h"

typedef struct s_parse_data {
	char		*input;
	int			pos;
	int			start;
	t_bool		has_data;
	t_bool		is_literal;
	t_string	*string;
}	t_parse_data;

t_bool		append_content(t_parse_data *data);
t_bool		parse_env_variable(t_parse_data *data, t_list **head,
				t_minishell *minishell);
t_bool		parse_quotation(t_parse_data *data, char quote,
				t_list **head, t_minishell *minishell);
t_string	*safe_add_to_list(t_list **head, t_string *string, t_bool literal);
t_bool		is_pipe(char c);
t_bool		store_normal_arg(t_parse_data *data, t_list **head);
t_bool		parse_into_data(t_parse_data *data, t_list **head,
				t_minishell *minishell);
t_bool		parse_error(char near);
#endif
