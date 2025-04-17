/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr_pf.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:54:39 by hho-troc          #+#    #+#             */
/*   Updated: 2025/02/06 11:18:30 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* void	ft_putptr_pf(void *ptr, size_t *counter)
{
	if (!ptr)
	{
		ft_putstr_pf("(nil)", counter);
		return ;
	}
	ft_putstr_pf("0x", counter);
	ft_puthex_pf((unsigned long)ptr, counter, HEX_LOW_BASE);
} */

void	ft_putptr_pf(void *ptr, size_t *counter)
{
	char			*str;
	unsigned long	ptr_address;

	if (!ptr)
	{
		ft_putstr_pf("(nil)", counter);
		return ;
	}
	ptr_address = (unsigned long)ptr;
	ft_putstr_pf("0x", counter);
	str = ft_aux_pf(ptr_address, HEX_LOW_BASE);
	ft_putstr_pf(str, counter);
	free(str);
}
