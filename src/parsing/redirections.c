/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:20:41 by ndabbous          #+#    #+#             */
/*   Updated: 2025/05/21 12:58:11 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_redir_in(t_token *tmp, t_cmd *cmd, t_mini *mini)
{
	int	fd;

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
	else
		close(fd);
}

void	handle_redir_out(t_token *tmp, t_cmd *cmd, t_mini *mini)
{
	int	fd;

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
		else
			close(fd);
		cmd->fd_out = STDOUT_FILENO;
	}
}

void	handle_redir_append(t_token *tmp, t_cmd *cmd, t_mini *mini)
{
	int	fd;

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
		else
			close(fd);
		cmd->fd_out = STDOUT_FILENO;
	}
}

void	handle_hd(t_token *tmp, t_cmd *cmd, t_mini *mini)
{
	(void)tmp;
	(void)mini;

	if (cmd->infile)
		free(cmd->infile);
	cmd->last_redirin = 1;
	cmd->heredoc_nb += 1;
}
