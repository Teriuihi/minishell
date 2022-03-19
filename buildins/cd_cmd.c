/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_cmd.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/02 15:51:25 by sappunn       #+#    #+#                 */
/*   Updated: 2022/02/02 15:51:25 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include "../libft/libft.h"
#include "../headers/functions.h"
#include "../buildins/buildins.h"

/**
 * Get path to the new directory from the given argument
 *
 * @param	dir_arg		Argument
 * @param	minishell	Data for minishell
 *
 * @return	Char pointer to path, or NULL on failure
 */
char	*get_path_from_arg(char	*dir_arg, t_minishell *minishell)
{
	char	*dir;
	char	*tmp;

	if (*dir_arg == '/')
		return (dir_arg);
	else
	{
		tmp = ft_strjoin("/", dir_arg);
		if (!tmp)
		{
			ft_printf("Out of memory\n");
			return (NULL);
		}
		dir = ft_strjoin(minishell->cur_wd, tmp);
		free(tmp);
		if (!dir)
			return (NULL);
		return (dir);
	}
}

/**
 * Changes directory and updates pwd on success
 *
 * @param	command	Command that was executed to call tjis function
 *
 * @return	false if cd failed, true if it was executed correctly
 */
t_bool	cd(t_command *command, t_minishell *minishell)
{
	void	*tmp;
	char	*dir;
	t_bool	result;

	if (command->args_len != 2 || command->args[1] == NULL)
	{
		ft_printf("Invalid command, not enough args\n");
		minishell->exit_status = 2; //All builtins return an exit status of 2 to indicate incorrect usage, generally invalid options or missing arguments.
		return (false);
	}
	dir = get_path_from_arg(command->args[1], minishell);
	if (dir == NULL)
		return (false);
	tmp = opendir(dir);
	result = tmp != NULL;
	free(tmp);
	if (result == false)
		return (false);
	chdir(dir);
	if (dir != command->args[1])
		free(dir);
	result = set_pwd(getcwd(NULL, 0), minishell);
	return (result);
}
