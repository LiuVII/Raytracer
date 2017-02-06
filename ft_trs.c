/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfilipch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/15 21:51:48 by mfilipch          #+#    #+#             */
/*   Updated: 2016/10/15 21:51:50 by mfilipch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytracer.h"

t_3d	ft_translate(t_3d p, t_3d tr)
{
	t_3d	np;

	np.x = p.x + tr.x;
	np.y = p.y + tr.y;
	np.z = p.z;
	return (np);
}

t_3d	ft_rotate_aphi(t_3d p, t_3d a, float phi)
{
	t_3d	np;
	float	scl;
	float	sq;
	float	rt;

	sq = v_dmod(a);
	rt = sqrt(sq);
	scl = (a.x * p.x + a.y * p.y + a.z * p.z) * ((1 - cos(phi))) / sq;
	np.x = a.x * scl + p.x * cos(phi) + (a.y * p.z - a.z * p.y) * sin(phi) / rt;
	np.y = a.y * scl + p.y * cos(phi) + (a.z * p.x - a.x * p.z) * sin(phi) / rt;
	np.z = a.z * scl + p.z * cos(phi) + (a.x * p.y - a.y * p.x) * sin(phi) / rt;
	return (np);
}

t_3d	ft_zoom(t_3d p, float zoom, t_2d oz)
{
	t_3d	np;

	oz.x = oz.x;
	np.x = p.x * zoom;
	np.y = p.y * zoom;
	np.z = p.z;
	return (np);
}

t_3d	ft_scale(t_3d p, t_3d scale)
{
	t_3d	np;

	np.x = p.x * scale.x;
	np.y = p.y * scale.y;
	np.z = p.z * scale.z;
	return (np);
}

t_3d	ft_tr(t_data *d, t_3d p)
{
	t_3d	np;
	float	ang;

	ang = sqrt(d->length / v_dmodsq(p));
	p.x *= ang;
	p.y *= ang;
	p.z *= ang;
	ang = d->psi;
	np = p;
	d->psi = 0;
	if (d->phi)
	{
		ang = d->phi;
		np = d->ox;
		d->phi = 0;
	}
	else if (d->teta)
	{
		ang = d->teta;
		np = d->oy;
		d->teta = 0;
	}
	d->ox = ft_rotate_aphi(d->ox, np, ang);
	d->oy = ft_rotate_aphi(d->oy, np, ang);
	return (ft_rotate_aphi(p, np, ang));
}
