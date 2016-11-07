/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytrace.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfilipch <mfilipch@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/05 00:26:57 by mfilipch          #+#    #+#             */
/*   Updated: 2016/11/05 00:26:59 by mfilipch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytrace.h"

t_3di	intersect(t_data *d, t_3di p1, t_3di p2, int n)
{
	t_3di p

	p.x = -1;
	p.y = -1;
	p.z = -1;
	              
	return (p);
}

t_3di	cast_shadray(t_data *d, t_3di p1, t_lght lght, int depth)
{
	int		i;
	double 	min_dist;
	double	dist
	t_3di	p;

	i = -1;
	*n = -1;
	min_dist = d->max_dist;
	while (++i < d->nshp)
	{
		if ((p = intersect(d, lght.o, p1, i)).x != -1
			/*&& (dist = distance(d->pos, p)) < min_dist*/)
			if (depth == 1) 
			{
				p.x = 0;
				p.y = 0;
				p.z = 0;
				return(p);
			}
	}
	return (light.spctr);
}

t_3di	cast_primray(t_data *d, t_3di p1, int *n)
{
	int		i;
	double 	min_dist;
	double	dist
	t_3di	p;

	i = -1;
	*n = -1;
	min_dist = d->max_dist;
	while (++i < d->nshp)
	{
		if ((p = intersect(d, d->pos, p1, i)).x != -1 
			&& (dist = distance(d->pos, p)) < min_dist)
		{
			min_dist = dist;
			*n = i;
		}
	}
	return (p);
}

void	raytrace(t_data *d)
{
	t_3di	p1;
	t_3di	p;
	int		n;
	int 	i;
	t_3di	col;
	t_3di	colinc;

	p1.y = -1;
	while (++p1.y < YS && (p1.x = -1))
		while (++p1.x < XS && (i = -1))
		{
			col = 0;
			p = cast_primray(d, p1, &n);
			while (++i < d->nlght)
			{
				colinc = cast_shadray(d, p, d->lght[i], 1);
				col.x = col.x + colinc.x * (1 - d->shp[n].mu.x);
				col.x = (col.x > 255) ? 255: col.x;
				col.y = col.y + colinc.y * (1 - d->shp[n].mu.y);
				col.y = (col.y > 255) ? 255: col.y;
				col.z = col.z + colinc.z * (1 - d->shp[n].mu.z);
				col.z = (col.z > 255) ? 255: col.z;
			}
			draw_pixel(d, p1.x, p1.y, (col.x << 16) + (col.y << 8) + col.z);
		}
}

int		ft_drawit(t_data *d)
{
	d->img_p = mlx_new_image(d->mlx, XS, YS);
	d->img_p0 = mlx_get_data_addr(d->img_p, &(d->bpp), &(d->ls), &(d->endian));
	raytrace(d);
	mlx_clear_window(d->mlx, d->win);
	mlx_put_image_to_window(d->mlx, d->win, d->img_p, 0, 0);
	mlx_destroy_image(d->mlx, d->img_p);
	return (0);
}
