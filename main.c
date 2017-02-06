/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfilipch <mfilipch@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/13 14:48:15 by mfilipch          #+#    #+#             */
/*   Updated: 2016/10/13 14:48:18 by mfilipch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytracer.h"

int		ft_displayit(t_data *d)
{
	mlx_key_hook(d->win, ft_key_hook, d);
	mlx_hook(d->win, 17, 1, ft_close, d);
	mlx_clear_window(d->mlx, d->win);
	mlx_put_image_to_window(d->mlx, d->win, d->img_p, 0, 0);
	return (0);
}

void	ft_puterr_msg(int err)
{
	if (err == -1)
		ft_putendl("usage: ./rtv1 $(config file)");
	else if (err == -2)
		ft_putendl("error: mlx initializing failed");
	else if (err == -3)
		ft_putendl("error: window initializing failed");
	else if (err == -4)
		ft_putendl("error: file opening/reading failed");
	else if (err == -5)
		ft_putendl("error: memory allocation failed");
	else if (err == -10)
		ft_putendl("error: invalid config file");
	else
		ft_putendl("error: unknown");
}

void	ft_free_n_exit(t_data *d, t_list **img_l, char *line, int err)
{
	int i;
	int j;

	(d && d->img_p) ? mlx_destroy_image(d->mlx, d->img_p) : 0;
	(d && d->pth) ? free(d->pth) : 0;
	(d && d->mlx && d->win) ? mlx_destroy_window(d->mlx, d->win) : 0;
	(d && d->shps) ? free(d->shps) : 0;
	(d && d->lght) ? free(d->lght) : 0;
	i = -1;
	if (d && d->img)
	{
		while (++i < d->img_size.z && (j = -1))
		{
			while (++j < d->img_size.y)
				free(d->img[i][j]);
			free(d->img[i]);
		}
		free(d->img);
	}
	(d && d->mlx) ? free(d->mlx) : 0;
	(d) ? free(d) : 0;
	(img_l && *img_l) ? ft_lstclr(img_l) : 0;
	(line) ? free(line) : 0;
	(err < 0) ? ft_puterr_msg(err) : 0;
	(err >= 0) ? exit(0) : exit(1);
}

void	data_init(t_data *d)
{
	d->img = NULL;
	d->img_p = NULL;
	d->phi = 0;
	d->teta = 0;
	d->psi = 0;
	d->param = 0;
	d->ox.x = 1;
	d->ox.y = 0;
	d->ox.z = 0;
	d->oy.x = 0;
	d->oy.y = 1;
	d->oy.z = 0;
	d->pth = (pthread_t*)malloc(sizeof(pthread_t) * THRD_N);
}

int		main(int argc, char **argv)
{
	t_data *d;

	if (!(argc > 1 && argv[1]))
	{
		ft_puterr_msg(-1);
		return (1);
	}
	if (!(d = (t_data*)malloc(sizeof(t_data))))
	{
		ft_puterr_msg(-5);
		return (1);
	}
	if (!(d->mlx = mlx_init()))
		ft_free_n_exit(d, NULL, NULL, -2);
	if (!(d->win = mlx_new_window(d->mlx, XS, YS, "RTv1")))
		ft_free_n_exit(d, NULL, NULL, -3);
	data_init(d);
	ft_read(argv[1], d);
	d->length = v_dmodsq(v_i2d(v_id2v(d->pos, d->vwp)));
	display_controls();
	raytrace(d);
	mlx_expose_hook(d->win, ft_displayit, d);
	mlx_loop_hook(d->mlx, ft_displayit, d);
	mlx_loop(d->mlx);
	return (0);
}
