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
#include <stdio.h>

static int	chk_content(t_data *d, char *line)
{
	static int	c;
	static int	row;

	// printf("line %s c %d row %d\n", line, c, row);
	// printf("cmp my %d lib %d\n", ft_strncmp(line, "//", 2), strncmp(line, "//", 2));
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
		(ret = chk_content(d, line)) >= 0)
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
	(line) ? free(line) : 0;
	(ret == -1) ? ft_free_n_exit(d, img_l, NULL, -10) : 0;
	return (1);
}

static int	ft_record(t_data *d, char **tab_line, int row, int j)
{
	while (tab_line[++j])
	{
		if (row == 0)
		{
			(j == 0) ? d->depth = ft_atof(tab_line[j]) : 0;
			(j == 1) ? d->pos.x = ft_atof(tab_line[j]) : 0;
			(j == 2) ? d->pos.y = ft_atof(tab_line[j]) : 0;
			(j == 3) ? d->pos.z = ft_atof(tab_line[j]) : 0;
			(j == 4) ? d->vwp.x = ft_atof(tab_line[j]) : 0;
			(j == 5) ? d->vwp.y = ft_atof(tab_line[j]) : 0;
			(j == 6) ? d->vwp.z = ft_atof(tab_line[j]) : 0;
			(j == 7) ? d->max_dist = ft_atof(tab_line[j]) : 0;
		}
		if (row > 0 && row < d->nshp + 1)
		{
			(j == 0) ? d->shps[row - 1].id = ft_atof(tab_line[j]) : 0;
			(j == 1) ? d->shps[row - 1].o.x = ft_atof(tab_line[j]) : 0;
			(j == 2) ? d->shps[row - 1].o.y = ft_atof(tab_line[j]) : 0;
			(j == 3) ? d->shps[row - 1].o.z = ft_atof(tab_line[j]) : 0;
			(j == 4) ? d->shps[row - 1].l = ft_atof(tab_line[j]) : 0;
			(j == 5) ? d->shps[row - 1].h = ft_atof(tab_line[j]) : 0;
			(j == 6) ? d->shps[row - 1].mu.x = ft_atof(tab_line[j]) : 0;
			(j == 7) ? d->shps[row - 1].mu.y = ft_atof(tab_line[j]) : 0;
			(j == 8) ? d->shps[row - 1].mu.z = ft_atof(tab_line[j]) : 0;
			(j == 9) ? d->shps[row - 1].nm.x = ft_atof(tab_line[j]) : 0;
			(j == 10) ? d->shps[row - 1].nm.y = ft_atof(tab_line[j]) : 0;
			(j == 11) ? d->shps[row - 1].nm.z = ft_atof(tab_line[j]) : 0;
			(j == 12) ? d->shps[row - 1].n = ft_atof(tab_line[j]) : 0;
		}
		if (row >= d->nshp + 1 && row < d->nshp + d->nlght + 1)
		{
			(j == 0) ? d->lght[row - d->nshp - 1].o.x = ft_atof(tab_line[j]) : 0;
			(j == 1) ? d->lght[row - d->nshp - 1].o.y = ft_atof(tab_line[j]) : 0;
			(j == 2) ? d->lght[row - d->nshp - 1].o.z = ft_atof(tab_line[j]) : 0;		
			(j == 3) ? d->lght[row - d->nshp - 1].spctr.x = ft_atof(tab_line[j]) : 0;
			(j == 4) ? d->lght[row - d->nshp - 1].spctr.y = ft_atof(tab_line[j]) : 0;
			(j == 5) ? d->lght[row - d->nshp - 1].spctr.z = ft_atof(tab_line[j]) : 0;
			(j == 6) ? d->lght[row - d->nshp - 1].l = ft_atof(tab_line[j]) : 0;
			(j == 7) ? d->lght[row - d->nshp - 1].I = ft_atof(tab_line[j]) : 0;
		}
	}
	if (row == 0 && j < 8)
		return (-1);
	if (row > 0 && row < d->nshp + 1 && j < 13)
		return (-1);
	if (row >= d->nshp + 1 && row < d->nshp + d->nlght + 1 && j < 8)
		return (-1);
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
		if (ft_record(d, tab_line, i, -1) == -1)
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
	// if ((rows = ft_read_to_list(fd, &img_l, d)) <= 0)
	// 	ft_free_n_exit(d, &img_l, NULL, -10);
	// if (!(d->img = (t_3d***)ft_memalloc(sizeof(t_3d**))))
	// 	ft_free_n_exit(d, &img_l, NULL, -5);
	// d->img_size.z = 1;
	if (!((d->shps) = (t_shp*)ft_memalloc(sizeof(t_shp) * d->nshp)))
		ft_free_n_exit(d, &img_l, NULL, -5);
	if (!((d->lght) = (t_lght*)ft_memalloc(sizeof(t_lght) * d->nlght)))
		ft_free_n_exit(d, &img_l, NULL, -5);
	// d->img_size.y = rows;
	if (ft_split_n_rec(d, img_l) < 0)
		ft_free_n_exit(d, &img_l, NULL, -5);
	ft_lstclr(&img_l);
	return (0);
}
