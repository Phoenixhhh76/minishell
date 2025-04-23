/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:05:22 by hho-troc          #+#    #+#             */
/*   Updated: 2025/04/23 14:16:17 by hho-troc         ###   ########.fr       */
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
			count++;
		start = start->next;
	}
	return (count);
}

/* static char	**collect_args(t_token *start, t_token *end, t_mini *mini)
{
	int		i;
	int		size;
	char	**args;

	(void)mini;
	i = 0;
	size = count_args(start, end);
	args = (char **)ft_calloc(size + 1, sizeof(char *));
	if (!args)
		return (NULL);
	while (start && start != end)
	{
		if (start->type == CMD || start->type == UNKNOWN)
		{
			//args[i] = ft_strdup(start->str);
			args[i] = expand_arg(start->str, mini);//here will expand
			i++;
		}
		start = start->next;
	}
	args[i] = NULL;
	return (args);
} */
void	free_split(char **split)
{
	int	i;

	if (!split)
		return;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

static int	handle_single(char **args, int i, t_token *tok)
{
	args[i++] = ft_strdup(tok->str);
	return (i);
}

static int	add_split(char **args, int i, char *expanded)
{
	char	**split;
	int		j;

	split = ft_split(expanded, ' ');
	j = 0;
	while (split && split[j])
		args[i++] = ft_strdup(split[j++]);
	free(expanded);
	free_split(split);
	return (i);
}

static int	handle_expanded(char **args, int i, t_token *tok, t_mini *mini)
{
	char	*expanded;

	expanded = expand_arg(tok->str, mini);
	if (!expanded)
		return (i);
	if (tok->quote_type == QUOTE_DOUBLE)
		args[i++] = expanded;
	else
		i = add_split(args, i, expanded);
	return (i);
}

static int	process_token(char **args, int i, t_token *tok, t_mini *mini)
{
	if (tok->quote_type == QUOTE_SINGLE)
		return (handle_single(args, i, tok));
	return (handle_expanded(args, i, tok, mini));
}

static char **collect_args(t_token *start, t_token *end, t_mini *mini)
{
	int		i;
	int		size;
	char	**args;

	size = count_args(start, end);
	args = (char **)ft_calloc(size + 1, sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	while (start && start != end)
	{
		if (start->type == CMD || start->type == UNKNOWN)
			i = process_token(args, i, start, mini);
		start = start->next;
	}
	args[i] = NULL;
	return (args);
}

/*
static char	**collect_args(t_token *start, t_token *end, t_mini *mini)
{
	int		i;
	int		size;
	char	**args;

	(void)mini;
	i = 0;
	size = count_args(start, end);
	args = (char **)ft_calloc(size + 1, sizeof(char *));
	if (!args)
		return (NULL);
	while (start && start != end)
	{
		if (start->type == CMD || start->type == UNKNOWN)
		{
			if (start->quote_type == QUOTE_SINGLE)
			{
				args[i++] = ft_strdup(start->str);
			}
			else
			{
				char *expanded = expand_arg(start->str, mini);
				if (start->quote_type == QUOTE_DOUBLE)
					args[i++] = expanded;
				else // QUOTE_NONE
				{
					char **split = ft_split(expanded, ' ');
					for (int j = 0; split && split[j]; j++)
						args[i++] = ft_strdup(split[j]);
					free(expanded);
					free_split(split);
				}
			}
		}
		start = start->next;
	}
	args[i] = NULL;
	return (args);
} */

t_cmd	*build_command(t_token *start, t_token *end, t_mini *mini)
{
	t_cmd	*cmd;
	t_token	*tmp;

	if (start == end)
		return (NULL);
	tmp = start;
	cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	//if (!node || !cmd)
		//ERROR, return (NULL);
	cmd->fd_in = -1; //to be determined;
	cmd->fd_out = -1; //to be determined;
	while (tmp && tmp != end)
	{
		if (tmp->type == REDIR_IN && tmp->next)
		{
			cmd->infile = expand_arg(tmp->next->str, mini);
			tmp = tmp->next;
		}
		else if (tmp->type == REDIR_OUT && tmp->next)
		{
			cmd->outfile = expand_arg(tmp->next->str, mini);
			cmd->fd_out = STDOUT_FILENO;
			tmp = tmp->next;
		}
		else if (tmp->type == REDIR_APPEND && tmp->next)
		{
			cmd->outfile = expand_arg(tmp->next->str, mini);
			cmd->fd_out = STDOUT_FILENO;
			tmp = tmp->next;
		}
 		else if (tmp->type == HEREDOC && tmp->next) // à gérer plus tard in W2, and '$USER' cant expand
		{
			// if (is_quoted(tmp->next->str)) // function is_quoted need to creat
			// 	cmd->infile = ft_strdup(tmp->next->str);
			// else
			cmd->infile = expand_arg(tmp->next->str, mini);//need a tempfile
			tmp = tmp->next;
		}
/* 		else if (tmp->type == HEREDOC && tmp->next)
		{
			cmd->heredoc_tmpfile = create_tmpfile(tmp->next->str);
			tmp = tmp->next;
		} */

		tmp = tmp->next;
	}
	cmd->cmd_args = collect_args(start, end, mini);
//	if (cmd->cmd_args && cmd->cmd_args[0])
//		cmd->cmd_path = ft_strdup(cmd->cmd_args[0]);
//change to resolve_cmd_path
	if (cmd->cmd_args && cmd->cmd_args[0])
	{
		if (cmd->cmd_args[0][0] == '/' || cmd->cmd_args[0][0] == '.')
			cmd->cmd_path = ft_strdup(cmd->cmd_args[0]);
		else
			cmd->cmd_path = resolve_cmd_path(cmd->cmd_args[0], mini->env);
	}
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


void	init_ast(t_mini *mini)
{
	mini->ast = parse_pipeline(mini->token, NULL, mini);
}
