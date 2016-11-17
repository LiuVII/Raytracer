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

t_3d	ft_rotate(t_3d p, float phi, float teta, float psi)
{
	t_3d	np;

	np.x = cos(psi) * (cos(teta) * p.x - sin(teta) * (sin(phi) * p.y +
		cos(phi) * p.z)) - sin(psi) * (cos(phi) * p.y - sin(phi) * p.z);
	np.y = sin(psi) * (cos(teta) * p.x - sin(teta) * (sin(phi) * p.y +
		cos(phi) * p.z)) + cos(psi) * (cos(phi) * p.y - sin(phi) * p.z);
	np.z = sin(teta) * p.x + cos(teta) * (sin(phi) * p.y + cos(phi) * p.z);
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

	np = ft_rotate(p, d->phi, d->teta, d->psi);
	return (np);
}
