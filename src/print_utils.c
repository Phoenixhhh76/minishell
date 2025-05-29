#include "../includes/minishell.h"

// void	print_token_list(t_token *token)
// {
// 	while (token)
// 	{
// 		printf("Token: %-10s Type: %d\n", token->str, token->type);
// 		token = token->next;
// 	}
// }

void	print_tab(char **tab)
{
	int	i;

	i = 0;
	printf("*** PRINT TAB ***\n");
	while (tab[i])
	{
		printf("tab[%i]= %s\n", i, tab[i]);
		i++;
	}
	printf("*** END PRINT TAB ***\n\n");
}

void debug_tokens(t_token *tok)
{
	while (tok)
	{
		printf("Token [%s]  quote_type=%d  glued=%d\n",
			tok->str, tok->quote_type, tok->glued);
		tok = tok->next;
	}
}

// void	print_cmd(t_cmd *cmd)
// {
// 	int	i;

// 	i = 0;
// 	printf("### PRINT T_CMD ###\n");
// 	while (cmd->cmd_args[i])
// 	{
// 		printf("args[%i]= %s\n", i, cmd->cmd_args[i]);
// 		i++;
// 	}
// 	i = 0;
// 	if (cmd->cmd_opts)
// 	{
// 		while (cmd->cmd_opts[i])
// 		{
// 			printf("opt[%i]= %s\n", i, cmd->cmd_opts[i]);
// 			i++;
// 		}
// 	}
// 	if (cmd->cmd_path)
// 		printf("path= %s\n", cmd->cmd_path);
// 	printf("fd_in= %i\n", cmd->fd_in);
// 	printf("fd_out= %i\n", cmd->fd_out);
// 	if (cmd->infile)
// 		printf("infile = %s\n", cmd->infile);
// 	if (cmd->outfile)
// 		printf("infile = %s\n", cmd->outfile);
// 	if (cmd->pipe)
// 	{
// 		while (cmd->pipe[i])
// 		{
// 			printf("pipe[%i]= %i\n", i, cmd->pipe[i]);
// 			i++;
// 		}
// 	}
// 	if (cmd->pids)
// 	{
// 		while (cmd->pids[i])
// 		{
// 			printf("pids[%i]= %i\n", i, cmd->pids[i]);
// 			i++;
// 		}
// 	}
// 	printf("### END PRINT ###\n\n");
// }

// void	print_ast(t_ast *ast)
// {
// 	int	i;

// 	i = 0;
// 	printf("### PRINT T_AST ###\n");
// 	printf("Token_str: %-10s Type: %d\n", ast->ast_token->str, ast->ast_token->type);
// 	printf("fd[0]= %i, fd[1]")
// 	printf("### END PRINT AST###\n\n");
// }

void	print_mini(t_mini *mini)
{
	t_token	*tmp;
	//t_ast	*tmp_ast;

	tmp = mini->token;
	//tmp_ast = mini->ast;
	printf("Mini %p\n", tmp);
	while (tmp)
	{
		printf("Token: %-10s Type: %d\n", tmp->str, tmp->type);
		//printf("Ast_token: %d\n", tmp_ast->fd[0]);
		tmp = tmp->next;
		//tmp_ast = tmp_ast->next;
	}
}

void	print_token_list(t_token *token)
{
	while (token)
	{
		if (token->str[0])
			printf("Token [%s]  quote_type %d\n", token->str, token->quote_type);
		else
			printf("Token [(empty)]  quote_type %d\n", token->quote_type);
		token = token->next;
	}
}

const char *token_type_to_str(t_node type)
{
	switch (type)
	{
		case CMD:     return "CMD";
		case PIPE:    return "PIPE";
		case R_IN:    return "R_IN";
		case R_OUT:   return "R_OUT";
		case R_A:     return "R_APPEND";
		case HD:      return "HEREDOC";
		case UNKNOWN: return "UNKNOWN";
		default:      return "INVALID";
	}
}

void debug_tokens_type(t_token *tok)
{
	while (tok)
	{
		printf("Token [%s]  type=%s  quote_type=%d  glued=%d\n",
			tok->str,
			token_type_to_str(tok->type),
			tok->quote_type,
			tok->glued);
		tok = tok->next;
	}
}

