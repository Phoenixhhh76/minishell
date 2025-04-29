/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:40:49 by hho-troc          #+#    #+#             */
/*   Updated: 2025/04/25 16:43:25 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

pid_t g_signal_pid = 0;

// void	init_mini(t_mini *mini, char **av, char **env)
// {
// 	mini->token = NULL;
// 	mini->ast = NULL;
// 	mini->env = env;
// 	mini->av = av;
// }

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

// int	main(int ac, char **av, char **envp)
// {
// 	t_mini	mini;
// 	char	*line;

// 	(void)ac;
// 	init_mini(&mini, av, envp);

// 	while (1)
// 	{
// 		line = readline("minishell> ");
// 		if (!line)
// 		{
// 			//exit_shell();
// 			write(1, "exit\n", 5);
// 			break ;
// 		}
// 		if (line[0] == '\0')
// 		{
// 			free(line);
// 			continue ;
// 		}
// 		add_history(line);

// 		// Tokenize + Parse
// 		mini.token = tokenize_input(line);
// 		if (!mini.token)
// 		{
// 			free(line);
// 			continue ;
// 		}
// 		init_ast(&mini); // create mini.ast

// 		// Fork + 執行 AST
// 		if (mini.ast)
// 		{
// 			pid_t pid = fork();
// 			if (pid == 0)
// 			{
// 				exec_ast(mini.ast, mini.env);
// 				exit(1);
// 			}
// 			waitpid(pid, NULL, 0);
// 		}

// 		free_token_list(mini.token);
// 		// free_ast(mini.ast); TODO
// 		free(line);
// 		g_signal_pid = 0;
// 	}
// 	rl_clear_history();
// 	return (0);
// }

int	main(int ac, char **av, char **envp)
{
	pid_t	pid;
	t_mini	mini;
	char	*line;

	(void)ac;
	init_mini(&mini, av, envp);
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			//exit_minishell();
			write(1, "exit\n", 5);
			break ;
		}
		if (line[0] == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		mini.token = tokenize_input(line);
		//print_token_list(mini.token); //tet
		if (!mini.token)
		{
			free(line);
			continue ;
		}
		init_ast(&mini);
		//print_ast(mini.ast, 5);
		if (mini.ast)
		{
			check_heredocs(mini.ast);
			if (ft_builtin(mini.ast, &mini.env))
			{
				// free_token_list(mini.token);
				// free_ast(mini.ast);
				// free(line);
				continue ;
			}
			else
			{
				pid = fork();
				if (pid == 0)
				{
					exec_ast(mini.ast, mini.env);
					exit(1);
				}
				waitpid(pid, NULL, 0);
				close_all_heredocs(mini.ast);
			}
		}
		free_token_list(mini.token);
		// free_ast(mini.ast); TODO
		free(line);
		g_signal_pid = 0;
	}
	rl_clear_history();
	return (0);
}
