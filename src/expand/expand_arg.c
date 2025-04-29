/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:05:52 by hho-troc          #+#    #+#             */
/*   Updated: 2025/04/25 17:22:52 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* for "'$USER'" */
char	*expand_if_needed(t_token *token, t_mini *mini)
{
	if (token->quote_type == QUOTE_SINGLE)
		return (ft_strdup(token->str));
	return (expand_arg(token->str, mini));
}


char	*ft_strjoin_f(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
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
	if (str[start] == '?')// $? is for exit code
	//WEXITSTATUS(status) ????
	{
		(*i)++;
		return (ft_itoa(mini->last_exit));
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	var = ft_strndup(str + start, *i - start);
	val = get_env_value(var, mini->env);
	free(var);
	return (val);
}

/* char *expand_arg(const char *str, t_mini *mini)
{
	char	*result;
	int		i;
	int		start;

	i = 0;
	result = calloc(1, sizeof(char));
	while (str[i])
	{
		if (str[i] == '\'') // '$USER' can't be expand
		{
			start = ++i;
			while (str[i] && str[i] != '\'')
				i++;
			result = ft_strjoin_f(result, ft_strndup(str + start, i - start));
			if (str[i] == '\'')
				i++;
		}
		else if (str[i] == '"') // "$USER" can be expand
		{
			start = ++i;
			while (str[i] && str[i] != '"')
			{
				if (str[i] == '$')
				result = ft_strjoin_f(result, expand_var(str, &i, mini));
				elseecho abcd"$USERAAA$PATH"
					result = ft_strjoin_f(result, ft_strndup(str + i++, 1));
			}
			if (str[i] == '"')
				i++;
		}
		else if (str[i] == '$') // $USER...etc
			result = ft_strjoin_f(result, expand_var(str, &i, mini));
		else
			result = ft_strjoin_f(result, ft_strndup(str + i++, 1));
	}
	return (result);
} */

char	*expand_arg(const char *str, t_mini *mini)
{
	char	*result;
	int		i;

	i = 0;
	result = calloc(1, sizeof(char));
	while (str[i])
	{
		if (str[i] == '"')
		{
			i++;
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
