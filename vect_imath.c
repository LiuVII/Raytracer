/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect_imath.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfilipch <mfilipch@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/07 13:07:15 by mfilipch          #+#    #+#             */
/*   Updated: 2016/11/07 13:07:16 by mfilipch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

double	v_imodsq(t_3di p)
{
	return(p.x * p.x + p.y * p.y + p.z * p.z);
}

double	v_imod(t_3di p)
{
	return(sqrt(p.x * p.x + p.y * p.y + p.z * p.z));
}

t_3di	v_isop(t_3di p1, double x, char c)
{
	t_3di p;

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

t_3di	v_ivop(t_3di p1, t_3di p2, char c)
{
	t_3di p;

	p = v_isop(p1, 0, '=');
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

int		v_iscal(t_3di p1, t_3di p2)
{
	return (p1.x * p2.x + p1.y * p2.y + p1.z * p2.z);
}

t_3di	v_id2v(t_3di p1, t_3di p2)
{
	return(v_ivop(p2, p1, '-'));
}

t_3di	v_d2i(t_3d pd)
{
	t_3di pi;

	pi.x = pd.x;
	pi.y = pd.y;
	pi.z = pd.z;
	return (pi);
}
