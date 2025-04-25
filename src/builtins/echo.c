
#include "../../includes/minishell.h"
/*
void	ft_echo(t_cmd *cmd)
{
	int	i;
	int	option;

	i = 1;
	option = 0;
	print_cmd(cmd);
	if (!ft_strcmp(cmd->cmd_args[i], "-n"))
	{
		option = 1;
		i++;
	}
	while (cmd->cmd_args[i])
	{
		printf("%s", cmd->cmd_args[i]);
		if (cmd->cmd_args[i + 1])
			printf(" ");
		i++;
	}
	if (option == 0)
		printf("\n");
} */

//test for ''$USER''//
/*
void	ft_echo(t_cmd *cmd)
{
	int	i;
	int	option;
	int	first_printed;

	i = 1;
	option = 0;
	first_printed = 0;
	//print_cmd(cmd);
	if (!ft_strcmp(cmd->cmd_args[i], "-n"))
	{
		option = 1;
		i++;
	}
	while (cmd->cmd_args[i])
	{
		if (first_printed)
			printf(" ");
		printf("%s", cmd->cmd_args[i]);
		first_printed = 1;
		i++;
	}
	if (option == 0)
		printf("\n");
} */
//skip the first empty cmd as space
void	ft_echo(t_cmd *cmd)
{
	int	i = 1;
	int	option = 0;
	int	first = 1;

	if (cmd->cmd_args[i] && !ft_strcmp(cmd->cmd_args[i], "-n"))
	{
		option = 1;
		i++;
	}
	while (cmd->cmd_args[i])
	{
		if (cmd->cmd_args[i][0] != '\0')
		{
			if (!first)
				printf(" ");
			printf("%s", cmd->cmd_args[i]);
			first = 0;
		}
		i++;
	}
	if (!option)
		printf("\n");
}
