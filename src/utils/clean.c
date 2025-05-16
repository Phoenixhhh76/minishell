/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:20:37 by ndabbous          #+#    #+#             */
/*   Updated: 2025/05/16 17:28:02 by hho-troc         ###   ########.fr       */
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
	cmd->flag_error = 0;
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

// void	free_ast(t_ast *ast)
// {
// 	t_ast	*tmp;

// 	tmp = NULL;
// 	if (!ast)
// 		return ;
// 	while (tmp)
// 	{
// 		tmp = ast;
// 		free_cmd(tmp->cmd);
// 		free_ast(tmp->left);
// 		free_ast(tmp->right);
// 		free_token_list(&tmp->ast_token);
// 		tmp->fd[0] = -1;
// 		tmp->fd[1] = -1;
// 		free(tmp);
// 		tmp = tmp->next;
// 	}
// 	free(tmp);
// }

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
		free(ast->ast_token.str);
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
	// if (write)
	// 	ft_fprintf(1, "exit\n");
	exit(code);
}

// int	err_msg(char *str1, char *str2, char *str3, int erno)
// {
// 	ft_putstr_fd("minishell: ", 2);
// 	ft_putstr_fd(str1, 2);
// 	ft_putstr_fd(str2, 2);
// 	ft_putendl_fd(str3, 2);
// 	return (erno);
// }

// void	ft_close_fds(t_mini *mini)
// {
// 	if (mini->pipe_fd[0] != -1)
// 		close(mini->pipe_fd[0]);
// 	if (mini->pipe_fd[1] != -1)
// 		close(mini->pipe_fd[1]);
// 	mini->pipe_fd[0] = -1;
// 	mini->pipe_fd[1] = -1;
// }

// static void	ft_conditional_free(void *ptr, void (*func)(void *))
// {
// 	if (ptr)
// 		func(ptr);
// }

// void	ft_exit_error(t_mini *mini, char *str, int code)
// {
// 	ft_fprintf(ERR_OUT, "%s\n", str);
// 	ft_exit_clean(mini, code, false);
// }

//reset --> free + initialize to 0 to prevent invalid free