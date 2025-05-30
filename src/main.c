/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:20:31 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/27 17:16:08 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

pid_t	g_signal_pid = 0;

int	read_and_prepare_line(char **line, t_mini *mini)
{
	g_signal_pid = 0;
	*line = readline("minishell> ");
	(void)mini;
	if (!*line)
	{
		write(1, "exit\n", 5);
		return (1);
	}
	if ((*line)[0] == '\0')
		return (0);
	add_history(*line);
	return (0);
}

bool	only_spaces(const char *line)
{
	while (*line)
	{
		if (!ft_isspace(*line))
			return (false);
		line++;
	}
	return (true);
}

int	check_line(char *line, t_mini *mini)
{
	if (check_unclosed_quotes(line))
		return (mini->last_exit = 2, 0);
	if (only_spaces(line))
		return (0);
	mini->token = tokenize_input(line, mini);
	if (!mini->token || !check_syntax(mini->token))
	{
		mini->last_exit = 2;
		free_token_list(mini->token);
		mini->token = NULL;
		if (mini->ast)
		{
			free_ast(mini->ast);
			mini->ast = NULL;
		}
		return (2);
	}
	mini->ast = ft_calloc(1, sizeof(t_ast));
	if (!mini->ast)
		return (mini->ast = NULL, -2);
	if (parse_pipeline(mini->token, NULL, mini, mini->ast) == -2)
		return (-2);
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	t_mini	mini;
	char	*line;

	(void)ac;
	init_mini(&mini, av, envp);
	mini.last_exit = 0;
	static_struct(&mini);
	while (1)
	{
		ft_setup_signals();
		if (read_and_prepare_line(&line, &mini))
			break ;
		if (!check_line(line, &mini) || mini.stop_hd)
		{
			safe_cleanup(&mini, line);
			continue ;
		}
		exec_or_builtin(&mini);
		safe_cleanup(&mini, line);
	}
	safe_exit(&mini, 0);
	return (0);
}
