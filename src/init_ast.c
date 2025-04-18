/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndabbous <ndabbous@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-15 14:48:52 by ndabbous          #+#    #+#             */
/*   Updated: 2025-04-15 14:48:52 by ndabbous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	count_args(t_token *start, t_token *end)
{
	int	count;

	count = 0;
	while (start && start != end)
	{
		if (start->type == CMD || start->type == -1)
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
	args = (char **)ft_calloc(sizeof(t_ast*), size + 1);
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

t_cmd	*build_command(t_ast *ast, t_token *start, t_token *end)
{
	t_ast	*node;
	t_cmd	*cmd;
	t_token	*tmp;

	tmp = start;
	node = (t_ast *)ft_calloc(sizeof(t_ast), 1);
	cmd = (t_cmd *)ft_calloc(sizeof(t_cmd), 1);
	//if (!node || !cmd)
		//ERROR, return (NULL);
	cmd->fd_in = -1; //to be determined;
	cmd->fd_out = -1; //to be determined;
	while (tmp && tmp != end)
	{
		if (tmp->type == REDIR_IN && tmp->next)
		{
			cmd->infile = ft_strdup(tmp->next->str);
			tmp = tmp->next;
		}
		else if (tmp->type == REDIR_OUT && tmp->next)
		{
			cmd->outfile = ft_strdup(tmp->next->str);
			cmd->fd_out = STDOUT_FILENO;
			tmp = tmp->next;
		}
		// else if (tmp->type == REDIR_APPEND && tmp->next)
		// {
		// 	cmd->outfile = ft_strdup(tmp->next->str);
		// 	tmp = tmp->next;
		// }
		// else if (tmp->token == HEREDOC && tmp->next) // à gérer plus tard
		// {
		// 	tmp = tmp->next;
		// }
		tmp = tmp->next;
	}
	cmd->cmd_args = collect_args(start, end);
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
	ast->left = build_command(ast, mini_token, pipe_pos);
	ast->right = build_command(ast, pipe_pos->next, NULL);
	ast->next = NULL;
	return (ast);
}

void	init_ast(t_mini *mini)
{
	t_ast	*new;
	t_token	*token_last_pipe;
	t_token	*pipe_pos;

	new = NULL;
	token_last_pipe = mini->token;
	pipe_pos = NULL;
	while (token_last_pipe)
	{
		if (token_last_pipe->type == PIPE)
			pipe_pos = token_last_pipe;
		token_last_pipe = token_last_pipe->next;
	}
	if (pipe_pos)
	{
		new = create_ast(pipe_pos, mini->token);
		ft_ast_addback(&mini->ast, new);
	//	printf("Token_ast: %-10s Type: %d\n", mini->ast->ast_token.str, mini->ast->ast_token.type);
		pipe_pos->type = -1;
		init_ast(mini);
	}
	if (pipe_pos == NULL)
	{
		new = create_ast(pipe_pos, mini->token);
		build_command(new, mini->token, NULL);
	}
}

/* 


under is the tests 


*/

void	init_mini(t_mini *mini, t_token *token)
{
	mini->token = token;
	init_ast(mini);
	mini->env = NULL;
	mini->av = NULL;
	//mini->ast = ast;
}

void	print_token_list(t_token *token)
{
	while (token)
	{
		printf("Token: %-10s Type: %d\n", token->str, token->type);
		token = token->next;
	}
}

void	print_mini(t_mini *mini)
{
	t_token	*tmp;
	t_ast	*tmp_ast;

	tmp = mini->token;
	tmp_ast = mini->ast;
	printf("Mini %p\n", tmp);
	while (tmp)
	{
		printf("Token: %-10s Type: %d\n", tmp->str, tmp->type);
		//printf("Ast_token: %d\n", tmp_ast->fd[0]);
		tmp = tmp->next;
		//tmp_ast = tmp_ast->next;
	}
}

int	main()
{
	t_mini	mini;
	t_ast	*ast;
	t_token	*stack;
	t_token	token;
	t_token	token2;
	t_token	token3;
	t_token	token4;
	t_token	token5;

	token.type = 0;
	token.next = 0;
	token2.type = 1;
	token2.next = 0;
	token3.type = 0;
	token3.next = 0;
	token4.type = 1;
	token4.next = 0;
	token5.type = 2;
	token5.next = 0;
	stack = &token;
	stack->next = &token2;
	stack->next->next = &token3;
	stack->next->next->next = &token4;
	stack->next->next->next->next = &token5;
	init_mini(&mini, stack);
	print_mini(&mini);
	return (0);
}
