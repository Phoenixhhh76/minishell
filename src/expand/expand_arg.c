/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:05:52 by hho-troc          #+#    #+#             */
/*   Updated: 2025/06/01 11:22:52 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_if_needed(t_token *token, t_mini *mini)
{
	if (token->quote_type == Q_S)
		return (ft_strdup(token->str));
	if (token->quote_type == Q_D && token->is_dollar_quote)
		return (ft_strdup(token->str));
	if (token->quote_type == Q_D && ft_strcmp(token->str, "$") == 0)
		return (ft_strdup("$"));
	return (expand_arg(token->str, mini, \
				token->quote_type, token->is_dollar_quote));
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
	if (!env)
		return (ft_strdup(""));
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
char	*expand_var(const char *str, int *i, t_mini *mini)
{
	int		start;
	char	*var;
	char	*val;

	if (!str[*i])
		return (ft_strdup(""));
	start = ++(*i);
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

/* for case special: start from "$""" */
char	*handle_special_case(const char *str)
{
	int		j;
	char	*after;

	j = 4;
	while (str[j] == '"')
		j++;
	after = ft_strdup(&str[j]);
	return (ft_strjoin_ff(ft_strdup("$"), after));
}
