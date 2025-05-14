/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:47:30 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/13 10:54:13 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_env_value_line(const char *key, char **env)
{
	int		i;
	size_t	key_len;
	char	*equal_sign;
	char	*entry;
	size_t	name_len;

	if (!key || !env)
		return (ft_strdup(""));
	key_len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		entry = env[i];
		equal_sign = ft_strchr(entry, '=');
		if (!equal_sign)
		{
			i++;
			continue ;
		}
		name_len = equal_sign - entry;
		if (name_len == key_len && ft_strncmp(entry, key, key_len) == 0)
			return (ft_strdup(equal_sign + 1));
		i++;
	}
	return (ft_strdup(""));
}

char	*handle_dollar(const char *str, int *i, char *result, t_mini *mini)
{
	if (!str[*i + 1])
		return (ft_strjoin_f(result, ft_strdup("$")));
	if (str[*i + 1] == '?')
		return (handle_exit_code(result, i, mini));
	// if (str[*i + 1] == '$')
	// 	return (handle_pid(result, i));
	if (ft_isalpha(str[*i + 1]) || str[*i + 1] == '_')
		return (handle_variable(str, i, result, mini));
	(*i)++;
	return (ft_strjoin_f(result, ft_strdup("$")));
}

char	*handle_exit_code(char *result, int *i, t_mini *mini)
{
	char	*exit_str;

	exit_str = ft_itoa(mini->last_exit);
	if (!exit_str)
		return (NULL);
	(*i) += 2;
	return (ft_strjoin_f(result, exit_str));
}

// char	*handle_pid(char *result, int *i)
// {
// 	char	*pid_str;

// 	pid_str = ft_itoa(getpid());
// 	if (!pid_str)
// 		return (NULL);
// 	(*i) += 2;
// 	return (ft_strjoin_f(result, pid_str));
// }

char	*handle_variable(const char *str, int *i, char *result, t_mini *mini)
{
	int		start;
	char	*var;
	char	*val;

	start = ++(*i);
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	var = ft_strndup(str + start, *i - start);
	if (!var)
		return (NULL);
	val = get_env_value_line(var, mini->env);
	free(var);
	if (!val)
		val = ft_strdup("");
	return (ft_strjoin_f(result, val));
}

char	*append_char(char *result, char c)
{
	char	buf[2];

	buf[0] = c;
	buf[1] = '\0';
	return (ft_strjoin_f(result, ft_strdup(buf)));
}

char	*expand_heredoc_line(const char *str, t_mini *mini)
{
	char	*result;
	int		i;

	if (!str || !mini || !mini->env)
		return (ft_strdup(""));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			result = handle_dollar(str, &i, result, mini);
		else
			result = append_char(result, str[i++]);
		if (!result)
			return (NULL);
	}
	return (result);
}



/*
char	*expand_heredoc_line(const char *str, t_mini *mini)
{
	char	*result;
	int		i;
	int		start;
	char	*var;
	char	*val;
	char *exit_str;

	printf("DEBUG: mini = %p, mini->env = %p\n", mini, mini ? mini->env : NULL);
	i = 0;
	if (!str || !mini || !mini->env)
		return (ft_strdup(""));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '\0') // 單獨一個 $
			{
				result = ft_strjoin_f(result, ft_strdup("$"));
				i++;
			}
			else if (str[i + 1] == '?')
			{
				char *exit_str = ft_itoa(mini->last_exit);
				result = ft_strjoin_f(result, exit_str);
				i += 2;
			}
			else if (str[i + 1] == '$') // 例如：$$ → getpid()
			{
				char *pid_str = ft_itoa(getpid());
				result = ft_strjoin_f(result, pid_str);
				i += 2;
			}
			else if (ft_isalpha(str[i + 1]) || str[i + 1] == '_')
			{
				start = ++i;
				while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
					i++;
				char *var = ft_strndup(str + start, i - start);
				char *val = get_env_value_line(var, mini->env);
				result = ft_strjoin_f(result, val);
				free(var);
			}
			else // 無效變數名，保留 $
			{
				result = ft_strjoin_f(result, ft_strdup("$"));
				i++;
			}
		}
		else
		{
			result = ft_strjoin_f(result, ft_strndup(str + i, 1));
			i++;
		}
	}
	return (result);
} */
