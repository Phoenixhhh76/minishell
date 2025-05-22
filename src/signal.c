/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndabbous <ndabbous@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-07 08:22:25 by ndabbous          #+#    #+#             */
/*   Updated: 2025-05-07 08:22:25 by ndabbous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	//write(1, "\n", 1);
	//rl_replace_line("", 0);
	//rl_done = 1;
	//g_signal_pid = SIGINT;
	//safe_exit(static_struct(NULL), 130);
	if (sig == SIGINT)
	{
	//	dprintf(2, "         (sigint_handler) if (sig == SIGINT)\n");
		g_signal_pid = SIGINT;
		write(STDOUT_FILENO, "\n", 1);
		close(0);
	}
}

void	signal_handler(int sig)
{
	//(void)sig;
	//printf("Received SIGINT, %d!\n", sig);
	if (sig == SIGINT)
	{
		g_signal_pid = SIGINT;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGTERM)
	{
		write(STDOUT_FILENO, "\n", 1);
		safe_exit(static_struct(NULL), 1);
	}
}

void	ft_setup_signals(void)
{
	t_sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = signal_handler;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	//signal(SIGQUIT, SIG_IGN);
	sigaction(SIGQUIT, &sa, NULL);
	// sa.sa_handler = signal_handler;
	// sigemptyset(&sa.sa_mask);
	// sa.sa_flags = SA_RESTART;

	// sigaction(SIGINT, &sa, NULL);         // CTRL+C : affiche prompt à la ligne suivante
	// signal(SIGQUIT, SIG_IGN);            // CTRL+\ : ignoré
	// signal(SIGTSTP, SIG_IGN);            // CTRL+Z : ignoré
	// signal(SIGTTIN, SIG_IGN);            // job control
	// signal(SIGTTOU, SIG_IGN);

}

// int	get_exit_status(int status)
// {
// 	if (WIFEXITED(status))
// 		return (WEXITSTATUS(status));
// 	else if (WIFSIGNALED(status))
// 		return (128 + WTERMSIG(status));
// 	return (1);
// }
