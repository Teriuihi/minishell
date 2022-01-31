
#include "../libft/libft.h"
#include <stdio.h>

/**
 * searches the PATH variable in extern char **environ; 
 *
 * @param	input	User input
 *
 * @return	the PATH variable as string 
 */

extern char **environ;

char	*get_path()
{
	int i;

	i = 0;
	while (environ[i] != NULL)
	{
		if (ft_strcmp(environ[i], "PATH") == 0)
			return(environ[i]);
		i++;
	}
	return (NULL);
}

int main(void)
{
	printf("%s is the PATH\n", get_path());
	return (0);
}