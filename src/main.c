/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:40:49 by hho-troc          #+#    #+#             */
/*   Updated: 2025/04/15 12:09:30 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

pid_t g_signal_pid = 0;

int	main(int ac, char **av, char **env)
{
	char	*line;
	(void)ac;
	(void)av;
	(void)env;

	while (1)
	{
		line = readline("minishell>");
		if (!line) // Ctrl+D or error
		{
			write(1, "exit\n", 5);
			break ;
		}
		add_history(line);
		free(line);
		g_signal_pid = 0;
	}
	rl_clear_history();
	//free
	return (0);
}