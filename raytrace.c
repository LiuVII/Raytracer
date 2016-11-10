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

t_3di	intersect(t_data *d, t_3di p1, t_3di p2, int n)
{
	t_3di	p;
	double	ratio;
	t_3di	a;
	t_3di	b;

	a = v_id2v(p1, d->shps[n].o);
	b = v_id2v(p1, p2);
	// p = v_isop(p1, -1, '=');
	p = p1;
	ratio = 0;
	if (d->shps[n].id == 1 && v_imodsq(b) > 0) 
	{
		// printf("a: %.2f b: %.2f ab: %.2f, diff %.2f |", v_imod(a), v_imod(b), v_iscal(a, b), sqrt(v_imodsq(a) - SQ(v_iscal(a, b)) / v_imodsq(b)));
		if (SQ(d->shps[n].l) >
		(ratio = v_imodsq(a) - SQ(v_iscal(a, b)) / v_imodsq(b)))
		{
			// printf(" %.f %.f %.2f |", v_iscal(a, b), v_imodsq(b), v_iscal(a, b) / v_imodsq(b));
			// if (v_iscal(a, b) < 0)
			// 	printf(" 1x %d 1y %d 1z %d, 2x %d, 2y %d, 2z %d\n", p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
				// printf(" ax %d ay %d az %d, bx %d, by %d, bz %d\n", a.x, a.y, a.z, b.x, b.y, b.z);
			ratio = (v_iscal(a, b) / v_imodsq(b) -
				sqrt((SQ(d->shps[n].l) - ratio) / v_imodsq(b)));
			if (ABS(ratio) < 0.1)
				ratio = 2 * v_iscal(a, b) / v_imodsq(b);
			// printf(" %.2f |", ratio);
			p = v_ivop(p1, v_isop(b, ratio, '*'), '+');
		}
	}
	// else if (d->shps[n].id == 0)
	// {
	// 	ratio = 1 - v_iscal(v_id2v(d->shps[n].o, p2), v_ivop())
	// }             
	return (p);
}

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
		if (v_iscal(v_id2v(p1, p), v_id2v(p1, lght.o)) > 0.1 /*&& p.x > 0
			&& (dist = distance(d->pos, p)) < min_dist*/)
		{
			// if (i == 0)
			// 	printf(" %d, %d, %d |", p.x, p.y, p.z);
			if (depth == 1) 
				return(v_isop(p, 0, '='));
		}
	}
	// !LIGHT INTENSITY!
	if (v_imodsq(v_id2v(p1, lght.o)) > 0 && (ratio = (lght.l * lght.l)  / v_imodsq(v_id2v(p1, lght.o))))
	{
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

t_3di	add_gloss(t_data *d, t_3di p, t_lght lght, int num)
{
	t_3di	a;
	t_3di	c;
	t_3di	b;
	t_3di	nm;
	double	eps;
	double 	ratio;

	a = v_id2v(p, d->pos);
	b = v_id2v(p, lght.o);
	nm = v_id2v(d->shps[num].o, p);
	// eps = (v_imodsq(b) * v_imodsq(nm) > 0.001) ? lght.l / 2 * sqrt(1 - SQ(v_iscal(b, nm)) / v_imodsq(b) / v_imodsq(nm)) : lght.l / 2; 
	eps = lght.l;
	// if (ABS(ABS(v_iscal(a, nm) / v_imod(a)) - ABS(v_iscal(b, nm) / v_imod(b))) < eps &&
	// 	ABS(v_iscal(v_ivop(a, b, '*'), nm) / v_imod(v_ivop(a, b, '*'))) < 2 * eps)
	c = v_ivop(v_isop(a, 1, '*'), v_isop(b, v_imod(a) / v_imod(b), '*'), '+');
	// printf("%d %d %d |", c.x , c.y, c.z);
	ratio = (v_imodsq(c) * v_imodsq(nm) > 0.001) ? v_iscal(c, nm) / v_imod(c) / v_imod(nm) : 0;
	if (ratio > 0.95)
	{
		// printf(" %.f ", ratio);
		// ratio = 1;
		eps = (v_imodsq(b) * v_imodsq(nm) > 0.001) ? v_iscal(b, nm) / v_imod(b) / v_imod(nm) : 0;
		ratio = 2.0 * eps / (eps + sqrt(SQ(d->shps[num].n) - 1 + SQ(eps))) - 1;
		eps = 2.0 * SQ(d->shps[num].n) * eps / (eps + sqrt(SQ(d->shps[num].n) - 1 + SQ(eps))) - 1;
		// ratio = 1;
		// eps = 1;
		ratio = exp((v_iscal(c, nm) / v_imod(c) / v_imod(nm) - 1.0) / 0.03) / (4 * M_PI * 0.03) * lght.I * (SQ(ratio) + SQ(eps)) / 2.0 * ((v_imodsq(b) > 0) ? (lght.l * lght.l) / v_imodsq(b) : 1);
		// ratio = (SQ(2.0 * eps / (eps + sqrt(SQ(d->shps[num].n) - 1 + SQ(eps))) - 1) +
			// SQ(2.0 * SQ(d->shps[num].n) * eps / (eps + sqrt(SQ(d->shps[num].n) - 1 + SQ(eps))) - 1)) / 2.0;
		// b = v_isop(lght.spctr, (ratio * lght.I), '*');
		// printf("%.2f %d %d %d |", ratio, b.x , b.y, b.z);
		// printf(" %.f |", ratio * lght.I);
		return (v_isop(lght.spctr, (ratio), '*'));
	}
	// else
			// printf("a %.f a", ABS(ABS(v_iscal(v_id2v(p, d->pos), nm)) - ABS(v_iscal(b, nm))));
	return (v_isop(p, 0, '='));
}


void	raytrace(t_data *d)
{
	t_3di	p1;
	t_3di	p;
	int		n;
	int 	i;
	t_3d	b;
	t_3d	ox;
	t_3d	oy;
	t_3di	col;
	t_3di	colinc;
	t_3di	colgl;
	// t_3di 	tmp;

	p1.y = -1;
	b = v_i2d(v_id2v(d->pos, d->vwp));
	if (b.z != -1 && v_dmodsq(b) != 0)
	{
		b = v_dsop(b, 1 / sqrt(v_dmodsq(b)), '*');
		ox.x = (1.0 - SQ(b.x) / (1.0 + b.z));
		ox.y = -(b.y * b.x) / (1.0 + b.z);
		ox.z = -b.x;
		oy.x = -(b.y * b.x) / (1.0 + b.z);
		oy.y = 1.0 - SQ(b.y) / (1.0 + b.z);
		oy.z = -b.y;
	}
	else
	{
		ox.x = -1;
		ox.y = 0;
		ox.z = 0;
		oy.x = 0;
		oy.y = 1;
		oy.z = 0;
	}
	// printf("\n\n\n");
	while (++p1.y < YS && (p1.x = -1))
		while (++p1.x < XS && (i = -1))
		{
			// printf("\n\n");
			col = v_isop(col, 0, '=');
			b = v_dvop(v_dsop(ox, (p1.x - XS / 2), '*'), v_dsop(oy, (p1.y - YS / 2), '*'), '+');
			// tmp = v_ivop(d->vwp, v_d2i(b), '+');
			// if (v_iscal(v_id2v(d->pos, d->shps[0].o), v_id2v(d->pos, tmp)) < 0)
			// 	printf(" %d %d %d |", tmp.x, tmp.y, tmp.z);
			p = cast_primray(d, v_ivop(d->vwp, v_d2i(b), '+'), &n);
			if (n >= 0)
				while (++i < d->nlght)
				{
					colgl = v_isop(colgl, 0, '=');
					// printf("\n\n");
					// printf("sh %d lt %d |", n, i);
					// colinc.x = 0;
					colinc = cast_shadray(d, p, d->lght[i], 1);
					if (d->shps[n].n > 1.01 && v_imodsq(colinc) > 1 && i > 0)
						colgl = add_gloss(d, p, d->lght[i], n);
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
					// printf(" %.f, %.f, %.f |", col.x - colinc.x * (1 - d->shps[n].mu.x), col.y - colinc.y * (1 - d->shps[n].mu.y), col.z - colinc.z * (1 - d->shps[n].mu.z));
					// printf(" %.f, %.f, %.f |", (1 - d->shps[n].mu.x), (1 - d->shps[n].mu.y), (1 - d->shps[n].mu.z));
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
