/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:14:49 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/23 12:47:09 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*find_next_pipe(t_token *start, t_token *end)
{
	while (start && start != end)
	{
		if (start->type == PIPE)
			return (start);
		start = start->next;
	}
	return (NULL);
}
// void	init_pipe_node(t_ast *node)
// {
// 	node->ast_token.type = PIPE;
// 	node->ast_token.str = ft_strdup("|");
// 	node->fd[0] = -1;
// 	node->fd[1] = -1;
// }

// bool	check_invalid_node(t_ast *node)
// {
// 	if (!node->ast_token.str && !node->left && !node->right)
// 	{
// 		err_msg("syntax error ", "near unexpected token ", "pipe", 2);
// 		free_ast(node);
// 		return (true);
// 	}
// 	return (false);
// }

// void	build_pipe_branch(t_pipe_ctx ctx)
// {
// 	ctx.node->left = ft_calloc(1, sizeof(t_ast));
// 	if (!ctx.node->left)
// 		return ;
// 	parse_pipeline(ctx.start, ctx.pipe_pos, ctx.mini, ctx.node->left);
// 	if (check_invalid_node(ctx.node->left))
// 		return ;
// 	ctx.node->right = ft_calloc(1, sizeof(t_ast));
// 	if (!ctx.node->right)
// 		return ;
// 	parse_pipeline(ctx.pipe_pos->next, ctx.end, ctx.mini, ctx.node->right);
// 	if (check_invalid_node(ctx.node->right))
// 	{
// 		free_ast(ctx.node->right);
// 		ctx.node->right = NULL;
// 	}
// }

// void	build_cmd_node(t_token *start, t_token *end, t_mini *mini, t_ast *node)
// {
// 	node->ast_token.type = CMD;
// 	node->ast_token.str = ft_strdup("CMD");
// 	node->fd[0] = -1;
// 	node->fd[1] = -1;
// 	if (!node->ast_token.str)
// 		return ;
// 	node->cmd = build_command(start, end, mini);
// 	if (!node->cmd)
// 	{
// 		free(node->ast_token.str);
// 		node->ast_token.str = NULL;
// 	}
// }



// void	parse_pipeline(t_token *start, t_token *end, t_mini *mini, t_ast *node)
// {
// 	t_token	*pipe_pos;

// 	if (!start || start == end || !node)
// 		return ;
// 	pipe_pos = find_next_pipe(start, end);
// 	if (pipe_pos)
// 	{
// 		if (!start || start == pipe_pos || !pipe_pos->next || pipe_pos->next == end)
// 		{
// 			err_msg("syntax error ", "near unexpected token ", "pipe", 2);
// 			return ;
// 		}
// 		init_pipe_node(node);
// 		t_pipe_ctx ctx = {start, end, pipe_pos, mini, node};
// 		build_pipe_branch(ctx);
// 	}
// 	else
// 		build_cmd_node(start, end, mini, node);
// }


void	parse_pipeline(t_token *start, t_token *end, t_mini *mini, t_ast *node)
{
	t_token	*pipe_pos;

	if (!start || start == end || !node)
		return ;
	pipe_pos = find_next_pipe(start, end);
	if (pipe_pos)
	{
		if (!start || start == pipe_pos || !pipe_pos->next || pipe_pos->next == end)
		{
			err_msg("syntax error ", "near unexpected token ", "pipe", 2);
			return ;
		}
		node->ast_token.type = PIPE;
		node->ast_token.str = ft_strdup("|");
		node->fd[0] = -1;
		node->fd[1] = -1;
		if (!node->ast_token.str)
			return ;
		node->left = ft_calloc(1, sizeof(t_ast));
		if (!node->left)
			return ;
		parse_pipeline(start, pipe_pos, mini, node->left);
		if (!node->left->ast_token.str && !node->left->left && !node->left->right)
		{
			err_msg("syntax error ", "near unexpected token ", "pipe", 2);
			free_ast(node->left);
			node->left = NULL;
			return ;
		}
		node->right = ft_calloc(1, sizeof(t_ast));
		if (!node->right)
			return ;
		parse_pipeline(pipe_pos->next, end, mini, node->right);
		if (!node->right->ast_token.str && !node->right->left && !node->right->right)
		{
			err_msg("syntax error ", "near unexpected token ", "pipe", 2);
			free_ast(node->right);
			node->right = NULL;
			return ;
		}
	}
	else
	{
		node->ast_token.type = CMD;
		node->ast_token.str = ft_strdup("CMD");
		node->fd[0] = -1;
		node->fd[1] = -1;
		if (!node->ast_token.str)
			return ;
		node->cmd = build_command(start, end, mini);
		if (!node->cmd)
		{
			free(node->ast_token.str);
			node->ast_token.str = NULL;
			return ;
		}
	}
}

