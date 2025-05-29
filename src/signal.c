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
	if (sig == SIGINT)
	{
		g_signal_pid = SIGINT;
		write(STDOUT_FILENO, "\n", 1);
		close(0);
	}
}

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		static_struct(NULL)->last_exit = 130;
		g_signal_pid = SIGINT;
	}
	else if (sig == SIGTERM)
	{
		write(STDOUT_FILENO, "\n", 1);
		safe_exit(static_struct(NULL), 1);
	}
}

void	signal_handler_child(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		static_struct(NULL)->last_exit = 130;
		g_signal_pid = SIGINT;
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
	sigaction(SIGQUIT, &sa, NULL);
}
