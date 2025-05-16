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
	printf("  lat_redirin: %d\n", cmd->last_redirin);
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
	if (cmd->append)
		printf("  append: %s\n", cmd->append);
	printf("  heredoc_nb: %i\n", cmd->heredoc_nb);
	if (cmd->heredoc_nb > 0)
	{
		i = 0;
		printf("tab_heredocs= ");
		while (cmd->heredocs[i])
		{
			printf(" [%s]", cmd->heredocs[i]);
			i++;
		}
		printf("\n");
	}
}

void	print_token_type(t_node type)
{
	if (type == CMD)
		printf("Node: type = CMD\n");
	else if (type == PIPE)
		printf("Node: type = PIPE\n");
	else if (type == R_IN)
		printf("Node: type = R_IN\n");
	else if (type == R_OUT)
		printf("Node: type = R_OUT\n");
	else if (type == R_A)
		printf("Node: type = R_A\n");
	else if (type == HD)
		printf("Node: type = HD\n");
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
// void print_ast(t_ast *ast, int level)
// {
// 	if (!ast)
// 		return;

// 	for (int i = 0; i < level; i++) printf("  ");
// 	printf("Node: %-10s (type: %d)\n",
// 		ast->ast_token.str ? ast->ast_token.str : "(null)",
// 		ast->ast_token.type);

// 	if (ast->cmd)
// 	{
// 		for (int i = 0; i < level + 1; i++) printf("  ");
// 		printf("Command: ");
// 		for (int i = 0; ast->cmd->cmd_args && ast->cmd->cmd_args[i]; i++)
// 			printf("%s ", ast->cmd->cmd_args[i]);
// 		printf("\n");
// 	}

// 	if (ast->left)
// 	{
// 		for (int i = 0; i < level; i++) printf("  ");
// 		printf("Left:\n");
// 		print_ast(ast->left, level + 1);
// 	}
// 	if (ast->right)
// 	{
// 		for (int i = 0; i < level; i++) printf("  ");
// 		printf("Right:\n");
// 		print_ast(ast->right, level + 1);
// 	}
// }
