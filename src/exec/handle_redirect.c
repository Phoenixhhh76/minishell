/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndabbous <ndabbous@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-29 18:45:53 by ndabbous          #+#    #+#             */
/*   Updated: 2025-05-29 18:45:53 by ndabbous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_input_redirection(t_cmd *cmd)
{
	int	fd;

	if (cmd->infile && cmd->last_redirin == 0)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd < 0)
			perror(cmd->infile);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (cmd->heredoc_nb > 0 && !cmd->flag_hd)
	{
		dup2(cmd->heredoc_pipe[cmd->heredoc_nb - 1][0], STDIN_FILENO);
		close_all_heredoc_pipes(cmd);
	}
}

void	handle_output_redirection(t_cmd *cmd)
{
	int	fd;

	if (cmd->outfile)
	{
		if (cmd->append && ft_strcmp(cmd->outfile, cmd->append) == 0)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
		{
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
				perror("open outfile");
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	handle_redirects(t_cmd *cmd)
{
	handle_input_redirection(cmd);
	handle_output_redirection(cmd);
}
