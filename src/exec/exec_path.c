/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:30:57 by hho-troc          #+#    #+#             */
/*   Updated: 2025/04/23 16:02:44 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strjoin_with(const char *s1, const char *s2, char sep)
{
	size_t	len1;
	size_t	len2;
	size_t	i;
	size_t	j;
	char	*res;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	res = malloc(len1 + len2 + 2);
	i = 0;
	j = 0;
	if (!res)
		return (NULL);
	while (i < len1)
	{
		res[i] = s1[i];
		i++;
	}
	res[i++] = sep;
	while (j < len2)
		res[i++] = s2[j++];
	res[i] = '\0';
	return (res);
}

char	*get_path_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

char	*resolve_cmd_path(char *cmd, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	path_env = get_path_env(envp);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin_with(paths[i], cmd, '/');
		if (access(full_path, X_OK) == 0)
		{
			ft_free_char2(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_char2(paths);
	return (NULL);
}

