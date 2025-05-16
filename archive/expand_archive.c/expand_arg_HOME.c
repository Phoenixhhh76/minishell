/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg_HOME.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 11:20:26 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/16 11:16:31 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *ft_strjoin_f(char *s1, char *s2)
{
	char *joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
}

char *get_env_value(const char *key, char **env)
{
	int i = 0;
	size_t key_len = ft_strlen(key);

	while (env[i])
	{
		if (!ft_strncmp(env[i], key, key_len) && env[i][key_len] == '=')
			return (ft_strdup(env[i] + key_len + 1));
		i++;
	}
	return (ft_strdup(""));
}

static char *expand_var(const char *str, int *i, t_mini *mini)
{
	int start = ++(*i);
	char *var, *val;

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

static void handle_dollar(const char *str, int *i, char **res, t_mini *mini)
{
	char *val = expand_var(str, i, mini);
	*res = ft_strjoin_f(*res, val);
}

static void append_plain(const char *str, int *i, char **res)
{
	int start = *i;

	while (str[*i] && str[*i] != '$')
		(*i)++;
	*res = ft_strjoin_f(*res, ft_strndup(str + start, *i - start));
}

char *expand_arg(const char *str, t_mini *mini, t_quote quote_type)
{
	char *res = ft_strdup("");
	int i = 0;

	while (str[i])
	{
		if (str[i] == '$' && quote_type != Q_S)
			handle_dollar(str, &i, &res, mini);
		else
			append_plain(str, &i, &res);
	}
	return (res);
}

char *expand_if_needed(t_token *token, t_mini *mini)
{
	if (token->quote_type == Q_S)
		return (ft_strdup(token->str));
	return expand_arg(token->str, mini, token->quote_type);
}
