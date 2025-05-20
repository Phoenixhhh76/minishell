/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ast _all.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:05:22 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/16 12:07:07 by hho-troc         ###   ########.fr       */
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
/*
the condition strchr ", make sure if we have a string like
$HOLA="  bonjour he ",have space at the beginning and end
Token [wesh"$HOLA"]  quote_type 0, we won't split it, we keep "  bonjour he "
 */
static int	handle_expanded(char **args, int i, t_token *tok, t_mini *mini)
{
	char	*expanded;

	expanded = expand_if_needed(tok, mini);
	if (!expanded)
		return (i);
	if (tok->quote_type == Q_D || ft_strchr(tok->str, '"'))
	{
		args[i++] = expanded;
	}
	else
	{
		i = add_split(args, i, expanded);
	}
	return (i);
}

static int	process_token(char **args, int i, t_token *tok, t_mini *mini)
{
	if (tok->quote_type == Q_S)
		return (handle_single(args, i, tok));
	return (handle_expanded(args, i, tok, mini));
}
/*
static int	should_count_or_add(t_token *tok, t_mini *mini, char ***args, int *i)
static int	count_expanded_split(char *expanded)
static int	count_token_args(t_token *tok, t_mini *mini)
static int	count_args_advanced(t_token *start, t_token *end, t_mini *mini)
{
	int	count = 0;

	while (start && start != end)
	{
		if (start->type == CMD || start->type == UNKNOWN)
			count += count_token_args(start, mini);
		else if ((start->type == R_IN || start->type == R_OUT || \
				start->type == R_A || start->type == HD) && start->next)
			start = start->next;
		start = start->next;
	}
	return (count);
}

*/

static int	count_args_advanced(t_token *start, t_token *end, t_mini *mini)
{
	int		count;
	char	*expanded;
	char	**split;
	int		i;

	count = 0;
	while (start && start != end)
	{
		if (start->type == CMD || start->type == UNKNOWN)
		{
			split = NULL;
			if (start->quote_type == Q_S || start->quote_type == Q_D)
			{
				count++;
			}
			else
			{
				expanded = expand_if_needed(start, mini);
				if (!expanded)
				{
					start = start->next;
					continue ;
				}
				if (expanded[0] != '\0' || start->quote_type != Q_NONE)
				{
					split = ft_split(expanded, ' ');
					i = 0;
					while (split && split[i])
					{
						count++;
						i++;
					}
				}
				free(expanded);
				if (split)
					free_split(split);
			}
		}
		else if ((start->type == R_IN || start->type == R_OUT || \
			start->type == R_A || start->type == HD) && start->next)
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

	size = count_args_advanced(start, end, mini);
	args = (char **)ft_calloc(size + 1, sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	while (start && start != end)
	{
		if (start->type == CMD || start->type == UNKNOWN)
			i = process_token(args, i, start, mini);
		else if ((start->type == R_IN || start->type == R_OUT || \
			start->type == R_A || start->type == HD) && start->next)
			start = start->next;
		start = start->next;
	}
	args[i] = NULL;
	return (args);
}

static char	**collect_args_for_export(t_token *start, t_token *end, t_mini *mini)
{
	int		count;
	int		i;
	char	*expanded;
	char	**args;
	t_token	*tmp;

	count = 0;
	i = 0;
	tmp = start;
	while (tmp && tmp != end)
	{
		if (tmp->type == CMD || tmp->type == UNKNOWN)
		{
			expanded = expand_if_needed(tmp, mini);
			if (expanded && (expanded[0] != '\0' || tmp->quote_type != Q_NONE))
				count++;
			free(expanded);
		}
		tmp = tmp->next;
	}
	args = (char **)ft_calloc(count + 1, sizeof(char *));
	if (!args)
		return (NULL);
	while (start && start != end)
	{
		if (start->type == CMD || start->type == UNKNOWN)
		{
			expanded = expand_if_needed(start, mini);
			if (expanded && (expanded[0] != '\0' || start->quote_type != Q_NONE))
				args[i++] = expanded;
			else
				free(expanded);
		}
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
			fd = open(cmd->infile, O_RDONLY);
			if (fd < 0)
			{
				perror(cmd->infile);
				cmd->in_error = 1;
			}
			close(fd);
			tmp = tmp->next;
		}
		else if (tmp->type == R_OUT && tmp->next)
		{
			if (cmd->outfile)
				free(cmd->outfile);
			cmd->outfile = expand_arg(tmp->next->str, mini, tmp->next->quote_type);
			if (cmd->in_error != 1)
			{
				fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (fd < 0)
				{
					perror(cmd->outfile);
					cmd->path_error = 1;
				}
				close(fd);
				cmd->fd_out = STDOUT_FILENO;
			}
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
			if (cmd->in_error != 1)
			{
				fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (fd < 0)
				{
					perror(cmd->outfile);
					cmd->path_error = 1;
				}
				cmd->fd_out = STDOUT_FILENO;
			}
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
	if (start && start->str && ft_strcmp(start->str, "export") == 0)
		cmd->cmd_args = collect_args_for_export(start, end, mini);
	else
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