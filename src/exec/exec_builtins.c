/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:32:06 by hho-troc          #+#    #+#             */
/*   Updated: 2025/04/22 10:16:11 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
/* ft_si_builtin = ok
	this function use in exec_ast
	to check if node->cmd->cmd_args[0] is a built-in CMD
@@ -49,6 +31,26 @@ bool	ft_is_builtin(char *arg)
		return (true);
	return (false);
}*/

/* void	ft_exec_builtin(char **args)
{
	if (!ft_strcmp(args[0], "echo"))
		return (ft_echo(args));
	if (!ft_strcmp(args[0], "cd"))
		return (ft_cd(args[1]));
	if (!ft_strcmp(args[0], "env"))
		return (ft_env());
	if (!ft_strcmp(args[0], "pwd"))
		return (ft_pwd());
	if (!ft_strcmp(args[0], "export"))
		return (ft_export(args));
	if (!ft_strcmp(args[0], "unset"))
		return (ft_unset(args));
	ft_exit(args);
} */
//faut penser si on return the (exitno)


/* for ft_run_builtin
	sorry I did it quick vendredi soir for testing miniminiprototype
	we have to redo each buit-in function
@@ -61,49 +63,58 @@ bool	ft_is_builtin(char *arg)
	unset
	env
*/

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
		ft_echo(cmd);
	if (!ft_strcmp(cmd->cmd_args[0], "pwd"))
		return (ft_pwd());
	if (!ft_strcmp(cmd->cmd_args[0], "cd"))
		return (ft_cd(cmd, envp));
// 	if (!ft_strcmp(cmd->cmd_args[0], "env"))
// 		return (ft_env());
// 	if (!ft_strcmp(cmd->cmd_args[0], "export"))
// 		return (ft_export(args));
// 	if (!ft_strcmp(cmd->cmd_args[0], "unset"))
// 		return (ft_unset(args));
// //____________________________________________ CD
// 	else if (!ft_strcmp(name, "cd"))
// 	{
// 		if (!cmd->cmd_args[1])
// 			return chdir(getenv("HOME"));
// 		return chdir(cmd->cmd_args[1]);
// 	}
	else if (!ft_strcmp(name, "exit"))
	{
		printf("exit\n");
		exit(0);
	}
	// TODO: support export, unset, env

	return (1); // not handled
}
