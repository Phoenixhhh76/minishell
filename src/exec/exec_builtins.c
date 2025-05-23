/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:32:06 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/22 21:08:58 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
//faut penser si on return the (exitno)

bool	ft_is_builtin(char *arg)
{
	if (!arg)
		return (false);
	if (!ft_strcmp(arg, "echo")
		|| !ft_strcmp(arg, "cd")
		|| !ft_strcmp(arg, "exit")
		|| !ft_strcmp(arg, "pwd")
		|| !ft_strcmp(arg, "export")
		|| !ft_strcmp(arg, "unset")
		|| !ft_strcmp(arg, "env"))
		return (true);
	return (false);
}

bool	ft_builtin(t_ast *ast)
{
	if (!ast || ast->ast_token.type != CMD || !ast->cmd)
		return (false);
	if (!ft_is_builtin(ast->cmd->cmd_args[0]))
		return (false);
	return (true);
}

int	ft_run_builtin( t_mini *mini, t_cmd *cmd)
{
	//char	*name;

	//name = cmd->cmd_args[0];
	if (!cmd || !cmd->cmd_args || !cmd->cmd_args[0])
		return (1);
	if (!ft_strcmp(cmd->cmd_args[0], "echo"))
		return (ft_echo(cmd));
	if (!ft_strcmp(cmd->cmd_args[0], "pwd"))
		return (ft_pwd());
	if (!ft_strcmp(cmd->cmd_args[0], "cd"))
		return (ft_cd(cmd));
	if (!ft_strcmp(cmd->cmd_args[0], "export"))
		return (ft_export(cmd, &mini->env, mini));
	if (!ft_strcmp(cmd->cmd_args[0], "env"))
		return (ft_env(cmd, &mini->env));
	if (!ft_strcmp(cmd->cmd_args[0], "unset"))
		return (ft_unset(cmd, mini));
	else if (!ft_strcmp(cmd->cmd_args[0], "exit"))
		return (ft_exit(mini, cmd));
	return(1); // not handled
}
