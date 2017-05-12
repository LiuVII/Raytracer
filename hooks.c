/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfilipch <mfilipch@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/15 22:00:05 by mfilipch          #+#    #+#             */
/*   Updated: 2016/10/15 22:00:06 by mfilipch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytracer.h"

int	ft_close(t_data *d)
{
	ft_free_n_exit(d, NULL, NULL, 0);
	return (0);
}

int	ft_key_hook(int keycode, t_data *d)
{
	t_3d	tmp;

	tmp = v_dsop(d->pos, 0, '=');
	(keycode == KEY_ESC) ? ft_free_n_exit(d, NULL, NULL, 0) : 0;
	(keycode == KEY_UP) ? tmp = v_dsop(d->oy, -100, '*') : tmp;
	(keycode == KEY_DOWN) ? tmp = v_dsop(d->oy, 100, '*') : tmp;
	(keycode == KEY_LEFT) ? tmp = v_dsop(d->ox, -100, '*') : tmp;
	(keycode == KEY_RIGHT) ? tmp = v_dsop(d->ox, 100, '*') : tmp;
	(keycode == KEY_4) ? tmp = v_dsop(v_dd2v(d->pos, d->vwp),
		100 / sqrt(d->length), '*') : tmp;
	(keycode == KEY_1) ? tmp = v_dsop(v_dd2v(d->pos, d->vwp),
		-100 / sqrt(d->length), '*') : tmp;
	(keycode == KEY_6) ? d->phi = 0.1 : 0;
	(keycode == KEY_3) ? d->phi = -0.1 : 0;
	(keycode == KEY_5) ? d->teta = -0.1 : 0;
	(keycode == KEY_2) ? d->teta = 0.1 : 0;
	if (v_dmodsq(tmp) != 0)
	{
		d->pos = v_dvop(d->pos, tmp, '+');
		d->vwp = v_dvop(d->vwp, tmp, '+');
	}
	(d->phi || d->teta || d->psi) ? d->vwp = v_dvop(d->pos,
		ft_tr(d, v_dd2v(d->pos, d->vwp)), '+') : d->vwp;
	raytrace(d);
	return (0);
}

int	ft_mouse_down(int button, int x, int y, t_data *d)
{
	if (x >= 0 && x <= XS && y >= 0 && YS <= YS)
		d->mevent = button;
	return (0);
}

int	ft_mouse_up(int button, int x, int y, t_data *d)
{
	if (button)
		d->mevent = 0 * (x - y);
	if (button == 1 || button == 2)
		x = 0;
	return (0);
}
