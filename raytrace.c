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


t_3di	intsec_sphere(t_shp shp, t_3di a, t_3di b)
{
	double	ratio;

	ratio = 0;
	if (SQ(shp.l) > (ratio = v_imodsq(a) - SQ(v_iscal(a, b)) / v_imodsq(b)))
	{
		// printf(" %.f %.f %.2f |", v_iscal(a, b), v_imodsq(b), v_iscal(a, b) / v_imodsq(b));
		// if (v_iscal(a, b) < 0)
		// 	printf(" 1x %d 1y %d 1z %d, 2x %d, 2y %d, 2z %d\n", p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
			// printf(" ax %d ay %d az %d, bx %d, by %d, bz %d\n", a.x, a.y, a.z, b.x, b.y, b.z);
		ratio = (v_iscal(a, b) / v_imodsq(b) -
			sqrt((SQ(shp.l) - ratio) / v_imodsq(b)));
		if (ABS(ratio) < 0.1)
			ratio = 2 * v_iscal(a, b) / v_imodsq(b);
		// printf(" %.2f |", ratio);
		return (v_isop(b, ratio, '*'));
	}
	return (v_isop(b, 0, '*'));
}

t_3di	intsec_map(t_shp shp, t_3di a, t_3di b)
{
	double	ratio;
	t_3di	nm;

	ratio = 0;
	nm = v_id2v(shp.o, shp.nm);
	if (ABS(v_iscal(b, nm)) > 0.01)
	{
		if ((ratio = (v_iscal(a, nm) / v_iscal(b, nm))) > 0)
			return (v_isop(b, ratio, '*'));
	}
	return (v_isop(b, 0, '*'));
}

t_3di	intsec_cylinder(t_shp shp, t_3d dp, t_3d v)
{
	double	ratio;
	t_3d	nm;
	double a;
	double b;
	double c;

	nm = v_i2d(v_id2v(shp.o, shp.nm));
	nm = v_dsop(nm, 1.0 / v_dmod(nm), '*');
	// printf(" x %d y %d z %d |", nm.x, nm.y, nm.z);
	b = 2 * v_dscal(v_dvop(v, v_dsop(nm, v_dscal(v, nm), '*'), '-'), v_dvop(v_dsop(nm, v_dscal(dp, nm), '*'), dp, '-'));
	a = v_dmodsq(v_dvop(v, v_dsop(nm, v_dscal(v, nm), '*'), '-'));
	c = v_dmodsq(v_dvop(v_dsop(nm, v_dscal(dp, nm), '*'), dp, '-')) - SQ(shp.l);
	ratio = 0;
	// printf(" %.2f %.2f %.2f |", a, b, c);
	if (a > 0.01 && (ratio = SQ(b) - 4 * a * c) > 0.001)
	{
		ratio = (-b + SIGN(b) * sqrt(ratio)) / 2.0 / a;
		if (ratio < 0 || v_imodsq(v_d2i(v_dsop(v, ratio, '*'))) < 1.1)
			ratio = (-b - SIGN(b) * sqrt(SQ(b) - 4 * a * c)) / 2.0 / a;
		ratio = (ratio > 0) ? ratio : 0;
	}
	return (v_d2i(v_dsop(v, ratio, '*')));
}

t_3di	intsec_cone(t_shp shp, t_3d dp, t_3d v)
{
	double	ratio;
	t_3d	nm;
	double a;
	double b;
	double c;

	nm = v_i2d(v_id2v(shp.o, shp.nm));
	nm = v_dsop(nm, 1.0 / v_dmod(nm), '*');
	// printf(" x %d y %d z %d |", nm.x, nm.y, nm.z);
	ratio = (double)SQ(shp.h) / (SQ(shp.h) + SQ(shp.l));
	// printf(" %.2f |", ratio);
	b = 2 * ratio * v_dscal(v_dvop(v, v_dsop(nm, v_dscal(v, nm), '*'), '-'), v_dvop(v_dsop(nm, v_dscal(dp, nm), '*'), dp, '-'))
	+ 2 * (1 - ratio) * v_dscal(v, nm) * v_dscal(dp, nm);
	a = ratio * v_dmodsq(v_dvop(v, v_dsop(nm, v_dscal(v, nm), '*'), '-')) - (1 - ratio) * SQ(v_dscal(v, nm));
	c = ratio * v_dmodsq(v_dvop(v_dsop(nm, v_dscal(dp, nm), '*'), dp, '-')) - (1 - ratio) * SQ(v_dscal(dp, nm));
	ratio = 0;
	// printf(" %.2f %.2f %.2f |", a, b, c);
	if (a > 0.01 && (ratio = SQ(b) - 4 * a * c) > 0.001)
	{
		// printf(" %.2f %.2f %.2f |", a, b, c);
		ratio = (-b + SIGN(b) * sqrt(ratio)) / 2.0 / a;
		if (ratio < 0 || v_imodsq(v_d2i(v_dsop(v, ratio, '*'))) < 1.1)
			ratio = (-b - SIGN(b) * sqrt(SQ(b) - 4 * a * c)) / 2.0 / a;
		ratio = (ratio > 0) ? ratio : 0;
	}
	return (v_d2i(v_dsop(v, ratio, '*')));
}


t_3di	intersect(t_data *d, t_3di p1, t_3di p2, int n)
{
	t_3di	p;
	t_3di	a;
	t_3di	b;

	a = v_id2v(p1, d->shps[n].o);
	b = v_id2v(p1, p2);
	// p = v_isop(p1, -1, '=');
	p = p1;
	if (d->shps[n].id == 1 && v_imodsq(b) > 0) 
		p = v_ivop(p1, intsec_sphere(d->shps[n], a, b), '+');         
	else if (d->shps[n].id == 2)
		p = v_ivop(p1, intsec_cylinder(d->shps[n], v_i2d(a), v_i2d(b)), '+'); 
	else if (d->shps[n].id == 3)
		p = v_ivop(p1, intsec_cone(d->shps[n], v_i2d(a), v_i2d(b)), '+'); 
	else if (d->shps[n].id == 0)
	{
		p = v_ivop(p1, intsec_map(d->shps[n], a, b), '+');
		if (/*v_imod(v_id2v(p, p1)) > d->shps[n].l || */ ABS(d->shps[n].o.x - p.x) > 0.1 * (d->max_dist) ||
		ABS(d->shps[n].o.y - p.y) > 0.1 * d->max_dist || ABS (d->shps[n].o.z - p.z) > 0.1 * (d->max_dist))
			p = p1;
		// else
		// 	printf(" %d %d %d |", p.x, p.y, p.z);
	}
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
	// !LIGHT INTENSITY!
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

t_3di	add_gloss(t_3di a, t_3di p, t_lght lght, t_shp shp)
{
	t_3di	c;
	t_3di	b;
	t_3di	nm;
	double	eps;
	double 	ratio;

	b = v_id2v(p, lght.o);
	if (shp.id == 1)
		nm = v_id2v(shp.o, p);
	else if (shp.id == 2)
	{
		nm = v_id2v(shp.o, shp.nm);
		// nm = v_dsop(v_i2d(nm), 1 / v_imod(nm), '*');
		nm = v_d2i(v_dvop(v_dsop(v_i2d(nm), 1 / v_imod(nm), '*'), v_dvop(v_i2d(v_id2v(shp.o, p)), v_dsop(v_i2d(nm), 1 / v_imod(nm), '*'), '*'), '*'));
		// printf(" %d %d %d |", nm.x , nm.y, nm.z);
	}
	else if (shp.id == 3)
	{
		ratio = SIGN(v_iscal(v_id2v(shp.o, shp.nm), v_id2v(p, shp.o)));
		nm = v_isop(v_id2v(shp.o, shp.nm), ratio, '*');
		// nm = v_dsop(v_i2d(nm), 1 / v_imod(nm), '*');
		nm = v_d2i(v_dvop(v_dsop(v_i2d(nm), 1 / v_imod(nm), '*'), v_dvop(v_i2d(v_id2v(shp.o, p)), v_dsop(v_i2d(nm), 1 / v_imod(nm), '*'), '*'), '*'));
		nm = v_ivop(v_isop(v_isop(v_id2v(shp.o, shp.nm), ratio, '*'), v_imod(nm) * shp.l / shp.h / v_imod(v_id2v(shp.o, shp.nm)), '*'), nm, '+');
		// printf(" %d %d %d |", nm.x , nm.y, nm.z);
		
	}
	else
		nm = v_id2v(shp.o, shp.nm);
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
		ratio = 2.0 * eps / (eps + sqrt(SQ(shp.n) - 1 + SQ(eps))) - 1;
		eps = 2.0 * SQ(shp.n) * eps / (eps + sqrt(SQ(shp.n) - 1 + SQ(eps))) - 1;
		// ratio = 1;
		// eps = 1;
		ratio = exp((v_iscal(c, nm) / v_imod(c) / v_imod(nm) - 1.0) / 0.03) /
		(4 * M_PI * 0.03) * lght.I * (SQ(ratio) + SQ(eps)) / 2.0 * ((v_imodsq(b) > 0) ? (lght.l * lght.l) / v_imodsq(b) : 1);
		// ratio = (SQ(2.0 * eps / (eps + sqrt(SQ(shp.n) - 1 + SQ(eps))) - 1) +
			// SQ(2.0 * SQ(shp.n) * eps / (eps + sqrt(SQ(shp.n) - 1 + SQ(eps))) - 1)) / 2.0;
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
