
// t_cmd	*build_command(t_token *start, t_token *end, t_mini *mini)
// {
// 	t_cmd	*cmd;
// 	t_token	*tmp;
// 	int		fd;
// 	int		heredoc_nb;

// 	heredoc_nb = 0;
// 	if (start == end)
// 		return (NULL);
// 	tmp = start;
// 	cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
// 	if (!cmd)
// 		return (NULL);
// 	cmd->fd_in = -1; //to be determined;
// 	cmd->fd_out = -1; //to be determined;
// 	while (tmp && tmp != end)
// 	{
// 		if (tmp->type == R_IN && tmp->next)
// 		{
// 			if (cmd->infile)
// 				free(cmd->infile);
// 			cmd->last_redirin = 0;
// 			cmd->infile = expand_arg(tmp->next->str, mini, tmp->next->quote_type);
// 			fd = open(cmd->infile, O_RDONLY);
// 			if (fd < 0)
// 			{
// 				perror(cmd->infile);
// 				cmd->flag_error = 1;
// 			}
// 			else
// 				close(fd);
// 			tmp = tmp->next;
// 		}
// 		else if (tmp->type == R_OUT && tmp->next)
// 		{
// 			if (cmd->outfile)
// 				free(cmd->outfile);
// 			cmd->outfile = expand_arg(tmp->next->str, mini, tmp->next->quote_type);
// 			if (cmd->flag_error != 1)
// 			{
// 				fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 				if (fd < 0)
// 				{
// 					perror(cmd->outfile);
// 					cmd->path_error = 1;
// 				}
// 				else
// 					close(fd);
// 				cmd->fd_out = STDOUT_FILENO;
// 			}
// 			tmp = tmp->next;
// 		}
// 		else if (tmp->type == R_A && tmp->next)
// 		{
// 			if (cmd->outfile)
// 				free(cmd->outfile);
// 			cmd->outfile = expand_arg(tmp->next->str, mini, tmp->next->quote_type);
// 			if (cmd->append)
// 				free(cmd->append);
// 			cmd->append = expand_arg(tmp->next->str, mini, tmp->next->quote_type);
// 			if (cmd->flag_error != 1)
// 			{
// 				fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 				if (fd < 0)
// 				{
// 					perror(cmd->outfile);
// 					cmd->path_error = 1;
// 				}
// 				else
// 					close(fd);
// 				cmd->fd_out = STDOUT_FILENO;
// 			}
// 			tmp = tmp->next;
// 		}
// 		else if (tmp->type == HD && tmp->next)
// 		{
// 			if (cmd->infile)
// 				free(cmd->infile);
// 			cmd->last_redirin = 1;
// 			cmd->infile = expand_arg(tmp->next->str, mini, tmp->next->quote_type);
// 			heredoc_nb++;
// 			tmp = tmp->next;
// 		}
// 		tmp = tmp->next;
// 	}
// 	cmd->heredoc_nb = heredoc_nb;
// 	if (heredoc_nb > 0)
// 	{
// 		cmd->heredoc_pipe = create_heredoc_pipe(heredoc_nb);
// 		cmd->heredocs = get_heredoc(heredoc_nb, start, end, cmd);
// 	}
// 	if (start && start->str && ft_strcmp(start->str, "export") == 0)
// 		cmd->cmd_args = collect_args_for_export(start, end, mini);
// 	else
// 		cmd->cmd_args = collect_args(start, end, mini);
// 	if (cmd->cmd_args && cmd->cmd_args[0])
// 	{
// 		if (cmd->cmd_args[0][0] == '/' || cmd->cmd_args[0][0] == '.')
// 			cmd->cmd_path = ft_strdup(cmd->cmd_args[0]);
// 		else //add and change
// 		{
// 			cmd->cmd_path = resolve_cmd_path(cmd->cmd_args[0], mini->env);
// 			if (!cmd->cmd_path)
// 				cmd->cmd_path = ft_strdup(cmd->cmd_args[0]);// <<<<<<for empty"""
// 		}
// 	}
// 	return (cmd);
// }