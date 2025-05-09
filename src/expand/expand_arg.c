/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:05:52 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/09 15:21:51 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


/* for "'$USER'" */
char	*expand_if_needed(t_token *token, t_mini *mini)
{
	if (token->quote_type == QUOTE_SINGLE)
		return (ft_strdup(token->str));
	//return (expand_arg(token->str, mini));
	return expand_arg(token->str, mini, token->quote_type);

}


// char	*ft_strjoin_f(char *s1, char *s2)
// {
// 	char	*joined;

// 	joined = ft_strjoin(s1, s2);
// 	free(s1);
// 	return (joined);
// }

// char	*ft_strjoin_f(char *s1, char *s2)
// {
// 	char *joined;

// 	if (!s1 || !s2)
// 	{
// 		free(s1);
// 		free(s2);
// 		return (NULL);
// 	}
// 	joined = ft_strjoin(s1, s2);
// 	free(s1);
// 	return joined;
// }
char	*ft_strjoin_f(char *s1, char *s2)
{
	char *joined;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (s1); // 已有內容，後面是 NULL，不連接

	joined = ft_strjoin(s1, s2);
	free(s1);
	return joined;
}


char	*get_env_value(const char *key, char **env)
{
	int		i;
	size_t	key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, key_len) && env[i][key_len] == '=')
			return (ft_strdup(env[i] + key_len + 1));
		i++;
	}
	return (ft_strdup("")); // if not found, return empty string
}

static char	*expand_var(const char *str, int *i, t_mini *mini)
{
	int	start;
	char	*var;
	char	*val;

	start = ++(*i); // skip $
	if (str[start] == '"' || str[start] == '\'')
	{
		// Bash 中：$"" → 空變數（返回空字串）
		(*i)++; // 前進 1 讓 tokenizer 不卡住
		return (ft_strdup(""));
	}

	if (str[start] == '?')// $? is for exit code
	//WEXITSTATUS(status) ????
	{
		(*i)++;
		return (ft_itoa(mini->last_exit));
	}
	if (!str[start] || !(ft_isalpha(str[start]) || str[start] == '_'))
	{
		// echo $
		//(*i)++; // for echo "$"""
		return (ft_strdup("$"));
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	var = ft_strndup(str + start, *i - start);
	val = get_env_value(var, mini->env);
	free(var);
	return (val);
}

char *expand_arg(const char *str, t_mini *mini, t_quote_type quote_type)
{
	char	*result;
	int		i = 0;
	int		start;

	//result = calloc(1, sizeof(char));
	result = ft_strdup("");
	while (str[i])
	{
		//if (str[i] == '\'') // 單引號：整段當作原始文字
		if (str[i] == '\'' && quote_type == QUOTE_NONE) // 只有在外部沒引號時，才當 quote 處理
		{
			start = ++i;
			while (str[i] && str[i] != '\'')
				i++;
			result = ft_strjoin_f(result, ft_strndup(str + start, i - start));
			if (str[i] == '\'')
				i++;
		}
		else if (str[i] == '"') // 雙引號：展開 $，保留空格
		{
			start = ++i;
			while (str[i] && str[i] != '"')
			{
				if (str[i] == '$')
				{
					//處理 $"" 或 $'' 作為空字串
					if (str[i + 1] == '"' && str[i + 2] == '"')
						i += 3;
					else if (str[i + 1] == '\'' && str[i + 2] == '\'')
						i += 3;
					else
						result = ft_strjoin_f(result, expand_var(str, &i, mini));
				}
				else
					result = ft_strjoin_f(result, ft_strndup(str + i++, 1));
			}
			if (str[i] == '"')
				i++;
		}
		else if (str[i] == '$') // 無引號時：展開 $
		{
			// 🟡 這裡處理 $"" 或 $'' 為空字串
			if (str[i + 1] == '"' && str[i + 2] == '"')
				i += 3;
			else if (str[i + 1] == '\'' && str[i + 2] == '\'')
				i += 3;
			else
				result = ft_strjoin_f(result, expand_var(str, &i, mini));
		}
		else // 普通字元
			result = ft_strjoin_f(result, ft_strndup(str + i++, 1));
	}
	result = ft_strjoin_f(result, ft_strdup(""));
	return (result);
}
