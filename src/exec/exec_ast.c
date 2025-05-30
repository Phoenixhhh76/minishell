/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 20:06:06 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/26 10:25:06 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_pipe_child_left(t_mini *mini, t_ast *node, char **envp)
{
	close(node->fd[0]);
	dup2(node->fd[1], STDOUT_FILENO);
	close(node->fd[1]);
	exec_ast(mini, node->left, envp);
	safe_exit(mini, mini->last_exit);
}

void	exec_pipe_child_right(t_mini *mini, t_ast *node, char **envp)
{
	close(node->fd[1]);
	dup2(node->fd[0], STDIN_FILENO);
	close(node->fd[0]);
	exec_ast(mini, node->right, envp);
	safe_exit(mini, mini->last_exit);
}

static void	exec_pipe_forks(t_mini *mini, t_ast *node, char **envp, pid_t *pids)
{
	pids[0] = fork();
	if (pids[0] < 0)
	{
		perror("left_pid fork");
		mini->last_exit = -2;
	}
	if (pids[0] == 0)
		exec_pipe_child_left(mini, node, envp);
	pids[1] = fork();
	if (pids[1] < 0)
	{
		perror("right_pid fork");
		mini->last_exit = -2;
	}
	if (pids[1] == 0)
		exec_pipe_child_right(mini, node, envp);
}

void	exec_pipe_node(t_mini *mini, t_ast *node, char **envp)
{
	int		status;
	pid_t	pids[2];

	if (pipe(node->fd) == -1)
		perror("pipe");
	exec_pipe_forks(mini, node, envp, pids);
	close(node->fd[0]);
	close(node->fd[1]);
	waitpid(pids[1], &status, 0);
	waitpid(pids[0], NULL, 0);
	safe_exit(mini, status / 256);
}

void	exec_ast(t_mini *mini, t_ast *node, char **envp)
{
	if (!node)
		return ;
	if (node->ast_tok.type == PIPE)
		exec_pipe_node(mini, node, envp);
	else if (node->ast_tok.type == CMD)
	{
		if (ft_builtin(node))
			ft_run_builtin(mini, node->cmd);
		else
			exec_cmd_node(mini, node, envp);
	}
}
