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
		dir = ft_strjoin(get_pwd(minishell), tmp);
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
		//ft_printf("%d is command args len\n", command->args_len);
		//ft_printf("Invalid command, not enough args\n");
		return (set_exit_status(minishell, 1));
	}
	//if there are more args
	dir = get_path_from_arg(command->args[1], minishell);
	if (dir == NULL)
		return (set_exit_status(minishell, 1));
	tmp = opendir(dir);
	result = tmp != NULL;
	free(tmp);
	ft_set_env("OLDPWD", get_pwd(minishell), get_hash_table(), true);
	if (result == false)
		return (set_exit_status(minishell, 1));
	chdir(dir);
	if (dir != command->args[1])
		free(dir);
	result = set_pwd(getcwd(NULL, 0), minishell);
	if (result == true)
		return (set_exit_status(minishell, 0));
	else
		return (set_exit_status(minishell, 1));
}
