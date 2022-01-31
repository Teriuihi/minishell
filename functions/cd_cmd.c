#include <unistd.h>
#include "../libft/libft.h"
#include "../headers/functions.h"

/**
 * Changes directory and updates pwd on success
 *
 * @param	dir	free-able char pointer
 *
 * @return	-1 for failure in chdir command
 * 	-2 if dir was null
 */
int	cd(char *dir)
{
	errno = 0;
	int	success;
	if (dir == null)
		success = -2;
	else
		success = chdir(dir);
	if (!success)
		pwd(dir);
	return (success);
}
