/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:46:14 by hho-troc          #+#    #+#             */
/*   Updated: 2025/02/05 17:34:49 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*get_line(char *save)
{
	int		i;
	char	*str;

	i = 0;
	if (!save || !save[i])
		return (NULL);
	while (save[i] && save[i] != '\n')
		i++;
	str = (char *)malloc(sizeof(char) * (i + 2));
	if (!str)
		return (NULL);
	i = 0;
	while (save[i] && save[i] != '\n')
	{
		str[i] = save[i];
		i++;
	}
	if (save[i] == '\n')
	{
		str[i] = save[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*get_read_save(int fd, char *save)
{
	char	*buff;
	int		n_bytes;

	buff = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buff)
		return (NULL);
	n_bytes = 1;
	while (!save || (!ft_strchr(save, '\n') && n_bytes))
	{
		n_bytes = read(fd, buff, BUFFER_SIZE);
		if (n_bytes == -1)
		{
			free(buff);
			free(save);
			return (NULL);
		}
		buff[n_bytes] = '\0';
		save = ft_strjoin(save, buff);
	}
	free(buff);
	return (save);
}

char	*get_save(char *save)
{
	int		i;
	int		pos;
	char	*str;

	i = 0;
	while (save[i] && save[i] != '\n')
		i++;
	if (!save[i])
	{
		free(save);
		return (NULL);
	}
	str = (char *)malloc(sizeof(char) * (ft_strlen(save) - i + 1));
	if (!str)
	{
		return (NULL);
	}
	i++;
	pos = 0;
	while (save[i])
		str[pos++] = save[i++];
	str[pos] = '\0';
	free(save);
	return (str);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*save[4096];

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > 4096)
		return (NULL);
	save[fd] = get_read_save(fd, save[fd]);
	if (!save[fd])
		return (NULL);
	line = get_line(save[fd]);
	save[fd] = get_save(save[fd]);
	if (!save[fd])
		save[fd] = NULL;
	return (line);
}
