/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:14:49 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/23 10:22:10 by hho-troc         ###   ########.fr       */
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

// static int	validate_pipe_pos(t_token *start, t_token *pipe_pos, t_token *end)
// {
// 	if (!start || start == pipe_pos || !pipe_pos->next || pipe_pos->next == end)
// 	{
// 		err_msg("syntax error ", "near unexpected token ", "pipe", 2);
// 		return (0);
// 	}
// 	return (1);
// }

// static	t_ast	*init_pipe_ast(void)
// {
// 	t_ast	*ast;

// 	ast = ft_calloc(1, sizeof(t_ast));
// 	if (!ast)
// 		return (NULL);
// 	ast->ast_token.type = PIPE;
// 	ast->ast_token.str = ft_strdup("|");
// 	ast->fd[0] = -1;
// 	ast->fd[1] = -1;
// 	return (ast);
// }

// t_ast	*create_pipe_node(t_token *start,
// 	t_token *pipe_pos, t_token *end, t_mini *mini)
// {
// 	t_ast	*ast;
// 	t_ast	*right_ast;

// 	if (!validate_pipe_pos(start, pipe_pos, end))
// 		return (NULL);
// 	ast = init_pipe_ast();
// 	if (!ast)
// 		return (NULL);
// 	ast->left = parse_pipeline(start, pipe_pos, mini);
// 	if (!ast->left)
// 	{
// 		err_msg("syntax error ", "near unexpected token ", "pipe", 2);
// 		free(ast);
// 		return (NULL);
// 	}
// 	right_ast = parse_pipeline(pipe_pos->next, end, mini);
// 	if (!right_ast)
// 	{
// 		err_msg("syntax error ", "near unexpected token ", "pipe", 2);
// 		if (ast->left)
// 			free_ast(ast->left);
// 		free(ast);
// 		return (NULL);
// 	}
// 	ast->right = right_ast;
// 	return (ast);
// }

// t_ast	*parse_pipeline(t_token *start, t_token *end, t_mini *mini)
// {
// 	t_token	*pipe_pos;
// 	t_ast	*ast;

// 	if (start == end)
// 		return (NULL);
// 	pipe_pos = find_next_pipe(start, end);
// 	if (pipe_pos)
// 		return (create_pipe_node(start, pipe_pos, end, mini));
// 	ast = ft_calloc(1, sizeof(t_ast));
// 	if (!ast)
// 		return (NULL);
// 	mini->ast = ast;//ADD BY NINA for heredocs leaks !
// 	ast->ast_token.type = CMD;
// 	ast->ast_token.str = ft_strdup("CMD");
// 	ast->fd[0] = -1;
// 	ast->fd[1] = -1;
// 	ast->cmd = build_command(start, end, mini);
// 	if (!ast->cmd)
// 	{
// 		free(ast->ast_token.str);
// 		free(ast);
// 		return (NULL);
// 	}
// 	return (ast);
// }

void	parse_pipeline2(t_token *start, t_token *end, t_mini *mini, t_ast *node)
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
		parse_pipeline2(start, pipe_pos, mini, node->left);
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
		parse_pipeline2(pipe_pos->next, end, mini, node->right);
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
