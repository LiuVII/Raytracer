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

#include "raytracer.h"
#include <stdio.h>

t_3di	cast_shadray(t_data *d, t_3di p1, t_lght lght, int depth)
{
	int		i;
	double 	ratio;
	t_3di	p;

	i = -1;
	while (++i < d->nshp)
	{
		p = intersect(d, p1, lght.o, i);
		// if (i == 0)
		// 		printf(" %d, %d, %d |", p.x, p.y, p.z);
		if (v_iscal(v_id2v(p1, p), v_id2v(p1, lght.o)) > 0.1 &&
			v_imodsq(v_id2v(p1, p)) <= v_imodsq(v_id2v(p1, lght.o)))
		{
			// if (i == 0)
			// 	printf(" %d, %d, %d |", p.x, p.y, p.z);
			if (depth == 1) 
				return(v_isop(p, 0, '='));
		}
		// else if (d->shps[i].id == 2 && p1.x < XS / 7 && v_imodsq(v_id2v(p1, p)) != 0)
		// 	printf(" %d %d %.f %.f |", v_id2v(p1, p).x, v_id2v(p1, p).y, v_imod(v_id2v(p1, p)), v_iscal(v_id2v(p1, p), v_id2v(p1, lght.o)));
	}
	if (v_imodsq(v_id2v(p1, lght.o)) > 0 && (ratio = (lght.l * lght.l)  / v_imodsq(v_id2v(p1, lght.o))))
	{
		// printf(" %d %d %d |", p1.x, p1.y, p1.z);
		// printf(" %.f |", ratio * lght.I);
		return(v_isop(lght.spctr, (ratio * lght.I), '*')); 
	}
	return (lght.spctr);
}

t_3di	cast_primray(t_data *d, t_3di p2, int *n)
{
	int		i;
	double 	min_dist;
	double	dist;
	t_3di	p;
	t_3di	pi;

	i = -1;
	*n = -1;
	pi = d->pos;
	min_dist = d->max_dist;
	while (++i < d->nshp)
	{
		p = intersect(d, d->pos, p2, i);
		if (v_imodsq(v_id2v(d->pos, p)) > 0.1)
		{
			// if (*n == 0)
			// 	printf(" %d, %d, %d |", p.x, p.y, p.z);
			// printf(" %d, %d, %d ", p.x, p.y, p.z);
			// printf(" 1 ");
			// if (isnan(sqrt(v_imodsq(v_id2v(d->pos, p)))))
			// 	printf(" %d, %d, %d ", p.x, p.y, p.z);
				// printf(" %.f ", min_dist - sqrt(v_imodsq(v_id2v(d->pos, p))));
			if ((dist = sqrt(v_imodsq(v_id2v(d->pos, p)))) < min_dist)
			{
				// printf("2 |");
				min_dist = dist;
				*n = i;
				pi = p;
			}
			// printf("|");
		}
	}
	return (pi);
}

t_3di	trace_one(t_data *d, t_3di p, int i, int n)
{
	t_3di	col;
	t_3di	colinc;
	t_3di	colgl;

	col = v_isop(p, 0, '=');
	p = cast_primray(d, v_ivop(d->vwp, p, '+'), &n);
	if (n >= 0)
		while (++i < d->nlght)
		{
			colgl = v_isop(colgl, 0, '=');
			// printf("\n\n");
			// printf("sh %d lt %d |", n, i);
			// colinc = v_isop(colinc, 120, '=');
			colinc = cast_shadray(d, p, d->lght[i], 1);
			if (d->shps[n].n > 1.01 && v_imodsq(colinc) > 1 && i > 0)
				colgl = add_gloss(v_id2v(p, d->pos), p, d->lght[i], d->shps[n]);
			col = v_ivop(col, colgl, '+');
			col.x = col.x + colinc.x * (1 - d->shps[n].mu.x);
			col.x = (col.x > 255) ? 255 : col.x;
			col.y = col.y + colinc.y * (1 - d->shps[n].mu.y);
			col.y = (col.y > 255) ? 255 : col.y;
			col.z = col.z + colinc.z * (1 - d->shps[n].mu.z);
			col.z = (col.z > 255) ? 255 : col.z;
			// col.x = 0;
			// col.y = 255;
			// col.z = 0;
			// printf(" %.f, %.f, %.f |", col.x - colinc.x *
			//(1 - d->shps[n].mu.x), col.y - colinc.y * (1 - d->shps[n].mu.y), col.z - colinc.z * (1 - d->shps[n].mu.z));
			// printf(" %.f, %.f, %.f |", (1 - d->shps[n].mu.x), (1 - d->shps[n].mu.y), (1 - d->shps[n].mu.z));
		}
	return (col);
}

void	rotate_view(t_3d *ox, t_3d *oy, t_3d p)
{
	if (p.z != -1 && v_dmodsq(p) != 0)
	{
		p = v_dsop(p, 1 / sqrt(v_dmodsq(p)), '*');
		ox->x = (1.0 - SQ(p.x) / (1.0 + p.z));
		ox->y = -(p.y * p.x) / (1.0 + p.z);
		ox->z = -p.x;
		oy->x = -(p.y * p.x) / (1.0 + p.z);
		oy->y = 1.0 - SQ(p.y) / (1.0 + p.z);
		oy->z = -p.y;
	}
	else
	{
		ox->x = -1;
		ox->y = 0;
		ox->z = 0;
		oy->x = 0;
		oy->y = 1;
		oy->z = 0;
	}
}

void	raytrace(t_data *d)
{
	t_3di	p1;
	t_3d	ox;
	t_3d	oy;
	t_3di	col;

	p1.y = -1;
	rotate_view(&ox, &oy, v_i2d(v_id2v(d->pos, d->vwp)));
	// printf("\n\n\n");
	while (++p1.y < YS && (p1.x = -1))
		while (++p1.x < XS)
		{
			col = trace_one(d, v_d2i(v_dvop(v_dsop(ox, (p1.x - XS / 2), '*')
				, v_dsop(oy, (p1.y - YS / 2), '*'), '+')), -1, -1);
			draw_pixel(d, p1.x, p1.y, (col.x << 16) + (col.y << 8) + col.z);
		}
}
