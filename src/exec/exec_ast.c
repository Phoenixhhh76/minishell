/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 20:06:06 by hho-troc          #+#    #+#             */
/*   Updated: 2025/04/18 20:06:30 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exec_ast(t_ast *node, char **envp)
{
	if (!node)
		return;

	// ─────────────────────────────────────────────
	if (node->ast_token.type == PIPE)
	{
		if (pipe(node->fd) == -1)
		{
			perror("pipe");
			exit(1);
		}

		pid_t left_pid = fork();
		if (left_pid == 0)
		{
			// 子進程處理左邊，寫入 pipe
			close(node->fd[0]);
			dup2(node->fd[1], STDOUT_FILENO);
			close(node->fd[1]);

			exec_ast(node->left, envp);
			exit(1); // 若失敗
		}

		pid_t right_pid = fork();
		if (right_pid == 0)
		{
			// 子進程處理右邊，讀取 pipe
			close(node->fd[1]);
			dup2(node->fd[0], STDIN_FILENO);
			close(node->fd[0]);

			exec_ast(node->right, envp);
			exit(1);
		}

		// 父進程收尾
		close(node->fd[0]);
		close(node->fd[1]);
		waitpid(left_pid, NULL, 0);
		waitpid(right_pid, NULL, 0);
	}
	// ─────────────────────────────────────────────
	else if (node->ast_token.type == CMD && node->cmd)
	{
		// 處理 input redirection
		if (node->cmd->infile)
		{
			int fd = open(node->cmd->infile, O_RDONLY);
			if (fd < 0)
			{
				perror("open infile");
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}

		// 處理 output redirection
		if (node->cmd->outfile)
		{
			int fd = open(node->cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				perror("open outfile");
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}

		// 處理 builtin
		if (ft_is_builtin(node->cmd->cmd_args[0]))
		{
			ft_run_builtin(node->cmd, &envp); // 不進 fork 執行的是副本
			exit(0);
		}

		// 外部指令
		execve(node->cmd->cmd_path, node->cmd->cmd_args, envp);
		perror("execve");
		exit(1);
	}
}
