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
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>

int		ft_displayit(t_data *d)
{
	mlx_expose_hook(d->win, ft_drawit, d);
	mlx_key_hook(d->win, ft_key_hook, d);
	mlx_hook(d->win, 17, 1, ft_close, d);
	// mlx_hook(d->win, 4, 1, ft_mouse_down, d);
	// mlx_hook(d->win, 6, 1, ft_mouse_move, d);
	// mlx_hook(d->win, 5, 1, ft_mouse_up, d);
	ft_drawit(d);
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
	d->oz.x = 0;
	d->oz.y = 0;
	d->phi = 0;
	d->teta = 0;	
	d->param = 0;
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
	// printf("depth %d\n", d->depth);
	// printf("pos.x %d\n", d->pos.x);
	// printf("pos.y %d\n", d->pos.y);
	// printf("pos.z %d\n", d->pos.z);
	// printf("vwp.x %d\n", d->vwp.x);
	// printf("vwp.y %d\n", d->vwp.y);
	// printf("vwp.z %d\n", d->vwp.z);
	// printf("max_dist %.2f\n", d->max_dist);
	// int i;
	// i = -1;
	// while (++i < d->nshp)
	// {
	// 	printf("\nShape %d\n", i);
	// 	printf("id %d\n", d->shps[i].id);
	// 	printf("o.x %d\n", d->shps[i].o.x);
	// 	printf("o.y %d\n", d->shps[i].o.y);
	// 	printf("o.z %d\n", d->shps[i].o.z);
	// 	printf("l %d\n", d->shps[i].l);
	// 	printf("h %d\n", d->shps[i].h);
	// 	printf("mu.x %.2f\n", d->shps[i].mu.x);
	// 	printf("mu.y %.2f\n", d->shps[i].mu.y);
	// 	printf("mu.z %.2f\n", d->shps[i].mu.z);
	// 	printf("nm.x %d\n", d->shps[i].nm.x);
	// 	printf("nm.y %d\n", d->shps[i].nm.y);
	// 	printf("nm.z %d\n", d->shps[i].nm.z);
	// 	printf("n %.2f\n", d->shps[i].n);
	// }
	// i = -1;
	// while (++i < d->nlght)
	// {
	// 	printf("\nLight %d\n", i);
	// 	printf("o.x %d\n", d->lght[i].o.x);
	// 	printf("o.y %d\n", d->lght[i].o.y);
	// 	printf("o.z %d\n", d->lght[i].o.z);
	// 	printf("spctr.x %d\n", d->lght[i].spctr.x);
	// 	printf("spctr.y %d\n", d->lght[i].spctr.y);
	// 	printf("spctr.z %d\n", d->lght[i].spctr.z);
	// 	printf("l %d\n", d->lght[i].l);
	// 	printf("I %d\n", d->lght[i].I);
	// }
	//config
	// {
		// int sh;
		// sh = 0;
		// d->depth = 1;
		// d->pos.x = (XS) / 2;
		// d->pos.y = YS / 2;
		// d->pos.z = -YS;
		// d->vwp.x = XS / 2;
		// d->vwp.y = YS / 2;
		// d->vwp.z = 0;
		// d->max_dist = 100000;
		// d->nshp = 4 - sh;
		// d->nlght = 2;
		// d->shps = (t_shp*)malloc(sizeof(t_shp) * d->nshp);
		// d->lght = (t_lght*)malloc(sizeof(t_lght) * d->nlght);
	// //make sphere

		// d->shps[0 - sh].id = 1;
		// d->shps[0 - sh].o.x = XS / 6;
		// d->shps[0 - sh].o.y = (YS * 1) / 2;
		// d->shps[0 - sh].o.z = (YS * 2) / 3;
		// d->shps[0 - sh].l = YS / 6;
		// d->shps[0 - sh].mu.x = 1;
		// d->shps[0 - sh].mu.y = 0.2;
		// d->shps[0 - sh].mu.z = 0.2;	
		// d->shps[0 - sh].n = 1.5;
		// d->shps[1 - sh].id = 1;
		// d->shps[1 - sh].o.x = (2 * XS) / 5;
		// d->shps[1 - sh].o.y = (YS * 1) / 2;
		// d->shps[1 - sh].o.z = (YS * 2) / 3;
		// d->shps[1 - sh].l = YS / 20;
		// d->shps[1 - sh].mu.x = 0.2;
		// d->shps[1 - sh].mu.y = 0.2;
		// d->shps[1 - sh].mu.z = 1;
		// d->shps[1 - sh].n = 1.5;
		// d->shps[2 - sh].id = 0;
		// d->shps[2 - sh].o.x = XS / 2;
		// d->shps[2 - sh].o.y = (YS * 2) / 3;
		// d->shps[2 - sh].o.z = (YS * 2) / 3;
		// d->shps[2 - sh].mu.x = 0.2;
		// d->shps[2 - sh].mu.y = 1;
		// d->shps[2 - sh].mu.z = 0.2;
		// d->shps[2 - sh].nm.x = d->shps[2 - sh].o.x;
		// d->shps[2 - sh].nm.y = d->shps[2 - sh].o.y - YS / 2;
		// d->shps[2 - sh].nm.z = d->shps[2 - sh].o.z;
		// d->shps[2 - sh].l = 2 * YS;
		// d->shps[2 - sh].n = 1;
		// d->shps[3 - sh].id = 0;
		// d->shps[3 - sh].o.x = XS / 2;
		// d->shps[3 - sh].o.y = YS / 2;
		// d->shps[3 - sh].o.z = (YS);
		// d->shps[3 - sh].mu.x = 0.2;
		// d->shps[3 - sh].mu.y = 1;
		// d->shps[3 - sh].mu.z = 0.2;
		// d->shps[3 - sh].nm.x = d->shps[3 - sh].o.x;
		// d->shps[3 - sh].nm.y = d->shps[3 - sh].o.y;
		// d->shps[3 - sh].nm.z = d->shps[3 - sh].o.z - YS / 2;
		// d->shps[3 - sh].l = 2 * YS;
		// d->shps[3 - sh].n = 1;
		// d->shps[4 - sh].id = 2;
		// d->shps[4 - sh].o.x = XS / 6;
		// d->shps[4 - sh].o.y = (YS * 3) / 4;
		// d->shps[4 - sh].o.z = (YS * 3) / 4;
		// d->shps[4 - sh].l = XS / 6;
		// d->shps[4 - sh].mu.x = 1;
		// d->shps[4 - sh].mu.y = 0.2;
		// d->shps[4 - sh].mu.z = 0.2;	
		// d->shps[4 - sh].n = 1.5;
		// d->shps[4 - sh].nm.x = d->shps[4 - sh].o.x - XS / 20;
		// d->shps[4 - sh].nm.y = d->shps[4 - sh].o.y - YS / 4;
		// d->shps[4 - sh].nm.z = d->shps[4 - sh].o.z;
		// d->shps[5 - sh].id = 3;
		// d->shps[5 - sh].o.x = XS / 6;
		// d->shps[5 - sh].o.y = (YS * 1) / 2;
		// d->shps[5 - sh].o.z = (YS * 3) / 4;
		// d->shps[5 - sh].l = XS / 12;
		// d->shps[5 - sh].h = XS / 6;
		// d->shps[5 - sh].mu.x = 1;
		// d->shps[5 - sh].mu.y = 0.2;
		// d->shps[5 - sh].mu.z = 0.2;	
		// d->shps[5 - sh].n = 1.5;
		// d->shps[5 - sh].nm.x = d->shps[5 - sh].o.x;
		// d->shps[5 - sh].nm.y = d->shps[5 - sh].o.y - YS / 4;
		// d->shps[5 - sh].nm.z = d->shps[5 - sh].o.z;					
	// //make light
		// d->lght[0].o.x = XS / 2;
		// d->lght[0].o.y = YS / 2;
		// d->lght[0].o.z = -YS;
		// d->lght[0].spctr.x = 0;
		// d->lght[0].spctr.y = 0;
		// d->lght[0].spctr.z = 0;
		// d->lght[0].l = 10;
		// d->lght[0].I = 2;
		// d->lght[1].o.x = (XS * 4) / 6;
		// d->lght[1].o.y = (YS * 1) / 2;
		// d->lght[1].o.z = (YS * 2) / 3;
		// d->lght[1].spctr.x = 255;
		// d->lght[1].spctr.y = 255;
		// d->lght[1].spctr.z = 255;
		// d->lght[1].l = 10;
		// d->lght[1].I = 2000;
		// d->lght[2].o.x = (XS * 1) / 12;
		// d->lght[2].o.y = (YS * 1) / 6;
		// d->lght[2].o.z = (YS * 2) / 3;
		// d->lght[2].spctr.x = 0;
		// d->lght[2].spctr.y = 0;
		// d->lght[2].spctr.z = 0;
		// d->lght[2].l = 10;
		// d->lght[2].I = 700;
	// }
	// display_controls();
	mlx_loop_hook(d->mlx, ft_displayit, d);
	mlx_loop(d->mlx);
	return (0);
}
