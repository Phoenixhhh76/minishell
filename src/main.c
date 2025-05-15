/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:20:31 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/14 16:11:25 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

pid_t g_signal_pid = 0;

static int	read_and_prepare_line(char **line)
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

static int	check_line(char *line, t_mini *mini)
{
	if (check_unclosed_quotes(line))
	{
		mini->last_exit = 2;
		return (0);
	}
	mini->token = tokenize_input(line);
	 print_token_list(mini->token);
	if (!mini->token || !check_syntax(mini->token))
	{
		mini->last_exit = 2;
		free_token_list(mini->token);
		mini->token = NULL;
		return (0);
	}
	init_ast(mini);
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

static void	exec_or_builtin(t_mini *mini)
{
	pid_t	pid;
	int		status;
	int		out_fd;
	int		in_fd;

	if (!mini->ast)
		return ;
	check_heredocs(mini->ast, mini);
	if (!is_there_pipe(mini) && ft_builtin(mini->ast))
	{
		if (mini->ast->cmd->flag_error != 1 && mini->ast->cmd->path_error != 1)
		{
			in_fd = dup(STDIN_FILENO);
			out_fd = dup(STDOUT_FILENO);
			if (has_redirection(mini->ast->cmd))
				handle_redirects(mini->ast->cmd);
			mini->last_exit = ft_run_builtin(mini->ast->cmd, mini);
			dup2(out_fd, STDOUT_FILENO);
			dup2(in_fd, STDIN_FILENO);
			close(out_fd);
			close(in_fd);
			return ;
		}
	}
	else
	{
		pid = fork();
		if (ft_builtin(mini->ast))
		{
			mini->last_exit = ft_run_builtin(mini->ast->cmd, mini);
			return ;
		}
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			exec_ast(mini->ast, mini->env);
			exit(1); // fallback if execve fails
		}
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			mini->last_exit = WEXITSTATUS(status);
	}
	// else
	// 	mini->last_exit = 1;
	close_all_heredocs(mini->ast);
}

static void	safe_cleanup(t_mini *mini, char *line)
{
	free_token_list(mini->token);
	mini->token = NULL;
	// free_ast(mini->ast); // ← TODO if implemented
	free(line);
	g_signal_pid = 0;
}
/* have to choose a good name */
static int	read_and_prepare_line(char **line);
static int	check_line(char *line, t_mini *mini);
static void	exec_or_builtin(t_mini *mini);
static void	safe_cleanup(t_mini *mini, char *line);

int	main(int ac, char **av, char **envp)
{
	t_mini	mini;
	char	*line;

	(void)ac;
	init_mini(&mini, av, envp);
	mini.last_exit = 0;
	ft_setup_signals();
	while (1)
	{
		if (read_and_prepare_line(&line))
			break ;
		if (check_line(line, &mini))
		{
			exec_or_builtin(&mini);
		}
		safe_cleanup(&mini, line);
	}
	rl_clear_history();
	return (0);
}
