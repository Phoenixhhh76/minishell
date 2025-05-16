/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 14:44:38 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/16 16:08:42 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	begins_with_digits(char *str)
{
	int	i;

	i = 0;
	while (ft_isdigit(str[i]))
		i++;
	return (i);
}

bool	is_authorized_character(char c)
{
	if (ft_isalnum(c))
		return (true);
	if (c == '_')
		return (true);
	else
		return (false);
}

bool	is_valid_var_name(char *var)
{
	int	i;

	i = 0;
	if (!var || !var[0] || var[0] == '=')
		return (false);
	if (begins_with_digits(var))
		return (false);
	while (var[i] && var[i] != '=')
	{
		if (is_authorized_character(var[i]) == false)
			return (false);
		i++;
	}
	return (true);
}

int	add_var_to_env(char **env, char *cmd, char ***mini_env)
{
	int		i;
	int		index;
	char	**new_env;

	i = 0;
	index = 0;
	while (env && env[index])
		index++;
	new_env = (char **)ft_calloc(index + 2, sizeof(char *));
	if (!new_env)
		return (-1); //alloc error
	while (i < index)
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
			return (free_double_tab(new_env), -1);
		i++;
	}
	new_env[i++] = ft_strdup(cmd);
	new_env[i] = NULL;
	free_double_tab(*mini_env);
	*mini_env = new_env;
	return (0);
}

static char	*strip_quotes_only(const char *val)
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

static char **split_export_arg(const char *arg)
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

static char	*make_joined_assignment(char **var)
{
	char	*cleaned_val;
	char	*joined;

	//printf("[DEBUG] var[1] addr: %p\n", (void *)var[1]); // ✅ Debug memory
	cleaned_val = strip_quotes_only(var[1]);
	if (!cleaned_val)
		return (NULL);
	//printf("export debug: var[0]=[%s], var[1]=[%s]\n", var[0], var[1] ? var[1] : "(null)");
	//printf("export debug: cleaned_val=[%s]\n", cleaned_val);
	joined = ft_strjoin(var[0], "=");
	if (!joined)
		return (free(cleaned_val), NULL);
	joined = ft_strjoin_f(joined, cleaned_val);
	free(cleaned_val);
	//printf("export debug: joined=[%s]\n", joined);
	return (joined);
}

 static int	handle_var_assignment(char **env, char *joined, char ***mini_env, int index)
{
	if (!joined)
		return (-1);
	if (index >= 0)
	{
		free(env[index]);
		env[index] = ft_strdup(joined);
		if (!env[index])
			return (-1);
	}
	else
	{
		// do not free joined here, ownership passes to add_var_to_env's strdup
		if (add_var_to_env(env, joined, mini_env) < 0)
			return (-1);
	}
	//printf("export assigned: [%s]\n", joined);
	return (0);
}

bool	in_exp_list(char **exp_list, const char *key)
{
	int	i;

	i = 0;
	while (exp_list && exp_list[i])
	{
		if (ft_strcmp(exp_list[i], key) == 0)
			return (true);
		i++;
	}
	return (false);
}

void	add_to_exp_list(char ***exp_list, const char *key)
{
	int		len;
	char	**new_list;
	int		i;

	len = 0;
	i = 0;
	while (*exp_list && (*exp_list)[len])
		len++;
	new_list = ft_calloc(len + 2, sizeof(char *));
	while (i < len)
	{
		new_list[i] = ft_strdup((*exp_list)[i]);
		i++;
	}
	new_list[i] = ft_strdup(key);
	new_list[i + 1] = NULL;
	free_double_tab(*exp_list);
	*exp_list = new_list;
}

void	remove_from_exp_list(char ***exp_list, const char *key)
{
	int		i;
	int		j;
	int		len;
	char	**new_list;

	i = 0;
	j = 0;
	len = 0;
	while (*exp_list && (*exp_list)[len])
		len++;
	new_list = ft_calloc(len, sizeof(char *));
	if (!new_list)
		return ;
	while ((*exp_list)[i])
	{
		if (ft_strcmp((*exp_list)[i], key) != 0)
			new_list[j++] = ft_strdup((*exp_list)[i]);
		i++;
	}
	new_list[j] = NULL;
	free_double_tab(*exp_list);
	*exp_list = new_list;
}


int	handle_single_export(char *arg, char ***mini_env, t_mini *mini)
{
	char	**env;
	char	**var;
	char	*joined;
	int		env_index;

	env = *mini_env;
	if (!arg || !arg[0] || arg[0] == '=' || !is_valid_var_name(arg))
		return (export_err_msg(arg, 1));
	if (!ft_strchr(arg, '='))
	{
		if (does_var_exist(env, arg) < 0 && !in_exp_list(mini->exp_list, arg))
			add_to_exp_list(&mini->exp_list, arg); // add in exp_list
		return (0);
	}
	var = split_export_arg(arg);
	if (!var || !var[0] || var[0][0] == '\0')
		return (free_double_tab(var), -1);
	if (in_exp_list(mini->exp_list, var[0]))
		remove_from_exp_list(&mini->exp_list, var[0]);
	joined = make_joined_assignment(var);
	if (!joined)
		return (free_double_tab(var), -1);
	env_index = does_var_exist(env, var[0]);
	if (handle_var_assignment(env, joined, mini_env, env_index) < 0)
		return (free_double_tab(var), free(joined), -1);
	free_double_tab(var);
	free(joined);
	return (0);
}


int	ft_export(t_cmd *cmd, char ***mini_env, t_mini *mini)
{
	int		i;
	int		status;

	i = 1;
	status = 0;
	if (!cmd || !cmd->cmd_args || !cmd->cmd_args[1])
	{
		print_export_env(*mini_env, mini->exp_list);
		return (0);
	}
	while (cmd->cmd_args[i])
	{
		if (handle_single_export(cmd->cmd_args[i], mini_env, mini) < 0)
			status = 1;
		i++;
	}
	mini->last_exit = status;
	return (status);
}










