/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfilipch <mfilipch@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/14 16:05:38 by mfilipch          #+#    #+#             */
/*   Updated: 2016/11/14 16:05:42 by mfilipch         ###   ########.fr       */
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
	// nm = v_id2v(shp.o, shp.nm);
	nm = shp.nm;
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

	nm = v_i2d(shp.nm);
	// nm = v_i2d(v_id2v(shp.o, shp.nm));
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

	nm = v_i2d(shp.nm);
	// nm = v_i2d(v_id2v(shp.o, shp.nm));
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
