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

pid_t g_signal_pid = 0;

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
	// if (g_signal_pid == SIGINT)
	// 	mini->last_exit = 130;//not working when @ new echo after CTRL+C
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
	{
		mini->last_exit = 2;
		return (0);
	}
	if (only_spaces(line))
		return (0);
	mini->token = tokenize_input(line, mini);
	//print_token_list(mini->token);
	//debug_tokens_type(mini->token);
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
		return (0); //need to specify
	parse_pipeline(mini->token, NULL, mini, mini->ast);
	//print_ast(mini->ast, 10);
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
			// if (!ft_strcmp(mini->ast->cmd->cmd_args, "exit"))
			// {
			// 	mini->in_fd = in_fd;
			// 	mini->out_fd = out_fd;
			// }
			printf(" DEBUG 1 = %i\n", mini->last_exit);
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
			safe_exit(mini, mini->last_exit);
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
	printf(" DEBUG exec ast = %i\n", mini->last_exit);
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
		if (read_and_prepare_line(&line, &mini))
			break ;
		if (!check_line(line, &mini) || mini.stop_hd)
		{
			safe_cleanup(&mini, line);
			continue ;
		}
		exec_or_builtin(&mini);
		// if (g_signal_pid == SIGINT)
		// {
		// 	mini.last_exit = 130;
		// //	safe_cleanup(&mini, line);  //  line + token + ast
		// // 	continue;
		// }
		safe_cleanup(&mini, line);
	}
	safe_exit(&mini, 0);
	return (0);
}
