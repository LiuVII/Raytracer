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

t_3d	intsec_sphere(t_shp shp, t_3d a, t_3d b)
{
	double	ratio;

	ratio = v_dscal(a, b);
	if (ratio >= 0 &&
		(SQ(shp.l) > (ratio = v_dmodsq(a) - SQ(ratio) / v_dmodsq(b))))
	{
		ratio = (v_dscal(a, b) / v_dmodsq(b) -
			sqrt((SQ(shp.l) - ratio) / v_dmodsq(b)));
		if (ABS(ratio) < 0.01)
			ratio = 2 * v_dscal(a, b) / v_dmodsq(b);
		return (v_dsop(b, ratio, '*'));
	}
	return (v_dsop(b, 0, '*'));
}

t_3d	intsec_map(t_shp shp, t_3d a, t_3d b)
{
	double	ratio;
	t_3d	nm;

	ratio = 0;
	nm = shp.nm;
	nm = v_dsop(nm, 1.0 / v_dmod(nm), '*');
	if (ABS(v_dscal(b, nm)) > 0.01)
	{
		if ((ratio = (v_dscal(a, nm) / v_dscal(b, nm))) > 0)
			return (v_dsop(b, ratio, '*'));
	}
	return (v_dsop(b, 0, '*'));
}

t_3d	intsec_cylinder(t_shp shp, t_3d dp, t_3d v)
{
	double	ratio;
	t_3d	nm;
	double	a;
	double	b;
	double	c;

	nm = shp.nm;
	nm = v_dsop(nm, 1.0 / v_dmod(nm), '*');
	b = 2 * v_dscal(v_dvop(v, v_dsop(nm, v_dscal(v, nm), '*'), '-'),
		v_dvop(v_dsop(nm, v_dscal(dp, nm), '*'), dp, '-'));
	a = v_dmodsq(v_dvop(v, v_dsop(nm, v_dscal(v, nm), '*'), '-'));
	c = v_dmodsq(v_dvop(v_dsop(nm, v_dscal(dp, nm), '*'), dp, '-')) - SQ(shp.l);
	ratio = 0;
	if (a > 0.01 && (ratio = SQ(b) - 4 * a * c) > 0.001)
	{
		ratio = (-b + SIGN(b) * sqrt(ratio)) / 2.0 / a;
		if ((ratio < 0 || v_imodsq(v_d2i(v_dsop(v, ratio, '*'))) < 1.1))
			ratio = (-b - SIGN(b) * sqrt(SQ(b) - 4 * a * c)) / 2.0 / a;
		ratio = (ratio < 0) ? 0 : cap_cylinder(shp, dp, v, ratio);
	}
	return ((v_dsop(v, ratio, '*')));
}

t_3d	intsec_cone(t_shp shp, t_3d dp, t_3d v)
{
	double	ratio;
	t_3d	nm;
	double	a;
	double	b;
	double	c;

	nm = shp.nm;
	nm = v_dsop(nm, 1.0 / v_dmod(nm), '*');
	ratio = (double)SQ(shp.h) / (SQ(shp.h) + SQ(shp.l));
	b = 2 * ratio * v_dscal(v_dvop(v, v_dsop(nm, v_dscal(v, nm), '*'), '-'),
		v_dvop(v_dsop(nm, v_dscal(dp, nm), '*'), dp, '-'))
	+ 2 * (1 - ratio) * v_dscal(v, nm) * v_dscal(dp, nm);
	a = ratio * v_dmodsq(v_dvop(v, v_dsop(nm, v_dscal(v, nm), '*'), '-')) -
	(1 - ratio) * SQ(v_dscal(v, nm));
	c = ratio * v_dmodsq(v_dvop(v_dsop(nm, v_dscal(dp, nm), '*'), dp, '-')) -
	(1 - ratio) * SQ(v_dscal(dp, nm));
	ratio = 0;
	if (a > 0.01 && (ratio = SQ(b) - 4 * a * c) > 0.001)
	{
		ratio = (-b + SIGN(b) * sqrt(ratio)) / 2.0 / a;
		if (ratio < 0 || v_imodsq(v_d2i(v_dsop(v, ratio, '*'))) < 1.1)
			ratio = (-b - SIGN(b) * sqrt(SQ(b) - 4 * a * c)) / 2.0 / a;
		ratio = (ratio < 0) ? 0 : cap_cone(shp, dp, v, ratio);
	}
	return ((v_dsop(v, ratio, '*')));
}

t_3d	intersect(t_data *d, t_3d p1, t_3d p2, int n)
{
	t_3d	p;
	t_3d	a;
	t_3d	b;

	a = v_dd2v(p1, d->shps[n].o);
	b = v_dd2v(p1, p2);
	p = p1;
	if (d->shps[n].id == 1 && v_dmodsq(b) > 0)
		p = v_dvop(p1, intsec_sphere(d->shps[n], a, b), '+');
	else if (d->shps[n].id == 2)
		p = v_dvop(p1, intsec_cylinder(d->shps[n], a, b), '+');
	else if (d->shps[n].id == 3)
		p = v_dvop(p1, intsec_cone(d->shps[n], a, b), '+');
	else if (d->shps[n].id == 0)
	{
		p = v_dvop(p1, intsec_map(d->shps[n], a, b), '+');
		if (ABS(d->shps[n].o.x - p.x) > 0.1 * (d->max_dist) ||
		ABS(d->shps[n].o.y - p.y) > 0.1 * d->max_dist ||
		ABS(d->shps[n].o.z - p.z) > 0.1 * (d->max_dist))
			p = p1;
	}
	return (p);
}
