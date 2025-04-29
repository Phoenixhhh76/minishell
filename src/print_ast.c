#include "../includes/minishell.h"

void	print_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
	{
		printf("  (null cmd)\n");
		return;
	}

	if (cmd->cmd_path)
		printf("  cmd_path: %s\n", cmd->cmd_path);
	else
		printf("  cmd_path: (null)\n");

	if (cmd->infile)
		printf("  infile: %s\n", cmd->infile);
	else
		printf("  infile: (null)\n");

	if (cmd->outfile)
		printf("  outfile: %s\n", cmd->outfile);
	else
		printf("  outfile: (null)\n");

	printf("  fd_in: %d\n", cmd->fd_in);
	printf("  fd_out: %d\n", cmd->fd_out);

	if (cmd->cmd_args)
	{
		printf("  args: ");
		i = 0;
		while (cmd->cmd_args[i])
		{
			printf("[%s] ", cmd->cmd_args[i]);
			i++;
		}
		printf("\n");
	}
	else
	{
		printf("  args: (null)\n");
	}
}

void	print_token_type(t_node_type type)
{
	if (type == CMD)
		printf("Node: type = CMD\n");
	else if (type == PIPE)
		printf("Node: type = PIPE\n");
	else if (type == REDIR_IN)
		printf("Node: type = REDIR_IN\n");
	else if (type == REDIR_OUT)
		printf("Node: type = REDIR_OUT\n");
	else if (type == REDIR_APPEND)
		printf("Node: type = REDIR_APPEND\n");
	else if (type == HEREDOC)
		printf("Node: type = HEREDOC\n");
	else if (type == UNKNOWN)
		printf("Node: type = UNKNOWN\n");
	else
		printf("Node: type = INVALID\n");
}

void	print_indent(int depth)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		printf("  ");
		i++;
	}
}

void	print_ast(t_ast *node, int depth)
{
	if (!node)
		return;

	print_indent(depth);
	print_token_type(node->ast_token.type);

	if (node->cmd)
	{
		print_indent(depth);
		printf("Command:\n");
		print_cmd(node->cmd);
	}

	if (node->left)
	{
		print_indent(depth);
		printf("Left:\n");
		print_ast(node->left, depth + 1);
	}

	if (node->right)
	{
		print_indent(depth);
		printf("Right:\n");
		print_ast(node->right, depth + 1);
	}

	if (node->next)
	{
		print_indent(depth);
		printf("Next:\n");
		print_ast(node->next, depth);
	}
}
