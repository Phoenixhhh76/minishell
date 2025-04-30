/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:32:06 by hho-troc          #+#    #+#             */
/*   Updated: 2025/04/30 18:30:19 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
//faut penser si on return the (exitno)
/* modifier for main)sepretate */
bool	ft_builtin(t_ast *ast, char ***env)
{
	(void)env;
	if (!ast || ast->ast_token.type != CMD || !ast->cmd)
		return (false);
	if (!ft_is_builtin(ast->cmd->cmd_args[0]))
		return (false);
	//ft_run_builtin(ast->cmd, env);
	return (true);
}
/* original version for main-Origing */
// bool	ft_builtin(t_ast *ast, char ***env)
// {
// 	if (!ast || ast->ast_token.type != CMD || !ast->cmd)
// 		return (false);
// 	if (!ft_is_builtin(ast->cmd->cmd_args[0]))
// 		return (false);
// 	ft_run_builtin(ast->cmd, env);
// 	return (true);
// }


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

int	ft_run_builtin(t_cmd *cmd, char ***envp)
{
	char	*name;

	(void)envp;
	name = cmd->cmd_args[0];
	if (!cmd || !cmd->cmd_args || !cmd->cmd_args[0])
		return (1);
	if (!ft_strcmp(cmd->cmd_args[0], "echo"))
		return (ft_echo(cmd));
	if (!ft_strcmp(cmd->cmd_args[0], "pwd"))
		return (ft_pwd());
	if (!ft_strcmp(cmd->cmd_args[0], "cd"))
		return (ft_cd(cmd));
	if (!ft_strcmp(cmd->cmd_args[0], "export"))
		return (ft_export(cmd, envp));
	if (!ft_strcmp(cmd->cmd_args[0], "env"))
		return (ft_env(cmd, envp));
	if (!ft_strcmp(cmd->cmd_args[0], "unset"))
		return (ft_unset(cmd, envp));
	else if (!ft_strcmp(name, "exit"))
	{
		printf("exit\n");
		exit(0);
	}
	return (1); // not handled
}
