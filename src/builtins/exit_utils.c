/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 10:32:55 by ndabbous          #+#    #+#             */
/*   Updated: 2025/05/31 11:54:11 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	convert_and_compare(const char *nptr, int i, int sign)
{
	long long	nb;
	int			digit;

	nb = 0;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		digit = nptr[i] - '0';
		if (sign == 1 && (nb > (LLONG_MAX - digit) / 10))
			return (false);
		if (sign == -1 && (-nb < (LLONG_MIN + digit) / 10))
			return (false);
		nb = nb * 10 + digit;
		i++;
	}
	return (true);
}

bool	ft_is_long_limits(const char *nptr)
{
	int			i;
	int			sign;
	//long long	nb;

	i = 0;
	//nb = 0;
	sign = 1;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	if (!convert_and_compare(nptr, i, sign))
		return (false);
	return (true);
}

bool	ft_isnumeric(char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (false);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

int	args_count(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}
