/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndabbous <ndabbous@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-16 12:23:52 by ndabbous          #+#    #+#             */
/*   Updated: 2025-05-16 12:23:52 by ndabbous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	set_args_and_path(t_cmd *cmd, t_token *start, \
	t_token *end, t_mini *mini)
{
	if (start && start->str && ft_strcmp(start->str, "export") == 0)
		cmd->cmd_args = collect_args_for_export(start, end, mini);
	else
		cmd->cmd_args = collect_args(start, end, mini);
	if (cmd->cmd_args && cmd->cmd_args[0])
	{
		if (cmd->cmd_args[0][0] == '/' || cmd->cmd_args[0][0] == '.')
			cmd->cmd_path = ft_strdup(cmd->cmd_args[0]);
		else
		{
			cmd->cmd_path = resolve_cmd_path(cmd->cmd_args[0], mini->env);
			if (!cmd->cmd_path)
				cmd->cmd_path = ft_strdup(cmd->cmd_args[0]);
		}
	}
}

void	parse_tokens(t_token *start, t_token *end, t_cmd *cmd, t_mini *mini)
{
	t_token	*tmp;
	int		i;

	tmp = start;
	while (tmp && tmp != end)
	{
		if (tmp->type == R_IN && tmp->next)
			handle_redir_in(tmp, cmd, mini);
		else if (tmp->type == R_OUT && tmp->next)
			handle_redir_out(tmp, cmd, mini);
		else if (tmp->type == R_A && tmp->next)
			handle_redir_append(tmp, cmd, mini);
		else if (tmp->type == HD && tmp->next)
			handle_hd(tmp, cmd, mini);
		if (tmp->type == R_IN || tmp->type == R_OUT || \
			tmp->type == R_A || tmp->type == HD)
			tmp = tmp->next;
		tmp = tmp->next;
	}
	if (cmd->heredoc_nb > 0)
	{
		i = 0;
		cmd->heredoc_pipe = create_heredoc_pipe(cmd->heredoc_nb);
		cmd->heredocs = get_heredoc(cmd->heredoc_nb, start, end, cmd);
		while (i < cmd->heredoc_nb)
		{
			if (cmd->flag_hd == 1 || g_signal_pid == 1)
				return ;
			if (fork_heredoc(cmd, cmd->heredocs[i], i))
				return ;
			i++;
		}
		//cmd->heredoc_pipe = create_heredoc_pipe(cmd->heredoc_nb);
		//cmd->heredocs = get_heredoc(cmd->heredoc_nb, start, end, cmd);
	}
}

t_cmd	*build_command(t_token *start, t_token *end, t_mini *mini)
{
	t_cmd	*cmd;

	if (start == end)
		return (NULL);
	cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);//check error
	cmd->fd_in = -1;
	cmd->fd_out = -1;
	parse_tokens(start, end, cmd, mini);
	set_args_and_path(cmd, start, end, mini);
	return (cmd);
}
