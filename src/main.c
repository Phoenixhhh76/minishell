/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:40:49 by hho-troc          #+#    #+#             */
/*   Updated: 2025/04/18 19:55:01 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

pid_t g_signal_pid = 0;

/* void	init_mini(t_mini mini, char **av, char **env)
{
	t_ast	*ast;
	t_token	*type;

	init_token(&type);
	// if (!token)
	// 	//ERROR;
	mini.token = type;
	init_ast(&mini, ast);
	// if (!ast)
	// 	//ERROR;
	mini.env = env;
	// if (!env)
	// 	//ERROR;
	mini.av = av;
	mini.ast = ast;
} */

void	init_mini(t_mini *mini, char **av, char **env)
{
	mini->token = NULL;
	mini->ast = NULL;
	mini->env = env;
	mini->av = av;
}

/* int	main(int ac, char **av, char **env)
{
	t_mini	mini;
	char	*line;
	(void)ac;
	//(void)av;
	//(void)env;

	init_mini(mini, av, env);
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
} */
int	main(int ac, char **av, char **envp)
{
	t_mini	mini;
	char	*line;

	(void)ac;
	init_mini(&mini, av, envp);

	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (line[0] == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);

		// Tokenize + Parse
		mini.token = tokenize_input(line);
		if (!mini.token)
		{
			free(line);
			continue ;
		}
		init_ast(&mini); // create mini.ast

		// Fork + 執行 AST
		if (mini.ast)
		{
			pid_t pid = fork();
			if (pid == 0)
			{
				exec_ast(mini.ast, mini.env);
				exit(1);
			}
			waitpid(pid, NULL, 0);
		}

		free_token_list(mini.token);
		// free_ast(mini.ast); TODO
		free(line);
		g_signal_pid = 0;
	}
	rl_clear_history();
	return (0);
}