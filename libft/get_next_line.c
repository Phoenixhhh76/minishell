/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 13:25:38 by hho-troc          #+#    #+#             */
/*   Updated: 2025/02/06 13:25:38 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_ptr(char **ptr)
{
	free(*ptr);
	*ptr = NULL;
}

static char	*get_line(char **backup, char **line)
{
	char	*next_backup;
	int		i;

	i = 0;
	next_backup = NULL;
	while (*(*backup + i) != '\n' && *(*backup + i) != '\0')
		i++;
	if (*(*backup + i) == '\n')
	{
		i++;
		*line = ft_substr(*backup, 0, i);
		next_backup = ft_strdup(*backup + i);
	}
	else
		*line = ft_strdup(*backup);
	ft_free_ptr(backup);
	return (next_backup);
}

static int	read_line(int fd, char **buffer, char **backup, char **line)
{
	int		bytes_read;
	char	*temporary;

	bytes_read = 1;
	while (!ft_strchr(*backup, '\n') && bytes_read)
	{
		bytes_read = read(fd, *buffer, BUFFER_SIZE);
		(*buffer)[bytes_read] = '\0';
		temporary = *backup;
		*backup = ft_strjoin(temporary, *buffer);
		free(temporary);
	}
	ft_free_ptr(buffer);
	*backup = get_line(backup, line);
	if (!(**line))
		ft_free_ptr(line);
	return (bytes_read);
}

char	*get_next_line(int fd)
{
	static char	*buffer_backup = NULL;
	char		*buffer;
	char		*line;

	if (fd < 0 || fd >= 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	if (read(fd, buffer, 0) < 0)
	{
		free(buffer);
		return (NULL);
	}
	if (!buffer_backup)
		buffer_backup = ft_strdup("");
	if (!read_line(fd, &buffer, &buffer_backup, &line) && !line)
		return (NULL);
	return (line);
}
