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

/* ft_si_builtin = ok
	this function use in exec_ast
	to check if node->cmd->cmd_args[0] is a built-in CMD
	if yes, we run the ft_run_builtin
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
/* for ft_run_builtin
	sorry I did it quick vendredi soir for testing miniminiprototype
	we have to redo each buit-in function
	echo -n -> need to do -n option
	pwd -> have to check
	Need to do;
	cd
	exit
	export
	unset
	env
*/
int	ft_run_builtin(t_cmd *cmd, char ***envp)
{
	(void)envp;
	if (!cmd || !cmd->cmd_args || !cmd->cmd_args[0])
		return (1);

	char *name = cmd->cmd_args[0];

	//replace lines below as ...
	//if (!ft_strcmp(name, "echo"))
	//{
	//	ft_echo(name);
	//}
	if (!ft_strcmp(name, "echo"))
	{
		for (int i = 1; cmd->cmd_args[i]; i++)
		{
			printf("%s", cmd->cmd_args[i]);
			if (cmd->cmd_args[i + 1])
				printf(" ");
		}
		printf("\n");
		return (0);
	}
	else if (!ft_strcmp(name, "pwd"))
	{
		char cwd[1024];
		if (getcwd(cwd, sizeof(cwd)))
			printf("%s\n", cwd);
		return (0);
	}
	else if (!ft_strcmp(name, "cd"))
	{
		if (!cmd->cmd_args[1])
			return chdir(getenv("HOME"));
		return chdir(cmd->cmd_args[1]);
	}
	else if (!ft_strcmp(name, "exit"))
	{
		printf("exit\n");
		exit(0);
	}
	// TODO: support export, unset, env

	return (1); // not handled
}
