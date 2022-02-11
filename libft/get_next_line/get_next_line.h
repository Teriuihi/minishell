/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/10 17:59:12 by sappunn       #+#    #+#                 */
/*   Updated: 2021/12/10 17:59:12 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <wchar.h>

char		*get_next_line(int fd);
char		*ft_nl_substr(void *nl, char *buffer);
char		*ft_strjoin_clear(char *nl, char *str, int clear);
size_t		ft_strlen_stop(const char *str, char stop);
#endif
