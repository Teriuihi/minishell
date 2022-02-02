#ifndef STRUCTS_H
#define STRUCTS_H

#include <signal.h>

typedef enum
{
	true,
	false
}           t_bool;

typedef struct signal
{
	pid_t	pid;
	int		exit_status;
}				t_signal;

typedef struct lexer
{
	char **
}				t_lexer;

#endif