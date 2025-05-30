/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:47:30 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/20 10:57:14 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		return (ft_strjoin_ff(result, ft_strdup("$")));
	if (str[*i + 1] == '?')
		return (handle_exit_code(result, i, mini));
	if (ft_isalpha(str[*i + 1]) || str[*i + 1] == '_')
		return (handle_variable(str, i, result, mini));
	(*i)++;
	return (ft_strjoin_ff(result, ft_strdup("$")));
}

char	*handle_exit_code(char *result, int *i, t_mini *mini)
{
	char	*exit_str;

	exit_str = ft_itoa(mini->last_exit);
	if (!exit_str)
		return (NULL);
	(*i) += 2;
	return (ft_strjoin_ff(result, exit_str));
}

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
	return (ft_strjoin_ff(result, val));
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
