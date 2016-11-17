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
	(keycode == KEY_ESC) ? ft_free_n_exit(d, NULL, NULL, 0) : 0;
	(keycode == KEY_UP) ? d->pos.y -= 50 : 0;
	(keycode == KEY_DOWN) ? d->pos.y += 50 : 0;
	(keycode == KEY_LEFT) ? d->pos.x -= 50 : 0;
	(keycode == KEY_RIGHT) ? d->pos.x += 50 : 0;
	(keycode == KEY_4) ? d->pos.z += 50 : 0;
	(keycode == KEY_1) ? d->pos.z -= 50 : 0;
	(keycode == KEY_7) ? d->phi += 0.05 : 0;
	(keycode == KEY_8) ? d->phi -= 0.05 : 0;
	(keycode == KEY_5) ? d->teta -= 0.05 : 0;
	(keycode == KEY_2) ? d->teta += 0.05 : 0;
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
		d->oz.y = 0;
	return (0);
}
