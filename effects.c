/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   effects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfilipch <mfilipch@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/05 00:26:57 by mfilipch          #+#    #+#             */
/*   Updated: 2016/11/05 00:26:59 by mfilipch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytracer.h"

double	cap_cylinder(t_shp shp, t_3d dp, t_3d v, double ratio)
{
	t_3d	nm;
	double	a;
	double	b;
	double	c;

	nm = shp.nm;
	nm = v_dsop(nm, 1.0 / v_dmod(nm), '*');
	b = -v_dscal(dp, nm);
	a = v_dscal(v_dsop(v, ratio, '*'), nm) + b;
	c = 0;
	if (ABS(v_dscal(v, nm)) > 0.01)
	{
		if (b > shp.h || a > shp.h)
			dp = v_dvop(dp, v_dsop(nm, shp.h, '*'), '+');
		c = v_dscal(dp, nm) / v_dscal(v, nm);
		if (v_dmodsq(v_dvop(v_dsop(v, c, '*'), dp, '-')) > shp.l * shp.l)
			c = 0;
	}
	if ((a < 0 || a > shp.h) || (ABS(c) > 0.01 && ABS(c) < ABS(ratio)))
		return (c);
	return (ratio);
}

double	cap_cone(t_shp shp, t_3d dp, t_3d v, double ratio)
{
	t_3d	nm;
	double	a;
	double	b;
	double	c;

	nm = shp.nm;
	nm = v_dsop(nm, 1.0 / v_dmod(nm), '*');
	b = v_dscal(dp, nm);
	a = b - v_dscal(v_dsop(v, ratio, '*'), nm);
	c = 0;
	if ((b > shp.h || a > shp.h) && ABS(v_dscal(v, nm)) > 0.01)
	{
		dp = v_dvop(dp, v_dsop(nm, shp.h, '*'), '-');
		c = v_dscal(dp, nm) / v_dscal(v, nm);
		if (v_dmodsq(v_dvop(v_dsop(v, c, '*'), dp, '-')) > shp.l * shp.l)
			c = 0;
	}
	if ((a < 0 || a > shp.h) || (ABS(c) > 0.01 && ABS(c) < ABS(ratio)))
		return (c);
	return (ratio);
}

t_3d	get_normal(t_shp shp, t_3d p)
{
	t_3d	nm;
	t_3d	nms;
	t_3d	tmp;

	if (shp.id == 1)
		nm = v_dd2v(shp.o, p);
	else if (shp.id == 2)
	{
		tmp = v_dsop(shp.nm, 1 / v_dmod(shp.nm), '*');
		nm = v_dvop(tmp, v_dvop(v_dd2v(shp.o, p), tmp, '*'), '*');
	}
	else if (shp.id == 3)
	{
		nms = v_dsop(shp.nm, SIGN(v_dscal(shp.nm, v_dd2v(p, shp.o))), '*');
		tmp = v_dsop(nms, 1 / v_dmod(nms), '*');
		nm = v_dvop(tmp, v_dvop(v_dd2v(shp.o, p), tmp, '*'), '*');
		nm = v_dvop(v_dsop(nms, v_dmod(nm) * shp.l / shp.h
			/ v_dmod(shp.nm), '*'), nm, '+');
	}
	else
		nm = shp.nm;
	return (nm);
}

t_3d	add_gloss(t_3d a, t_3d p, t_lght lght, t_shp shp)
{
	t_3d	c;
	t_3d	b;
	t_3d	nm;
	double	eps;
	double	ratio;

	b = v_dd2v(p, lght.o);
	nm = get_normal(shp, p);
	eps = lght.l;
	c = v_dvop(v_dsop(a, 1, '*'), v_dsop(b, v_dmod(a) / v_dmod(b), '*'), '+');
	ratio = (v_dmodsq(c) * v_dmodsq(nm) > 0.001) ? v_dscal(c, nm)
	/ v_dmod(c) / v_dmod(nm) : 0;
	if (ratio > 0.95)
	{
		eps = (v_dmodsq(b) * v_dmodsq(nm) > 0.001) ? v_dscal(b, nm)
		/ v_dmod(b) / v_dmod(nm) : 0;
		ratio = 2.0 * eps / (eps + sqrt(SQ(shp.n) - 1 + SQ(eps))) - 1;
		eps = 2.0 * SQ(shp.n) * eps / (eps + sqrt(SQ(shp.n) - 1 + SQ(eps))) - 1;
		ratio = exp((v_dscal(c, nm) / v_dmod(c) / v_dmod(nm) - 1.0) / 0.03) /
		(4 * M_PI * 0.03) * lght.in * (SQ(ratio) + SQ(eps)) / 2.0
		* ((v_dmodsq(b) > 0) ? (lght.l * lght.l) / v_dmodsq(b) : 1);
		return (v_dsop(lght.spctr, (ratio), '*'));
	}
	return (v_dsop(p, 0, '='));
}
