/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfilipch <mfilipch@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/15 22:00:05 by mfilipch          #+#    #+#             */
/*   Updated: 2016/10/15 22:00:06 by mfilipch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytracer.h"

void	display_controls(void)
{
	ft_putstr("\
____________________________________\n\
CONTROLS:\n\
Move camera:\n\
	Forward, Back: 4, 1\n\
	Up, down: Key: UP, DOWN\n\
	Left, Right: Key: LEFT, RIGHT\n\n\
Rotate camera:\n\
	OY axis: 5,2\n\
	OX axis: 6,3\n\
____________________________________\n");
}

void	view_setup(t_data *d)
{
	t_3d	base;

	base = v_i2d(v_id2v(d->pos, d->vwp));
	d->length = v_dmodsq(base);
	if (base.x <= base.z)
	{
		d->ox.x = 1;
		d->ox.y = 0;
		d->ox.z = 0;
	}
	else
	{
		d->ox.x = 0;
		d->ox.y = 0;
		d->ox.z = -1;
	}
	base = v_dsop(base, 1 / sqrt(d->length), '*');
	d->oy = v_dvop(base, d->ox, '*');
	d->ox = v_dvop(d->oy, base, '*');
	d->ox = v_dsop(d->ox, 1 / v_dmod(d->ox), '*');
	d->oy = v_dsop(d->oy, 1 / v_dmod(d->oy), '*');
}
