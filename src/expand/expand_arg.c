/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:05:52 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/26 19:11:36 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* for "'$USER'" */
char	*expand_if_needed(t_token *token, t_mini *mini)
{
	if (token->quote_type == Q_S)
		return (ft_strdup(token->str));
	if (token->quote_type == Q_D && token->is_dollar_quote)
		return (ft_strdup(token->str));
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

char	*expand_arg(const char *str, t_mini *mini, t_quote quote_type, bool is_dollar_quote)
{
	char	*result;
	int		i;
	int		old_i;

	result = ft_strdup("");
	i = 0;
	if (is_dollar_quote)
		return (ft_strdup(str));
	if (str[0] == '"' && str[1] == '$' && str[2] == '"' && str[3] == '"')
		return (handle_special_case(str));
	while (str[i])
	{
		old_i = i;
		if (str[i] == '\'' && quote_type == Q_NONE)
			result = ft_strjoin_ff(result, handle_single_quote(str, &i));
		else if (str[i] == '"')
			result = ft_strjoin_ff(result, handle_double_quote(str, &i, mini));
		else if (str[i] == '$')
			result = ft_strjoin_ff(result, expand_var(str, &i, mini));
		else
			result = ft_strjoin_ff(result, ft_strndup(str + i++, 1));
		if (i == old_i)
			i++;
	}
	return (result);
}

