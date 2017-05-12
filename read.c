/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfilipch <mfilipch@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/15 21:48:16 by mfilipch          #+#    #+#             */
/*   Updated: 2016/10/15 21:48:18 by mfilipch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytracer.h"

static int	ft_chk_content(t_data *d, char *line)
{
	static int	c;
	static int	row;

	if (ft_strcmp(line, "") == 0)
	{
		if (row == 1)
			d->nshp = c;
		row++;
		c = 0;
		return (0);
	}
	else if (ft_strncmp(line, "//", 2) == 0)
		return (0);
	if (row == 0 && c == 1)
		return (-1);
	c++;
	if (row == 2)
		d->nlght = c;
	return (1);
}

static int	ft_read_to_list(int fd, t_list **img_l, t_data *d)
{
	t_list	*tmp;
	char	*line;
	int		ret;

	line = NULL;
	while ((ret = get_next_line(fd, &line)) > 0 &&
		(ret = ft_chk_content(d, line)) >= 0)
	{
		if (ret > 0 && !(*img_l))
		{
			if (!(*img_l = ft_lstnew(line, ft_strlen(line) + 1)))
				ft_free_n_exit(d, img_l, line, -5);
			tmp = *img_l;
		}
		else if (ret > 0)
		{
			if (!(tmp->next = ft_lstnew(line, ft_strlen(line) + 1)))
				ft_free_n_exit(d, img_l, line, -5);
			tmp = tmp->next;
		}
		free(line);
	}
	(line) ? free(line) : 0;
	(ret == -1) ? ft_free_n_exit(d, img_l, NULL, -10) : 0;
	return (1);
}

static int	ft_split_n_rec(t_data *d, t_list *img_l)
{
	char	**tab_line;
	int		i;

	i = -1;
	while (img_l)
	{
		i++;
		if (!(tab_line = ft_strsplit(img_l->content, ' ')))
			return (-1);
		if (ft_rec_data(d, tab_line, i, -1) == -1)
			i = -1;
		ft_mapfree(&tab_line);
		if (i == -1)
			ft_free_n_exit(d, &img_l, NULL, -10);
		img_l = img_l->next;
	}
	return (0);
}

int			ft_read(char *filename, t_data *d)
{
	int		fd;
	t_list	*img_l;

	img_l = NULL;
	if ((fd = open(filename, O_RDONLY)) < 0)
		ft_free_n_exit(d, &img_l, NULL, -4);
	ft_read_to_list(fd, &img_l, d);
	if (fd && close(fd))
		ft_free_n_exit(d, &img_l, NULL, -6);
	if (!((d->shps) = (t_shp*)ft_memalloc(sizeof(t_shp) * d->nshp)))
		ft_free_n_exit(d, &img_l, NULL, -5);
	if (!((d->lght) = (t_lght*)ft_memalloc(sizeof(t_lght) * d->nlght)))
		ft_free_n_exit(d, &img_l, NULL, -5);
	if (ft_split_n_rec(d, img_l) < 0)
		ft_free_n_exit(d, &img_l, NULL, -5);
	ft_lstclr(&img_l);
	return (0);
}
