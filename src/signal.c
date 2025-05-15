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
		//exit_error();
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
	signal(SIGQUIT, SIG_IGN);
}
