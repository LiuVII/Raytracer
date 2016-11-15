/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect_dmath.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfilipch <mfilipch@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/07 22:52:17 by mfilipch          #+#    #+#             */
/*   Updated: 2016/11/07 22:52:19 by mfilipch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

t_3d	v_dsop(t_3d p1, double x, char c)
{
	t_3d	p;

	p.x = 0;
	p.y = 0;
	p.z = 0;
	if (c == '+')
	{
		p.x = p1.x + x;
		p.y = p1.y + x;
		p.z = p1.z + x;
	}
	else if (c == '*')
	{
		p.x = p1.x * x;
		p.y = p1.y * x;
		p.z = p1.z * x;
	}
	else if (c == '=')
	{
		p.x = x;
		p.y = x;
		p.z = x;
	}
	return (p);
}

t_3d	v_dvop(t_3d p1, t_3d p2, char c)
{
	t_3d	p;

	p = v_dsop(p1, 0, '=');
	if (c == '+')
	{
		p.x = p1.x + p2.x;
		p.y = p1.y + p2.y;
		p.z = p1.z + p2.z;
	}
	else if (c == '-')
	{
		p.x = p1.x - p2.x;
		p.y = p1.y - p2.y;
		p.z = p1.z - p2.z;
	}
	else if (c == '*')
	{
		p.x = p1.y * p2.z - p1.z * p2.y;
		p.y = p1.z * p2.x - p1.x * p2.z;
		p.z = p1.x * p2.y - p1.y * p2.x;
	}
	return (p);
}

double	v_dscal(t_3d p1, t_3d p2)
{
	return (p1.x * p2.x + p1.y * p2.y + p1.z * p2.z);
}

t_3d	v_dd2v(t_3d p1, t_3d p2)
{
	return (v_dvop(p2, p1, '-'));
}

t_3d	v_i2d(t_3di pi)
{
	t_3d	pd;

	pd.x = pi.x;
	pd.y = pi.y;
	pd.z = pi.z;
	return (pd);
}
