/* ************************************************************************** */
/*                                                                            */
/*                        tokenizer.c (refactored)                             */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * Remove all global variable dependencies.
 * Replaces g_minishell.line with argument.
 * Replaces g_minishell.exit_s with return value.
 */

// New return type to hold both tokens and exit status
typedef struct s_tokenizer_result {
	t_token *tokens;
	int     exit_code; // 0 if success, 258 if quote error, etc.
} t_tokenizer_result;

// Updated function signature
static int ft_append_identifier_safe(char **line_ptr, t_token **token_list, int *exit_code)
{
	char *tmp_line = *line_ptr;
	size_t i = 0;
	while (tmp_line[i] && !ft_is_separator(tmp_line + i))
	{
		if (ft_is_quote(tmp_line[i]))
		{
			if (!ft_skip_quotes(tmp_line, &i))
			{
				ft_putstr_fd("minishell: unexpected EOF while looking for matching `", 2);
				ft_putchar_fd(tmp_line[i], 2);
				ft_putstr_fd("'\n", 2);
				*exit_code = 258;
				return (0);
			}
		}
		else
			i++;
	}
	char *value = ft_substr(tmp_line, 0, i);
	if (!value)
		return (0);
	t_token *token = ft_new_token(value, T_IDENTIFIER);
	if (!token)
		return (free(value), 0);
	*line_ptr += i;
	return (ft_token_list_add_back(token_list, token), 1);
}

static t_tokenizer_result ft_tokenization_handler_safe(char *line)
{
	t_tokenizer_result result = {0};
	t_token *token_list = NULL;
	int error = 0;
	int exit_code = 0;
	while (*line && !error)
	{
		if (ft_isspace(*line))
			ft_skip_spaces(&line);
		else if (!ft_strncmp(line, "<", 1) || !ft_strncmp(line, ">", 1)
			|| !ft_strncmp(line, "|", 1) || !ft_strncmp(line, "&&", 2)
			|| !ft_strncmp(line, "(", 1) || !ft_strncmp(line, ")", 1))
			error = (!ft_handle_separator(&line, &token_list) && 1);
		else
			error = (!ft_append_identifier_safe(&line, &token_list, &exit_code) && 1);
	}
	if (error)
		ft_clear_token_list(&token_list);
	result.tokens = token_list;
	result.exit_code = exit_code;
	return result;
}

t_tokenizer_result ft_tokenize_line(char *line)
{
	return ft_tokenization_handler_safe(line);
}
