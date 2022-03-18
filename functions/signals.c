/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                                :+:    :+:      */
/*                                                     +:+                    */
/*   By: sappunn <sappunn@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/26 14:36:43 by sappunn       #+#    #+#                 */
/*   Updated: 2022/01/26 14:36:43 by sappunn       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../headers/functions.h"
#include "../headers/structs.h"
#include "../headers/minishell.h"
/**
 * Handling different signals based on signum
 *
 *
 * @param	args signum
 *
 * @return	void
 */


//https://www.cons.org/cracauer/sigint.html

//https://stackoverflow.com/questions/1516122/how-to-capture-controld-signal
void	crtld_handler(int signum)
{
	(void)signum;
}


/* WAS IN SIGINT AFTER ASSIGNED VAL
if (this_signal == SIGINT) //crtl c lett ez?
	{
		g_signal.sigint = 1;
		if (g_signal.pid != 0)
		{
			//g_signal.sigquit = 0;
			ft_printf("entered here pid %d\n", g_signal.pid);
			g_signal.sigquit = 1;
			exit(0); //if its the parent we have to exit it
			//kill(getpid(), SIGKILL);
		}
		else //else its now we are in the child process, quit that while waiting 
		{
			//g_signal.sigquit = 0;
			kill(g_signal.pid, SIGKILL);
			//exit(0);
		}
		//exit(0); //just set the status, and maybe check at the parent?
		//we only need to exit the forked pid not the entire shell
}
*/


/*
void	sigquit_handler(int signum) //crtl + \ 
{
	(void)signum; //write(1, "entered to crtl + backslash signal\n", 36);
	if (global_signal.pid != 0)
	{
		global_signal.exit_status = 128 + 3;
		global_signal.sigquit = 1;
	}
	else
	{
		//some error somewhere?
	}
}

void	sigint_handler(int signum) // crtl + C , repeat prompt /
{
	(void)signum;
	global_signal->sigint = 1;
	if (global_signal->pid == 0) //if its a child process
	{
		global_signal->exit_status = 1;
		keep_running = 0;
	}
	else
	{
		global_signal->exit_status = 128 + 2; //https://unix.stackexchange.com/questions/386836/why-is-doing-an-exit-130-is-not-the-same-as-dying-of-sigint
		keep_running = 0;
	}
	//have to get somehow the exit status, 1 iter27 or 0 and assign it into a struct?
}
*/


void	signal_check(char *input)
{
	if (input == 0 || g_signal.sigint == 1) //EOF RECEIVED crtld at the moment
	{
		if (g_signal.sigint != 1)
		{
			g_signal.sigquit = 1;
		}
	}
}

void	init_signal(void) //should this be global?
{
	g_signal.sigint = 0;
	g_signal.sigquit = 0;
	g_signal.finished = false;
	g_signal.pid = getpid();
	g_signal.keep_running = 1;
}



/*



/*
void	sig_hnd(int sig)
{
	(void)sig;
	ft_printf("CRTL C\n");
	//exit(0);
}


void	sigint_handler(int this_signal)
{

	if (this_signal == SIGINT)
	{
		//global signal stuff to 0
		ft_printf("sigint registered\n");
		g_signal.sigint = 1;
		if (g_signal.pid == 0) //if we are in a child process
		{
			//we have to stop the child process, kill it?
			//ft_printf("setting sigint to 1 inside CHILD\n");
			kill(g_signal.pid, SIGKILL);
			g_signal.sigint = 0;
		}
		if (g_signal.pid != 0) //if we are in the parent
		{
			//g_signal.sigquit = 0;
			//ft_printf("setting sigint to 1 inside parent\n");
			g_signal.pid = 1;
			g_signal.sigint = 1; //this sets the loop var to one, breaks start program loop
			//kill(g_signal.pid, SIGINT);
		}
		//ft_printf("\nsome shell>");
	}
	
}

// Restore terminal to original settings


static void terminal_done(void)
{
    if (g_signal.terminal_descriptor != -1)
        tcsetattr(g_signal.terminal_descriptor, TCSANOW, &g_signal.terminal_original);
}
static void terminal_signal(int signum)
{
	ft_printf("%d is signum, received\n", signum);

	if (signum == SIGINT)
	{
		ft_printf("we also sent sigint here\n");
		if (g_signal.terminal_descriptor != -1)
			tcsetattr(g_signal.terminal_descriptor, TCSANOW, &g_signal.terminal_original);
		ft_printf("gonna exit in terminal signal w 128 + %\n", signum);
		g_signal.sigint = 1;
		return ;
		//_exit(128 + signum);
	}	
}

void termios_init(t_minishell *minishell)
{
	//struct termios terminal_original, terminal_settings;
	struct sigaction act;

	if (isatty(STDERR_FILENO))
		g_signal.terminal_descriptor = STDERR_FILENO;
	if (isatty(STDIN_FILENO))
		g_signal.terminal_descriptor = STDIN_FILENO;
	if (isatty(STDOUT_FILENO))
		g_signal.terminal_descriptor = STDOUT_FILENO;
	else
        ft_printf("Error while initializing terminal\n");

	if (tcgetattr(g_signal.terminal_descriptor, &g_signal.terminal_original) ||
		tcgetattr(g_signal.terminal_descriptor, &g_signal.terminal_settings))
	{
		ft_printf("Error while initializing terminal2\n");
	}
	if (atexit(terminal_done))
		ft_printf("Error while calling atexit\n");
	sigemptyset(&act.sa_mask);
	act.sa_handler = terminal_signal;
	if (sigaction(SIGHUP,  &act, NULL) ||
        sigaction(SIGINT,  &act, NULL) ||
        sigaction(SIGQUIT, &act, NULL) ||
        sigaction(SIGTERM, &act, NULL) ||
#ifdef SIGXCPU
        sigaction(SIGXCPU, &act, NULL) ||
#endif
#ifdef SIGXFSZ    
        sigaction(SIGXFSZ, &act, NULL) ||
#endif
#ifdef SIGIO
        sigaction(SIGIO,   &act, NULL) ||
#endif
        sigaction(SIGPIPE, &act, NULL) ||
        sigaction(SIGALRM, &act, NULL))

 // Let BREAK cause a SIGINT in input. /
    g_signal.terminal_settings.c_iflag &= ~IGNBRK;
    g_signal.terminal_settings.c_iflag |=  BRKINT;
 
    // Ignore framing and parity errors in input. /
    g_signal.terminal_settings.c_iflag |=  IGNPAR;
    g_signal.terminal_settings.c_iflag &= ~PARMRK;
 
    // Do not strip eighth bit on input. /
    g_signal.terminal_settings.c_iflag &= ~ISTRIP;
 
    // Do not do newline translation on input. /
    g_signal.terminal_settings.c_iflag &= ~(INLCR | IGNCR | ICRNL);
 
#ifdef IUCLC
    // Do not do uppercase-to-lowercase mapping on input. /
    terminal_settings.c_iflag &= ~IUCLC;
#endif
 
    // Use 8-bit characters. This too may affect standard streams,
     // but any sane C library can deal with 8-bit characters. /
   	g_signal.terminal_settings.c_cflag &= ~CSIZE;
    g_signal.terminal_settings.c_cflag |=  CS8;
    // Enable receiver. /
    g_signal.terminal_settings.c_cflag |=  CREAD;
    // Let INTR/QUIT/SUSP/DSUSP generate the corresponding signals. /
    g_signal.terminal_settings.c_lflag |=  ISIG;
    // Enable noncanonical mode.
    // This is the most important bit, as it disables line buffering etc. /
    g_signal.terminal_settings.c_lflag &= ~ICANON;
    // Disable echoing input characters. /
    g_signal.terminal_settings.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
    // Disable implementation-defined input processing. /
    g_signal.terminal_settings.c_lflag &= ~IEXTEN;
    // To maintain best compatibility with normal behaviour of terminals,
     // we set TIME=0 and MAX=1 in noncanonical mode. This means that
     // read() will block until at least one byte is available. /
    g_signal.terminal_settings.c_cc[VTIME] = 0;
    g_signal.terminal_settings.c_cc[VMIN] = 1;
 	g_signal.terminal_settings.c_cc[VINTR] = 3; //C, sends SIGINT SIGNAL
	g_signal.terminal_settings.c_cc[VEOF] = 4;//_POSIX_VDISABLE;//4; //D
    // Set the new terminal settings.
     // Note that we don't actually check which ones were successfully
     // set and which not, because there isn't much we can do about it. /
    tcsetattr(g_signal.terminal_descriptor, TCSANOW, &g_signal.terminal_settings);

}

*/
//https://cboard.cprogramming.com/linux-programming/158476-termios-examples.html
//https://stackoverflow.com/questions/68602211/forcing-a-terminal-not-to-print-ctrl-hotkeys-when-signals-are-caughts
//https://www.mkssoftware.com/docs/man5/struct_termios.5.asp
//https://www.gnu.org/software/libc/manual/html_node/Local-Modes.html
//https://viewsourcecode.org/snaptoken/kilo/04.aTextViewer.html
//https://blog.nelhage.com/2009/12/a-brief-introduction-to-termios/s
//https://www.cons.org/cracauer/sigint.html
//https://www.gnu.org/software/libc/manual/html_node/Canonical-or-Not.html
//https://github.com/scocoyash/Text-Editor-In-C/blob/master/stex.cs
//https://en.wikibooks.org/wiki/Serial_Programming/termios
//https://stackoverflow.com/users/14701326/mampac?tab=profile
//https://www.youtube.com/watch?v=RU0ULe2f6hI

