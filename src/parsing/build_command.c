/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 12:23:52 by ndabbous          #+#    #+#             */
/*   Updated: 2025/06/03 10:39:07 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_args_and_path(t_cmd *cmd, t_token *start, \
	t_token *end, t_mini *mini)
{
	if (start && start->str && ft_strcmp(start->str, "export") == 0)
		cmd->cmd_args = collect_args_for_export(start, end, mini);
	else
		cmd->cmd_args = collect_args(start, end, mini);
	if (cmd->cmd_args && cmd->cmd_args[0] && cmd->cmd_args[0][0] != '\0')
	{
		if (ft_strchr(cmd->cmd_args[0], '/'))
			cmd->cmd_path = ft_strdup(cmd->cmd_args[0]);
		else
		{
			cmd->cmd_path = resolve_cmd_path(cmd->cmd_args[0], mini->env);
			if (!cmd->cmd_path)
				cmd->cmd_path = ft_strdup(cmd->cmd_args[0]);
		}
	}
}

int	parse_tokens(t_token *start, t_token *end, t_cmd *cmd, t_mini *mini)
{
	int		i;

	handle_redir(start, end, cmd, mini);
	if (cmd->heredoc_nb > 0)
	{
		i = 0;
		cmd->heredoc_pipe = create_heredoc_pipe(cmd->heredoc_nb);
		cmd->heredocs = get_heredoc(cmd->heredoc_nb, start, end, cmd);
		while (i++ < cmd->heredoc_nb)
		{
			if (mini->stop_hd == 1 || cmd->flag_hd == 1)
			{
				mini->stop_hd = 1;
				mini->last_exit = 130;
				return (130);
			}
			if (fork_heredocs(mini, cmd, cmd->heredocs[i - 1], i - 1))
			{
				if (mini->last_exit == 130)
					return (130);
				return (mini->last_exit);
			}
		}
	}
	return (mini->last_exit);
}

t_cmd	*build_command(t_token *start, t_token *end, t_mini *mini)
{
	t_cmd	*cmd;

	if (start == end)
		return (NULL);
	cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->fd_in = -1;
	cmd->fd_out = -1;
	mini->last_exit = parse_tokens(start, end, cmd, mini);
	set_args_and_path(cmd, start, end, mini);
	return (cmd);
}
