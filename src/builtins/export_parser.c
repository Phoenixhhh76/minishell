/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:11:58 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/20 12:34:08 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*strip_quotes_only(const char *val)
{
	size_t	len;

	if (!val)
		return (NULL);
	len = ft_strlen(val);
	if (len >= 2 && ((val[0] == '"' && val[len - 1] == '"') || \
					(val[0] == '\'' && val[len - 1] == '\'')))
		return (ft_substr(val, 1, len - 2));
	else
		return (ft_strdup(val));
}

char	**split_export_arg(const char *arg)
{
	char	*equal;
	size_t	name_len;
	char	*name;
	char	**out;

	if (!arg)
		return (NULL);
	equal = ft_strchr(arg, '=');
	if (!equal || equal == arg)
		return (NULL);
	name_len = equal - arg;
	name = ft_strndup(arg, name_len);
	out = ft_calloc(3, sizeof(char *));
	if (!out)
		return (free(name), NULL);
	out[0] = name;
	out[1] = ft_strdup(equal + 1);
	if (!out[0] || !out[1])
	{
		free_double_tab(out);
		return (NULL);
	}
	return (out);
}

char	*make_joined_assignment(char **var)
{
	char	*cleaned_val;
	char	*joined;

	cleaned_val = strip_quotes_only(var[1]);
	if (!cleaned_val)
		return (NULL);
	joined = ft_strjoin(var[0], "=");
	if (!joined)
		return (free(cleaned_val), NULL);
	joined = ft_strjoin_f(joined, cleaned_val);
	free(cleaned_val);
	return (joined);
}
