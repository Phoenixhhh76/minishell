/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ast_no_export.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:05:22 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/16 11:17:13 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

	expanded = expand_if_needed(tok, mini);
	if (!expanded)
		return (i);

	if (tok->quote_type == Q_D)
		args[i++] = expanded;
	else
		i = add_split(args, i, expanded);
	return (i);
}

static int	process_token(char **args, int i, t_token *tok, t_mini *mini)
{
	if (tok->quote_type == Q_S)
		return (handle_single(args, i, tok));
	return (handle_expanded(args, i, tok, mini));
}

static int	count_args(t_token *start, t_token *end)
{
	int	count;

	count = 0;
	while (start && start != end)
	{
		if (start->type == CMD || start->type == UNKNOWN)
			count++;
		else if ((start->type == R_IN || start->type == R_OUT \
			|| start->type == R_A || \
			start->type == HD) && start->next)
			start = start->next;
		start = start->next;
	}
	return (count);
}

static char	**collect_args(t_token *start, t_token *end, t_mini *mini)
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
		else if ((start->type == R_IN || start->type == R_OUT \
			|| start->type == R_A || \
			start->type == HD) && start->next)
			start = start->next;
		start = start->next;
	}
	args[i] = NULL;
	return (args);
}

t_cmd	*build_command(t_token *start, t_token *end, t_mini *mini)
{
	t_cmd	*cmd;
	t_token	*tmp;
	int		fd;
	int		heredoc_nb;

	heredoc_nb = 0;
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
		if (tmp->type == R_IN && tmp->next)
		{
			if (cmd->infile)
				free(cmd->infile);
			cmd->last_redirin = 0;
			cmd->infile = expand_arg(tmp->next->str, mini, tmp->next->quote_type);
			tmp = tmp->next;
		}
		else if (tmp->type == R_OUT && tmp->next)
		{
			if (cmd->outfile)
				free(cmd->outfile);
			cmd->outfile = expand_arg(tmp->next->str, mini, tmp->next->quote_type);
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
				exit_error("creation outfile");
			close(fd);
			cmd->fd_out = STDOUT_FILENO;
			tmp = tmp->next;
		}
		else if (tmp->type == R_A && tmp->next)
		{
			if (cmd->outfile)
				free(cmd->outfile);
			cmd->outfile = expand_arg(tmp->next->str, mini, tmp->next->quote_type);
			if (cmd->append)
				free(cmd->append);
			cmd->append = expand_arg(tmp->next->str, mini, tmp->next->quote_type);
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
				exit_error("append creation outfile");
			close(fd);
			cmd->fd_out = STDOUT_FILENO;
			tmp = tmp->next;
		}
		else if (tmp->type == HD && tmp->next)
		{
			if (cmd->infile)
				free(cmd->infile);
			cmd->last_redirin = 1;
			cmd->infile = expand_arg(tmp->next->str, mini, tmp->next->quote_type);
			heredoc_nb++;
			tmp = tmp->next;
		}
		tmp = tmp->next;
	}
	cmd->heredoc_nb = heredoc_nb;
	if (heredoc_nb > 0)
	{
		cmd->heredoc_pipe = create_heredoc_pipe(heredoc_nb);
		cmd->heredocs = get_heredoc(heredoc_nb, start, end, cmd);
	}
	cmd->cmd_args = collect_args(start, end, mini);
	if (cmd->cmd_args && cmd->cmd_args[0])
	{
		if (cmd->cmd_args[0][0] == '/' || cmd->cmd_args[0][0] == '.')
			cmd->cmd_path = ft_strdup(cmd->cmd_args[0]);
		else //add and change
		{
			cmd->cmd_path = resolve_cmd_path(cmd->cmd_args[0], mini->env);
			if (!cmd->cmd_path)
				cmd->cmd_path = ft_strdup(cmd->cmd_args[0]);// <<<<<<for empty"""
		}
	}
	return (cmd);
}

void	init_ast(t_mini *mini)
{
	mini->ast = parse_pipeline(mini->token, NULL, mini);
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
			if (start->quote_type == Q_S)
			{
				args[i++] = ft_strdup(start->str);
			}
			else
			{
				char *expanded = expand_arg(start->str, mini);
				if (start->quote_type == Q_D)
					args[i++] = expanded;
				else // E
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
