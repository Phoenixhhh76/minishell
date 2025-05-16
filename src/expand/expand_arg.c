/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:05:52 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/16 11:16:31 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* for "'$USER'" */
char	*expand_if_needed(t_token *token, t_mini *mini)
{
	if (token->quote_type == Q_S)
		return (ft_strdup(token->str));
	return (expand_arg(token->str, mini, token->quote_type));

}

char	*ft_strjoin_f(char *s1, char *s2)
{
	char	*joined;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
	{
		free(s1);
		return (NULL);
	}
	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
}
/*
find the value of the environment variable key in env
if not found, return an empty string
*/
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
	return (ft_strdup(""));
}

/*
this function to find the value of the environment variable key in env.
if in the expand_arg function, we find a $, we move the index i to the next char
if after $ is not a valid variable name, we return "$"
if we can't find the variable name, we return an empty string.
*/
static char	*expand_var(const char *str, int *i, t_mini *mini)
{
	int		start;
	char	*var;
	char	*val;

	if (!str[*i])
		return (ft_strdup(""));
	start = ++(*i); // skip $
	if (str[start] == '"' || str[start] == '\'')
	{
		(*i)++;
		return (ft_strdup(""));
	}
	if (str[start] == '?')
	{
		(*i)++;
		return (ft_itoa(mini->last_exit));
	}
	if (!str[start] || !(ft_isalpha(str[start]) || str[start] == '_'))
		return (ft_strdup("$"));
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	var = ft_strndup(str + start, *i - start);
	val = get_env_value(var, mini->env);
	free(var);
	return (val);
}

char	*expand_arg(const char *str, t_mini *mini, t_quote quote_type)
{
	char	*result;
	int		i;
	int		start;
	int		j;
	char	*after;

	i = 0;
	result = ft_strdup("");
	// for case special: start from "$"""
	if (str[0] == '"' && str[1] == '$' && str[2] == '"' && str[3] == '"')
	{
		j = 4;
		while (str[j] == '"')
			j++;
		after = ft_strdup(&str[j]);
		return (ft_strjoin(ft_strdup("$"), after));
	}
	while (str[i])
	{
		if (str[i] == '\'' && quote_type == Q_NONE)
		{
			start = ++i;
			while (str[i] && str[i] != '\'')
				i++;
			result = ft_strjoin_f(result, ft_strndup(str + start, i - start));
			if (str[i] == '\'')
				i++;
		}
		else if (str[i] == '"')
		{
			start = ++i;
			while (str[i] && str[i] != '"')
			{
				if (str[i] == '$')
					result = ft_strjoin_f(result, expand_var(str, &i, mini));
				else
					result = ft_strjoin_f(result, ft_strndup(str + i++, 1));
			}
			if (str[i] == '"')
				i++;
		}
		else if (str[i] == '$')
			result = ft_strjoin_f(result, expand_var(str, &i, mini));
		else
			result = ft_strjoin_f(result, ft_strndup(str + i++, 1));
	}
	return (result);
}
