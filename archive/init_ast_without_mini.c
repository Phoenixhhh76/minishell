/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ast_without_mini.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:48:52 by ndabbous          #+#    #+#             */
/*   Updated: 2025/05/16 11:10:59 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	count_args(t_token *start, t_token *end)
{
	int	count;

	count = 0;
	while (start && start != end)
	{
		if (start->type == CMD || start->type == UNKNOWN)
		//j'ai define UNKNOWN = -1 dans enum si non -Werror ne compile pas
			count++;
		start = start->next;
	}
	return (count);
}

static char	**collect_args(t_token *start, t_token *end)
{
	int		i;
	int		size;
	char	**args;

	i = 0;
	size = count_args(start, end);
	args = (char **)ft_calloc(size + 1, sizeof(char *));
	if (!args)
		return (NULL);
	while (start && start != end)
	{
		if (start->type == CMD || start->type == -1)
		{
			args[i] = ft_strdup(start->str);
			i++;
		}
		start = start->next;
	}
	args[i] = NULL;
	return (args);
}

t_cmd	*build_command(t_token *start, t_token *end)
{
	t_cmd	*cmd;
	t_token	*tmp;

	if (start == end) // 💥 關鍵保護：空指令不建 command
	return NULL;
	tmp = start;
	cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	//if (!node || !cmd)
		//ERROR, return (NULL);
	cmd->fd_in = -1; //to be determined;
	cmd->fd_out = -1; //to be determined;
	while (tmp && tmp != end)
	{
		if (tmp->type == R_IN && tmp->next)
		{
			cmd->infile = ft_strdup(tmp->next->str);
			tmp = tmp->next;
		}
		else if (tmp->type == R_OUT && tmp->next)
		{
			cmd->outfile = ft_strdup(tmp->next->str);
			cmd->fd_out = STDOUT_FILENO;
			tmp = tmp->next;
		}
		else if (tmp->type == R_A && tmp->next)
		{
			cmd->outfile = ft_strdup(tmp->next->str);
			cmd->fd_out = STDOUT_FILENO;
			tmp = tmp->next;
		}
		else if (tmp->type == HD && tmp->next) // à gérer plus tard
		{
			cmd->infile = ft_strdup(tmp->next->str);//need a tempfile
			tmp = tmp->next;
		}
		tmp = tmp->next;
	}
	cmd->cmd_args = collect_args(start, end, mini);
	if (cmd->cmd_args && cmd->cmd_args[0])
		cmd->cmd_path = ft_strdup(cmd->cmd_args[0]);
	return (cmd);
}

void	ft_ast_addback(t_ast **type, t_ast *new)
{
	t_ast	*tmp;

	if (!(*type))
	{
		*type = new;
		return ;
	}
	tmp = *type;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

/* replace by parse_pipeline() + create_pipe_node()

t_ast	*create_ast(t_token *pipe_pos, t_token *mini_token)
{
	t_ast	*ast;

	ast = (t_ast *)ft_calloc(sizeof(t_ast), 1);
	if (!ast)
		return (NULL);
	ast->ast_token.type = PIPE;
	ast->ast_token.str = ft_strdup("|");
	ast->fd[0] = -1;
	ast->fd[1] = -1;
	ast->left = build_command(mini_token, pipe_pos);
	ast->right = build_command(pipe_pos->next, NULL);
	ast->next = NULL;
	return (ast);
} */

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

t_ast	*parse_pipeline(t_token *start, t_token *end)
{
	t_token	*pipe_pos;
	t_ast	*ast;

	if (start == end)
	return NULL;
	pipe_pos = find_next_pipe(start, end);
	if (pipe_pos)
		return (create_pipe_node(start, pipe_pos, end));

	// case of no PIPE, just one CMD
	ast = ft_calloc(1, sizeof(t_ast));
	if (!ast)
		return (NULL);
	ast->ast_token.type = CMD;
	ast->ast_token.str = ft_strdup("CMD");
	ast->fd[0] = -1;
	ast->fd[1] = -1;
	ast->cmd = build_command(start, end);
	return (ast);
}


t_ast	*create_pipe_node(t_token *start, t_token *pipe_pos, t_token *end)
{
	t_ast	*ast;
	t_ast	*right_ast;

	if (!start || start == pipe_pos || !pipe_pos->next || pipe_pos->next == end)
	{
		fprintf(stderr, "syntax error near unexpected pipe\n");
		return NULL;
	}
	ast = ft_calloc(1, sizeof(t_ast));
	if (!ast)
		return (NULL);
	ast->ast_token.type = PIPE;
	ast->ast_token.str = ft_strdup("|");
	ast->fd[0] = -1;
	ast->fd[1] = -1;
	ast->left = parse_pipeline(start, pipe_pos);
	if (!ast->left)
	{
		fprintf(stderr, "syntax error: empty command before pipe\n");
		free(ast);
		return NULL;
	}

	// recursion
	right_ast = parse_pipeline(pipe_pos->next, end);
	// if (right_ast && right_ast->left)
	// 	ast->right = right_ast->left;
	// else if (right_ast)
	// 	ast->right = build_command(pipe_pos->next, end);
	if (!right_ast)
	{
		fprintf(stderr, "syntax error: invalid command after pipe\n");
		free(ast);
		return NULL;
	}
	ast->right = parse_pipeline(pipe_pos->next, end);
	return (ast);
}


void	init_ast(t_mini *mini)
{
	mini->ast = parse_pipeline(mini->token, NULL);
}
