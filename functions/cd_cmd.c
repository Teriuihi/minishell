#include <unistd.h>
#include <errno.h>
#include "../libft/libft.h"

int	cd(char *dir)
{
	errno = 0;
	int	success;
	if (dir == null)
		success = -2;
	else
		success = chdir(dir);
	return (success);
}
