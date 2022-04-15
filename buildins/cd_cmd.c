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
static char	*get_path_from_arg(char	*dir_arg, t_minishell *minishell)
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

static t_bool	set_single_cd_command(t_command *command,
										t_minishell *minishell)
{
	char	**args;

	if (ft_get_env_val("HOME", minishell->env) == NULL)
	{
		ft_printf(2, "%s", "some shell: cd: HOME not set\n");
		g_signal.print_basic_error = false;
		return (false);
	}
	args = ft_calloc(3, sizeof(char *));
	if (!args)
		return (false);
	args[0] = command->args[0];
	args[1] = ft_strdup(minishell->home);
	if (!args[1])
	{
		free(args[0]);
		free(args);
		return (false);
	}
	free(command->args);
	command->args = args;
	command->args_len = 2;
	return (true);
}

static t_bool	enter_dir(char *dir, t_command *command, t_minishell *minishell,
										t_bool result)
{
	if (ft_set_env("OLDPWD", get_pwd(minishell), get_hash_table(), true) == false)
	{
		free(dir);
		return (set_exit_status(minishell, 1, NULL, false));
	}
	if (result == false)
	{
		free(dir);
		return (set_exit_status(minishell, 1, NULL, false));
	}
	if (chdir(dir) == -1)
	{
		free(dir);
		return (set_exit_status(minishell, 1, NULL, false));
	}
	if (dir != command->args[1])
	{
		free(dir);
	}
	result = set_pwd(getcwd(NULL, 0), minishell);
	if (result == true)
		return (set_exit_status(minishell, 0, NULL, false));
	else
		return (set_exit_status(minishell, 1, NULL, false));
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
	char	**args;
	t_bool	result;

	if (command->args_len == 1)
	{
		if (set_single_cd_command(command, minishell) == false)
			return (set_exit_status(minishell, 1, NULL, false));
	}
	if (command->args_len != 2)
	{
		g_signal.print_basic_error = false;
		ft_printf(2, "%s%s%s\n", "some shell: cd: ", command->args[1],
			": No such file or directory");
		return (set_exit_status(minishell, 1, NULL, false));
	}
	dir = get_path_from_arg(command->args[1], minishell);
	tmp = opendir(dir);
	if (!tmp)
	{
		g_signal.print_basic_error = false;
		ft_printf(2, "%s%s%s\n", "some shell: cd: ", command->args[1],
			": No such file or directory");
		return (set_exit_status(minishell, 1, NULL, false));
	}
	result = tmp != NULL;
	free(tmp);
	return (enter_dir(dir, command, minishell, result));
}
