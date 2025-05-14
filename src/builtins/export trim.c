/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 14:44:38 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/13 14:12:27 by hho-troc         ###   ########.fr       */
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
	// if (!ft_strchr(var, '='))
	// 	return (false);
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
/*
static char	*strip_quotes_and_trim(const char *val)
In classic situations,
like echo "  bonjour he  " | cat -e -> we have to keep all spaces in quotes.
But in export, it's different:
	minishell> export hola=" bonjour he "
	Token [export]  quote_type 0
	Token [hola=" bonjour he "]  quote_type 0
	[split_export_arg] var[0]=[hola], var[1]=[" bonjour he "]
	[DEBUG] var[1] addr: 0xb26fc9b896f0
	[DEBUG] trimmed val addr: 0xb26fc9b8a290
	export debug: var[0]=[hola], var[1]=[" bonjour he "]
	export debug: cleaned_val=[bonjour he]
	export debug: joined=[hola=bonjour he]
	export assigned: [hola=bonjour he]
	minishell> echo $hola
	Token [echo]  quote_type 0
	Token [$hola]  quote_type 0
	bonjour he   ------->no space in quotes,
We need removes quotes from the beginning and end of a string, if they exist.
Also trims leading and trailing whitespace.
*/
static char	*strip_quotes_and_trim(const char *val)
{
	char	*val_copy;
	char	*stripped;
	//char	*trimmed;
	size_t	len;

	if (!val)
		return (NULL);
	len = ft_strlen(val);
	val_copy = ft_strdup(val);
	if (!val_copy)
		return (NULL);
	if (len >= 2 && ((val_copy[0] == '"' && val_copy[len - 1] == '"') || \
					(val_copy[0] == '\'' && val_copy[len - 1] == '\'')))
		stripped = ft_substr(val_copy, 1, len - 2);
	else
		stripped = ft_strdup(val_copy);
	free(val_copy);
	if (!stripped)
		return (NULL);
	// trimmed = ft_strtrim(stripped, " \t");
	// free(stripped);
	//printf("[DEBUG] trimmed val addr: %p\n", (void *)trimmed);
	// if (trimmed)
	// 	return (trimmed);
	else
		return (ft_strdup(""));
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
	out[1] = ft_strdup(equal + 1); // ✅ 無論值是空字串還是內容都處理
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

	printf("[DEBUG] var[1] addr: %p\n", (void *)var[1]); // ✅ Debug memory
	cleaned_val = strip_quotes_and_trim(var[1]);
	if (!cleaned_val)
		return (NULL);

	printf("export debug: var[0]=[%s], var[1]=[%s]\n", var[0], var[1] ? var[1] : "(null)");
	printf("export debug: cleaned_val=[%s]\n", cleaned_val);

	joined = ft_strjoin(var[0], "=");
	if (!joined)
		return (free(cleaned_val), NULL);
	joined = ft_strjoin_f(joined, cleaned_val);
	free(cleaned_val);
	printf("export debug: joined=[%s]\n", joined);
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
	printf("export assigned: [%s]\n", joined);
	return (0);
}

int	export_err_msg(char *arg, int erno)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (erno);
}

int	handle_single_export(char *arg, char ***mini_env, t_mini *mini)
{
	char	**env;
	char	**var;
	char	*joined;
	int		env_index;

	env = *mini_env;
	if (!arg || !arg[0] || arg[0] == '=' || !is_valid_var_name(arg))
	{
		mini->last_exit = 1;
		return (export_err_msg(arg, 1));
	}
	if (!ft_strchr(arg, '=')) // export abc without value
	{
		if (does_var_exist(env, arg) < 0)
		{
			// add as empty string
			char *tmp = ft_strjoin(arg, "=");
			if (!tmp)
				return (-1);
			if (add_var_to_env(env, tmp, mini_env) < 0)
			{
				free(tmp);
				return (-1);
			}
			free(tmp);
		}
		return (0);
	}
	var = split_export_arg(arg);
	if (!var || !var[0] || var[0][0] == '\0')
		return (free_double_tab(var), -1);
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
	//for export without args
	if (!cmd || !cmd->cmd_args || !cmd->cmd_args[1])
	{
		print_export_env(*mini_env);
		return (0);
	}
	while (cmd->cmd_args[i])
	{
		if (handle_single_export(cmd->cmd_args[i], mini_env, mini) < 0)
			status = 1; // error, to check with exit code
		i++;
	}
	return (status);
}











