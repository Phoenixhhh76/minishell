/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:53:15 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/30 10:46:42 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tab_int(int **tab, int size)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (i < size)
	{
		if (tab[i])
		{
			if (tab[i][0] >= 0)
				close(tab[i][0]);
			if (tab[i][1] >= 0)
				close(tab[i][1]);
			free(tab[i]);
		}
		i++;
	}
	free(tab);
}

void	free_double_tab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

/* for exit_error */
void	free_strs(char *str, char **strs)
{
	int	i;

	if (str != NULL)
	{
		free(str);
		str = NULL;
	}
	if (strs != NULL)
	{
		i = 0;
		while (strs[i])
		{
			free(strs[i]);
			i++;
		}
		free(strs);
		strs = NULL;
	}
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

char	*ft_strjoin_ff(char *s1, char *s2)
{
	char	*joined;
	char	*tmp;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
	{
		tmp = ft_strdup(s2);
		free(s2);
		return (tmp);
	}
	if (!s2)
	{
		free(s1);
		return (NULL);
	}
	joined = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (joined);
}

// char	*ft_strjoin_ff(char *s1, char *s2)
// {
// 	char	*joined;

// 	if (!s1 && !s2)
// 		return (NULL);
// 	if (!s1)
// 		return (free(s2), NULL);
// 	if (!s2)
// 		return (free(s1), NULL);
// 	joined = ft_strjoin(s1, s2);
// 	free(s1);
// 	free(s2);
// 	return (joined);
// }
