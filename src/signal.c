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
	printf("Received SIGINT, %d!\n", sig);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	exit(0);
}

// int	main()
// {
// 	signal(SIGINT, signal_handler);
// 	signal(SIGTERM, signal_handler);
// 	while (1) 
//	{
//
// 	}
// 	return 0;
// }

// int	main(void)
// {
// 	struct sigaction action;
// 	action.sa_handler = signal_handler;
// 	sigemptyset(&action.sa_mask);
// 	action.sa_flags = 0;

// 	sigaction(SIGINT, &action, NULL);
// 	while (1)
// 	{
// 	
// 	}
// 	return (0);
// }


// void setup_signals(void)
// {
// 	struct sigaction sa;

// 	sa.sa_handler = sigint_handler;
// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = SA_RESTART; // relance readline si interrompu
// 	sigaction(SIGINT, &sa, NULL);
// 	signal(SIGQUIT, SIG_IGN); // ignore Ctrl+\
// }

// 	pid_t pid = fork();
// 	if (pid == 0)
// 	{
// 	// dans l’enfant
// 		signal(SIGINT, SIG_DFL);   // comportement par défaut
// 		signal(SIGQUIT, SIG_DFL);

// 		execve(...);
// 		perror("execve"); // si erreur
// 		exit(1);
// 	}
// 	waitpid(pid, NULL, 0);

