/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:46:11 by ndabbous          #+#    #+#             */
/*   Updated: 2025/06/01 11:38:04 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pipe_syntax_error(t_ast *node)
{
	err_msg("syntax error ", "near unexpected token ", "pipe", 2);
	free_ast(node);
	return (-2);
}

int	init_pipe_node(t_ast *node)
{
	node->ast_tok.type = PIPE;
	node->ast_tok.str = ft_strdup("|");
	node->fd[0] = -1;
	node->fd[1] = -1;
	if (!node->ast_tok.str)
	{
		node->ast_tok.str = NULL;
		return (perror("parse_pipeline : strdup"), -2);
	}
	return (0);
}

int	handle_left_branch(t_token *start, t_token *pipe_pos,
						t_mini *mini, t_ast *node)
{
	node->left = ft_calloc(1, sizeof(t_ast));
	if (!node->left)
		return (perror("parse_pipeline : left calloc"), -2);
	if (parse_pipeline(start, pipe_pos, mini, node->left) == -2)
		return (-2);
	if (!node->left->ast_tok.str && !node->left->left && !node->left->right)
		return (handle_pipe_syntax_error(node->left));
	return (0);
}

int	handle_right_branch(t_token *pipe_pos, t_token *end,
						t_mini *mini, t_ast *node)
{
	node->right = ft_calloc(1, sizeof(t_ast));
	if (!node->right)
		return (perror("parse_pipeline : right calloc"), -2);
	if (parse_pipeline(pipe_pos->next, end, mini, node->right) == -2)
		return (-2);
	if (!node->right->ast_tok.str && !node->right->left && !node->right->right)
		return (handle_pipe_syntax_error(node->right));
	return (0);
}

int	handle_cmd_node(t_token *start, t_token *end, t_mini *mini, t_ast *node)
{
	node->ast_tok.type = CMD;
	node->ast_tok.str = ft_strdup("CMD");
	node->fd[0] = -1;
	node->fd[1] = -1;
	if (!node->ast_tok.str)
		return (perror("parse_pipeline : strdup"), -2);
	node->cmd = build_command(start, end, mini);
	if (!node->cmd)
	{
		free(node->ast_tok.str);
		node->ast_tok.str = NULL;
		return (perror("parse_pipeline : !node_cmd"), -2);
	}
	return (0);
}
