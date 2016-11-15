/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rec_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfilipch <mfilipch@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/14 19:32:49 by mfilipch          #+#    #+#             */
/*   Updated: 2016/11/14 19:32:50 by mfilipch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytracer.h"

static void	ft_rec_main(t_data *d, char *elem, int j)
{
	(j == 0) ? d->depth = ft_atof(elem) : 0;
	(j == 1) ? d->pos.x = ft_atof(elem) : 0;
	(j == 2) ? d->pos.y = ft_atof(elem) : 0;
	(j == 3) ? d->pos.z = ft_atof(elem) : 0;
	(j == 4) ? d->vwp.x = ft_atof(elem) : 0;
	(j == 5) ? d->vwp.y = ft_atof(elem) : 0;
	(j == 6) ? d->vwp.z = ft_atof(elem) : 0;
	(j == 7) ? d->max_dist = ft_atof(elem) : 0;
}

static void	ft_rec_shape(t_data *d, char *elem, int i, int j)
{
	(j == 0) ? d->shps[i].id = ft_atof(elem) : 0;
	(j == 1) ? d->shps[i].o.x = ft_atof(elem) : 0;
	(j == 2) ? d->shps[i].o.y = ft_atof(elem) : 0;
	(j == 3) ? d->shps[i].o.z = ft_atof(elem) : 0;
	(j == 4) ? d->shps[i].l = ft_atof(elem) : 0;
	(j == 5) ? d->shps[i].h = ft_atof(elem) : 0;
	(j == 6) ? d->shps[i].mu.x = ft_atof(elem) : 0;
	(j == 7) ? d->shps[i].mu.y = ft_atof(elem) : 0;
	(j == 8) ? d->shps[i].mu.z = ft_atof(elem) : 0;
	(j == 9) ? d->shps[i].nm.x = ft_atof(elem) : 0;
	(j == 10) ? d->shps[i].nm.y = ft_atof(elem) : 0;
	(j == 11) ? d->shps[i].nm.z = ft_atof(elem) : 0;
	(j == 12) ? d->shps[i].n = ft_atof(elem) : 0;
}

static void	ft_rec_light(t_data *d, char *elem, int i, int j)
{
	(j == 0) ? d->lght[i].o.x = ft_atof(elem) : 0;
	(j == 1) ? d->lght[i].o.y = ft_atof(elem) : 0;
	(j == 2) ? d->lght[i].o.z = ft_atof(elem) : 0;
	(j == 3) ? d->lght[i].spctr.x = ft_atof(elem) : 0;
	(j == 4) ? d->lght[i].spctr.y = ft_atof(elem) : 0;
	(j == 5) ? d->lght[i].spctr.z = ft_atof(elem) : 0;
	(j == 6) ? d->lght[i].l = ft_atof(elem) : 0;
	(j == 7) ? d->lght[i].I = ft_atof(elem) : 0;
}

int			ft_rec_data(t_data *d, char **t_l, int row, int j)
{
	while (t_l[++j])
	{
		if (row == 0)
			ft_rec_main(d, t_l[j], j);
		if (row > 0 && row < d->nshp + 1)
			ft_rec_shape(d, t_l[j], row - 1, j);
		if (row >= d->nshp + 1 && row < d->nshp + d->nlght + 1)
			ft_rec_light(d, t_l[j], row - d->nshp - 1, j);
	}
	if (row == 0 && j < 8)
		return (-1);
	if (row > 0 && row < d->nshp + 1 && j < 13)
		return (-1);
	if (row >= d->nshp + 1 && row < d->nshp + d->nlght + 1 && j < 8)
		return (-1);
	return (1);
}
