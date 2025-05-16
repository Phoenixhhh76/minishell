/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:33:03 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/16 16:05:06 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	swap(char **a, char **b)
{
	char	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	sort_env(char **env)
{
	int	i;
	int	j;

	i = 0;
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (ft_strcmp(env[i], env[j]) > 0)
				swap(&env[i], &env[j]);
			j++;
		}
		i++;
	}
}

void	print_export_env(char **env, char **exp_list)
{
	int		i;
	char	**sorted;

	i = 0;
	if (!env && !exp_list)
		return ;
	sorted = clone_and_sort_env(env);
	if (sorted)
	{
		while (sorted[i])
		{
			print_sorted_env_line(sorted[i]);
			i++;
		}
		free_double_tab(sorted);
	}
	i = 0;
	while (exp_list && exp_list[i])
	{
		write(1, "declare -x ", 11);
		write(1, exp_list[i], ft_strlen(exp_list[i]));
		write(1, "\n", 1);
		i++;
	}
}

char	**clone_and_sort_env(char **env)
{
	int		i;
	int		count;
	char	**sorted;

	i = 0;
	count = 0;
	while (env[count])
		count++;
	sorted = ft_calloc(count + 1, sizeof(char *));
	if (!sorted)
		return (NULL);
	while (i < count)
	{
		sorted[i] = ft_strdup(env[i]);
		i++;
	}
	sorted[i] = NULL;
	sort_env(sorted);
	return (sorted);
}

void	print_sorted_env_line(const char *entry)
{
	const char	*equal;
	size_t		name_len;
	size_t		val_len;

	equal = ft_strchr(entry, '=');
	if (equal && equal != entry)
	{
		name_len = equal - entry;
		val_len = ft_strlen(equal + 1);
		write(1, "declare -x ", 11);
		write(1, entry, name_len);
		write(1, "=\"", 2);
		write(1, equal + 1, val_len);
		write(1, "\"\n", 2);
	}
	else
	{
		write(1, "declare -x ", 11);
		write(1, entry, ft_strlen(entry));
		write(1, "\n", 1);
	}
}


