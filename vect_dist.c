/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect_dist.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfilipch <mfilipch@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/14 19:48:29 by mfilipch          #+#    #+#             */
/*   Updated: 2016/11/14 19:48:31 by mfilipch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

double	v_dmodsq(t_3d p)
{
	return (p.x * p.x + p.y * p.y + p.z * p.z);
}

double	v_dmod(t_3d p)
{
	return (sqrt(p.x * p.x + p.y * p.y + p.z * p.z));
}

double	v_imodsq(t_3di p)
{
	return (p.x * p.x + p.y * p.y + p.z * p.z);
}

double	v_imod(t_3di p)
{
	return (sqrt(p.x * p.x + p.y * p.y + p.z * p.z));
}
