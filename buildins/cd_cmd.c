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
			return (NULL);
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
	char	**args;

	if (command->args_len == 1)
	{
		args = ft_calloc(3, sizeof(char *));
		if (!args)
			return (set_exit_status(minishell, 1, NULL));
		args[0] = command->args[0];
		args[1] = ft_strdup(minishell->home); //TODO error checking
		free(command->args);
		command->args = args;
		command->args_len = 2;
	}
	if (command->args_len != 2)
	{
		char *message = ft_strjoin("some shell: cd: ", ft_strjoin(command->args[1], ": No such file or directory\n"));
		if (!message)
			return (set_exit_status(minishell, 1, NULL));
		return (set_exit_status(minishell, 1, message));
	}
	if (command->args[1] == NULL)
		return (set_exit_status(minishell, 1, NULL));
	dir = get_path_from_arg(command->args[1], minishell);
	if (dir == NULL)
		return (set_exit_status(minishell, 1, NULL));
	tmp = opendir(dir);
	if (!tmp)
	{
		char *message = ft_strjoin("some shell: cd: ", ft_strjoin(command->args[1], ": No such file or directory\n"));
		if (!message)
			return (set_exit_status(minishell, 1, NULL));
		return (set_exit_status(minishell, 1, message));
	}
	result = tmp != NULL;
	free(tmp);
	ft_set_env("OLDPWD", get_pwd(minishell), get_hash_table(), true); //TODO error checking
	if (result == false)
		return (set_exit_status(minishell, 1, NULL));
	if (chdir(dir) == -1) //what if chdir fails? this is technically a sys call
		return (set_exit_status(minishell, 1, NULL));
	if (dir != command->args[1])
		free(dir);
	result = set_pwd(getcwd(NULL, 0), minishell);
	if (result == true)
		return (set_exit_status(minishell, 0, NULL));
	else
		return (set_exit_status(minishell, 1, NULL));
}
