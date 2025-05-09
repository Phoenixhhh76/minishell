/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 14:44:38 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/09 16:21:09 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


/* static int begins_with_digits(char *str)
{
	int i = 0;
	while (ft_isdigit(str[i]))
		i++;
	return (i);
}

static bool is_authorized_character(char c)
{
	return (ft_isalnum(c) || c == '_');
}

// Updated to only check name part before '='
static bool is_valid_var_name(char *var)
{
	int i = 0;
	if (!var || !var[0] || var[0] == '=')
		return (false);
	if (begins_with_digits(var))
		return (false);
	while (var[i] && var[i] != '=')
	{
		if (!is_authorized_character(var[i]))
			return (false);
		i++;
	}
	return (true);
}

int add_var_to_env(char **env, char *cmd, char ***mini_env)
{
	int i = 0;
	int index = 0;
	char **new_env;

	while (env && env[index])
		index++;
	new_env = (char **)ft_calloc(index + 2, sizeof(char *));
	if (!new_env)
		return (-1);
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

static char *strip_quotes_and_trim(const char *val)
{
	char *stripped;
	char *trimmed;
	size_t len = ft_strlen(val);

	if (len >= 2 && ((val[0] == '"' && val[len - 1] == '"') ||
			(val[0] == '\'' && val[len - 1] == '\'')))
		stripped = ft_substr(val, 1, len - 2);
	else
		stripped = ft_strdup(val);
	if (!stripped)
		return (NULL);
	trimmed = ft_strtrim(stripped, " \t");
	free(stripped);
	return trimmed ? trimmed : ft_strdup("");
}

static char **split_export_arg_fallback(const char *arg, const char *fallback_val)
{
	if (!arg)
		return (NULL);
	char *equal = ft_strchr(arg, '=');
	if (!equal || equal == arg)
		return (NULL);

	size_t name_len = equal - arg;
	char *name = ft_strndup(arg, name_len);
	char **out = ft_calloc(3, sizeof(char *));
	if (!out)
	{
		free(name);
		return (NULL);
	}
	out[0] = name;
	if (equal[1] != '\0')
		out[1] = ft_strdup(equal + 1);
	else if (fallback_val)
		out[1] = ft_strdup(fallback_val);
	else
		out[1] = ft_strdup("");

	printf("[split_export_arg] var[0]=[%s], var[1]=[%s]\n",
		out[0] ? out[0] : "(null)", out[1] ? out[1] : "(null)");

	if (!out[0] || !out[1])
	{
		free_double_tab(out);
		return (NULL);
	}
	return out;
}

static char *make_joined_assignment(char **var)
{
	char *cleaned_val = strip_quotes_and_trim(var[1]);
	if (!cleaned_val)
		return (NULL);

	printf("export debug: var[0]=[%s], var[1]=[%s]\n", var[0], var[1] ? var[1] : "(null)");
	printf("export debug: cleaned_val=[%s]\n", cleaned_val);

	char *joined = ft_strjoin(var[0], "=");
	if (!joined)
		return (free(cleaned_val), NULL);
	joined = ft_strjoin_f(joined, cleaned_val);
	printf("export debug: joined=[%s]\n", joined);
	return joined;
}

static int handle_var_assignment(char **env, char *joined, char ***mini_env, int index)
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
		if (add_var_to_env(env, joined, mini_env) < 0)
			return (-1);
	}
	printf("export assigned: [%s]\n", joined);
	return (0);
}

int ft_export(t_cmd *cmd, char ***mini_env)
{
	int i = 1;
	int env_index;
	char **var;
	char *joined;
	char **env = *mini_env;

	if (!cmd || !cmd->cmd_args || !cmd->cmd_args[1])
		return (0);
	while (cmd->cmd_args[i])
	{
		char *fallback = ft_strdup("");
		int j = i + 1;
		while (cmd->cmd_args[j])
		{
			char *tmp = fallback;
			fallback = ft_strjoin_f(tmp, cmd->cmd_args[j]);
			fallback = ft_strjoin_f(fallback, " ");
			j++;
		}

		var = split_export_arg_fallback(cmd->cmd_args[i], fallback);
		free(fallback);
		if (!var || !var[0])
		{
			free_double_tab(var);
			fprintf(stderr, "export: invalid assignment\n");
			i++;
			continue;
		}
		if (!is_valid_var_name(var[0]))
		{
			perror("export: invalid identifier");
			free_double_tab(var);
			i++;
			continue;
		}
		joined = make_joined_assignment(var);
		if (!joined)
		{
			free_double_tab(var);
			i++;
			continue;
		}
		env_index = does_var_exist(env, var[0]);
		if (handle_var_assignment(env, joined, mini_env, env_index) < 0)
		{
			free_double_tab(var);
			free(joined);
			i++;
			continue;
		}
		env = *mini_env;
		free_double_tab(var);
		free(joined);
		i++;
	}
	return (0);
}
 */


 static int	begins_with_digits(char *str)
 {
	int	i;

	i = 0;
	while (ft_isdigit(str[i]))
		 i++;
	return (i);
 }

 static bool is_authorized_character(char c)
 {
	return (ft_isalnum(c) || c == '_');
 }

 // Updated to only check name part before '='
 static bool is_valid_var_name(char *var)
 {
	int	i;

	i = 0;
	if (!var || !var[0] || var[0] == '=')
		 return (false);
	if (begins_with_digits(var))
		 return (false);
	while (var[i] && var[i] != '=')
	{
		if (!is_authorized_character(var[i]))
			return (false);
		i++;
	}
	return (true);
 }

 int add_var_to_env(char **env, char *cmd, char ***mini_env)
 {
	int	index;
	char	**new_env;
	int	i;

	index = 0;
	i = 0;
	while (env && env[index])
		index++;
	new_env = (char **)ft_calloc(index + 2, sizeof(char *));
	if (!new_env)
		return (-1);
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

 static char *strip_quotes_and_trim(const char *val)
 {
	 char	*stripped;
	 char	*trimmed;
	 size_t	len;

	len = ft_strlen(val);
	if (len >= 2 && ((val[0] == '"' && val[len - 1] == '"') ||
			(val[0] == '\'' && val[len - 1] == '\'')))
		 stripped = ft_substr(val, 1, len - 2);
	 else
		stripped = ft_strdup(val);
	 if (!stripped)
		return (NULL);
	trimmed = ft_strtrim(stripped, " \t");
	free(stripped);
	return trimmed ? trimmed : ft_strdup("");
 }

 static char **split_export_arg_fallback(const char *arg, const char *fallback_val)
 {
	if (!arg)
		return (NULL);
	 char *equal = ft_strchr(arg, '=');
	 if (!equal || equal == arg)
		return (NULL);

	size_t name_len = equal - arg;
	char *name = ft_strndup(arg, name_len);
	char **out = ft_calloc(3, sizeof(char *));
	if (!out)
	{
		 free(name);
		 return (NULL);
	}
	out[0] = name;
	if (equal[1] != '\0')
		out[1] = ft_strdup(equal + 1);
	else if (fallback_val)
		out[1] = ft_strdup(fallback_val);
	else
		out[1] = ft_strdup("");

	printf("[split_export_arg] var[0]=[%s], var[1]=[%s]\n",
		out[0] ? out[0] : "(null)", out[1] ? out[1] : "(null)");

	if (!out[0] || !out[1])
	{
		free_double_tab(out);
		return (NULL);
	 }
	return (out);
 }

 static char	*make_joined_assignment(char **var)
 {
	 char *cleaned_val = strip_quotes_and_trim(var[1]);
	 if (!cleaned_val)
		 return (NULL);

	 printf("export debug: var[0]=[%s], var[1]=[%s]\n", var[0], var[1] ? var[1] : "(null)");
	 printf("export debug: cleaned_val=[%s]\n", cleaned_val);

	 char *joined = ft_strjoin(var[0], "=");
	 if (!joined)
		 return (free(cleaned_val), NULL);
	 joined = ft_strjoin_f(joined, cleaned_val);
	 free(cleaned_val);
	 printf("export debug: joined=[%s]\n", joined);
	 return joined;
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

 int	ft_export(t_cmd *cmd, char ***mini_env)
 {
	 int i = 1;
	 int env_index;
	 char **var;
	 char *joined;
	 char **env = *mini_env;

	 if (!cmd || !cmd->cmd_args || !cmd->cmd_args[1])
		 return (0);
	 while (cmd->cmd_args[i])
	 {
		 char *fallback = ft_strdup("");
		 int j = i + 1;
		 while (cmd->cmd_args[j])
		 {
			 char *tmp = fallback;
			 fallback = ft_strjoin_f(tmp, cmd->cmd_args[j]);
			 fallback = ft_strjoin_f(fallback, " ");
			 j++;
		 }

		 var = split_export_arg_fallback(cmd->cmd_args[i], fallback);
		 free(fallback);
		 if (!var || !var[0] || var[0][0] == '\0')
		 {
			 free_double_tab(var);
			 //fprintf(stderr, "export: invalid assignment of var\n");
			 i++;
			 continue;
		 }
		 joined = make_joined_assignment(var);
		 if (!joined)
		 {
			 free_double_tab(var);
			 i++;
			 continue;
		 }
		 if (!is_valid_var_name(var[0]))
		 {
			 fprintf(stderr, "export: invalid identifier\n");
			 free_double_tab(var);
			 free(joined);
			 i++;
			 continue;
		 }
		 env_index = does_var_exist(env, var[0]);
		 if (handle_var_assignment(env, joined, mini_env, env_index) < 0)
		 {
			 free_double_tab(var);
			 free(joined);
			 i++;
			 continue;
		}
		env = *mini_env;
		free_double_tab(var);
		free(joined);
		i++;
	 }
	return (0);
 }


