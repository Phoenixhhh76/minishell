/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:20:31 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/23 10:14:44 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

pid_t g_signal_pid = 0;

int	read_and_prepare_line(char **line)
{
	g_signal_pid = 0;
	*line = readline("minishell> ");
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

// int	check_line(char *line, t_mini *mini)
// {
// 	if (check_unclosed_quotes(line))
// 	{
// 		mini->last_exit = 2;
// 		return (0);
// 	}
// 	mini->token = tokenize_input(line);
// 	if (!check_syntax(mini->token))
// 	{
// 		mini->last_exit = 2;
// 		free_token_list(mini->token);
// 		mini->token = NULL;
// 		return (2);
// 	}
// 	mini->ast = parse_pipeline(mini->token, NULL, mini);
// 	//print_ast(mini->ast, 10);
// 	return (1);
// }

int	check_line(char *line, t_mini *mini)
{
	if (check_unclosed_quotes(line))
	{
		mini->last_exit = 2;
		return (0);
	}
	mini->token = tokenize_input(line);
	if (!check_syntax(mini->token))
	{
		mini->last_exit = 2;
		free_token_list(mini->token);
		mini->token = NULL;
		return (2);
	}
	mini->ast = ft_calloc(1, sizeof(t_ast));
	if (!mini->ast)
		return (0); //need to specify
	parse_pipeline2(mini->token, NULL, mini, mini->ast);
	return (1);
}

bool	is_there_pipe(t_mini *mini)
{
	t_token	tmp;

	tmp = mini->ast->ast_token;
	if (tmp.type == PIPE)
		return (true);
	return (false);
}

bool	has_redirection(t_cmd *cmd)
{
	if (cmd->infile || cmd->outfile || cmd->heredoc_nb > 0)
		return (true);
	return (false);
}

void	exec_or_builtin(t_mini *mini)
{
	pid_t	pid;
	int		status;
	int		out_fd;
	int		in_fd;
	int		fd;

	if (!mini->ast)
		return ;
	if (!is_there_pipe(mini) && ft_builtin(mini->ast))
	{
		if (mini->ast->cmd->in_error != 1 && mini->ast->cmd->path_error != 1)
		{
			in_fd = dup(STDIN_FILENO);
			out_fd = dup(STDOUT_FILENO);
			if (has_redirection(mini->ast->cmd))
				handle_redirects(mini->ast->cmd);
			mini->last_exit = ft_run_builtin(mini, mini->ast->cmd);
			dup2(out_fd, STDOUT_FILENO);
			fd = open("/dev/tty", O_RDONLY);
			if (fd != -1)
				dup2(fd, STDIN_FILENO);
			close(fd);
			// dup2(in_fd, STDIN_FILENO);
			close(out_fd);
			close(in_fd);
			return ;
		}
	}
	else
	{
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			mini->last_exit = 1;
		}
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			exec_ast(mini, mini->ast, mini->env);
			exit(mini->last_exit);
		}
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				mini->last_exit = 130;
			else if (WTERMSIG(status) == SIGQUIT)
				mini->last_exit = 131;
			// else
			// {
			// 	mini->last_exit = 128 + WTERMSIG(status);
			// //	dprintf(2, "     MINI EXIT = %i\n", mini->last_exit);
			// }
		}
		else if (WIFEXITED(status))
				mini->last_exit = WEXITSTATUS(status);
	}
	close_all_heredocs(mini->ast);
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
		if (read_and_prepare_line(&line))
			break ;
		if (g_signal_pid == SIGINT)
			mini.last_exit = 130;
		if (!check_line(line, &mini) || mini.stop_hd)
		{
			safe_cleanup(&mini, line);
			continue ;
		}
		exec_or_builtin(&mini);
		safe_cleanup(&mini, line);
	}
	safe_cleanup(&mini, NULL);
	free_double_tab(mini.env);
	free_double_tab(mini.exp_list);
	rl_clear_history();
	return (0);
}
