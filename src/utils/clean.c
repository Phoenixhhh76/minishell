/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:20:37 by ndabbous          #+#    #+#             */
/*   Updated: 2025/05/28 16:52:50 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	reset_cmd(t_cmd *cmd)
{
	cmd->cmd_args = NULL;
	cmd->cmd_path = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = NULL;
	cmd->fd_in = -1;
	cmd->fd_out = -1;
	cmd->pipe = NULL;
	cmd->pids = NULL;
	cmd->last_redirin = 0;
	cmd->heredocs = NULL;
	cmd->heredoc_nb = 0;
	cmd->heredoc_pipe = NULL;
	cmd->flag_hd = 0;
	cmd->in_error = 0;
	cmd->path_error = 0;
	cmd->heredocs_quote = NULL;
}

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->cmd_args)
		free_double_tab(cmd->cmd_args);
	if (cmd->cmd_path)
		free(cmd->cmd_path);
	if (cmd->infile)
		free(cmd->infile);
	if (cmd->outfile)
		free(cmd->outfile);
	if (cmd->append)
		free(cmd->append);
	if (cmd->pipe)
		free(cmd->pipe);
	if (cmd->pids)
		free(cmd->pids);
	if (cmd->heredocs)
		free_double_tab(cmd->heredocs);
	if (cmd->heredoc_pipe)
		ft_free_tab_int(cmd->heredoc_pipe, cmd->heredoc_nb);
	if (cmd->heredocs_quote)
		free(cmd->heredocs_quote);
	reset_cmd(cmd);
	free(cmd);
}

void	free_ast(t_ast *ast)
{
	if (!ast)
		return ;
	if (ast->left)
		free_ast(ast->left);
	if (ast->right)
		free_ast(ast->right);
	if (ast->cmd)
		free_cmd(ast->cmd);
	if (ast->ast_token.str)
	{
		free(ast->ast_token.str);
		ast->ast_token.str = NULL;
	}
	free(ast);
}

void	safe_cleanup(t_mini *mini, char *line)
{
	if (line)
		free(line);
	if (mini->token)
	{
		free_token_list(mini->token);
		mini->token = NULL;
	}
	if (mini->ast)
	{
		free_ast(mini->ast);
		mini->ast = NULL;
	}
	mini->stop_hd = 0;
	g_signal_pid = 0;
}

void	safe_exit(t_mini *mini, int code)
{
	if (mini->env)
		free_double_tab(mini->env);
	if (mini->exp_list)
		free_double_tab(mini->exp_list);
	if (mini->ast)
		free_ast(mini->ast);
	if (mini->token)
		free_token_list(mini->token);
	rl_clear_history();
	exit(code);
}

