/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfilipch <mfilipch@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/18 22:56:04 by mfilipch          #+#    #+#             */
/*   Updated: 2016/10/18 22:56:06 by mfilipch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytracer.h"

void	draw_pixel(t_data *d, int x, int y, int c)
{
	int		i;
	long	mem;

	if (x < 0 || y < 0 || x > XS - 10 || y > YS - 10)
		return ;
	i = -1;
	mem = (d->bpp >> 3) * x + d->ls * y;
	while (i++ < (d->bpp >> 3) - 1)
	{
		d->img_p0[mem + i] = c % 256;
		c = c >> 8;
	}
}

int		get_col_im(int x, int y, t_img img, double shade)
{
	int		c;
	int		i;
	long	mem;

	if (x < 0 || y < 0 || x > XS - 10 || y > YS - 10)
		return (0);
	c = 0;
	i = (img.bpp >> 3) - 1;
	mem = (img.bpp >> 3) * x + img.ls * y;
	while (i--)
	{
		c = c << 8;
		c += (unsigned char)(img.inf[mem + i]) * (1 - shade);
	}
	return (c);
}
