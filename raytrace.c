/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytrace.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfilipch <mfilipch@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/05 00:26:57 by mfilipch          #+#    #+#             */
/*   Updated: 2016/11/05 00:26:59 by mfilipch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytracer.h"

/*
** for each shape check if intersects path of light, return 0 color if any does
** depending on the distance to the light calcualte light intensity
** time measurement:
** #include <time.h>
** #include <sys/time.h>
*/

t_3di	cast_shadray(t_data *d, t_3d p1, t_lght lght, int depth)
{
	int		i;
	double	ratio;
	t_3d	p;

	i = -1;
	while (++i < d->nshp)
	{
		p = intersect(d, p1, v_i2d(lght.o), i);
		if (v_dscal(v_dd2v(p1, p), v_dd2v(p1, v_i2d(lght.o))) > 0.1 &&
			v_dmodsq(v_dd2v(p1, p)) <= v_dmodsq(v_dd2v(p1, v_i2d(lght.o))))
		{
			if (depth == 1)
				return (v_isop(lght.spctr, 0, '='));
		}
	}
	if (v_dmodsq(v_dd2v(p1, v_i2d(lght.o))) > 0 && (ratio = (lght.l * lght.l)
		/ v_dmodsq(v_dd2v(p1, v_i2d(lght.o)))))
	{
		return (v_isop(lght.spctr, (ratio * lght.in), '*'));
	}
	return (lght.spctr);
}

/*
** For each shape check if in the way of the ray, pick the closest one
** Return the intersection point and shape number
*/

t_3d	cast_primray(t_data *d, t_3d p2, int *n)
{
	int		i;
	double	min_dist;
	double	dist;
	t_3d	p;
	t_3d	pi;

	i = -1;
	*n = -1;
	pi = v_i2d(d->pos);
	min_dist = d->max_dist;
	while (++i < d->nshp)
	{
		p = (intersect(d, v_i2d(d->pos), (p2), i));
		if (v_dmodsq(v_dd2v(v_i2d(d->pos), p)) > 0.1)
		{
			if ((dist = sqrt(v_dmodsq(v_dd2v(v_i2d(d->pos), p)))) < min_dist)
			{
				min_dist = dist;
				*n = i;
				pi = p;
			}
		}
	}
	return (pi);
}

/*
** Trace primary ray to the first object
** If intersected, see if there's any objects in between for each light
** Add gloss effect
** Mix clolors: color after shape absorption + gloss color
*/

t_3di	trace_one(t_data *d, t_3d p, int i, int n)
{
	t_3di	col;
	t_3di	colinc;
	t_3di	colgl;

	col = v_isop(v_d2i(p), 0, '=');
	p = cast_primray(d, v_dvop(v_i2d(d->vwp), p, '+'), &n);
	if (n >= 0)
		while (++i < d->nlght)
		{
			colgl = v_isop(colgl, 0, '=');
			colinc = cast_shadray(d, (p), d->lght[i], 1);
			if (d->shps[n].n > 1.01 && v_imodsq(colinc) > 1 && i > 0)
				colgl = add_gloss(v_id2v(v_d2i(p), d->pos),
					v_d2i(p), d->lght[i], d->shps[n]);
			col = v_ivop(col, colgl, '+');
			col.x = col.x + colinc.x * (1 - d->shps[n].mu.x);
			col.x = (col.x > 255) ? 255 : col.x;
			col.y = col.y + colinc.y * (1 - d->shps[n].mu.y);
			col.y = (col.y > 255) ? 255 : col.y;
			col.z = col.z + colinc.z * (1 - d->shps[n].mu.z);
			col.z = (col.z > 255) ? 255 : col.z;
		}
	return (col);
}

/*
** For each thread in THRD_N call functiom to trace ray
** time measurement:
** struct timeval start,finish;
** double elapsed;
** gettimeofday(&start, NULL);
** gettimeofday(&finish, NULL);
** elapsed = (finish.tv_sec - start.tv_sec);
** elapsed += (finish.tv_usec - start.tv_usec) / 1000000.0;
** printf("THRD: %d TIME: %.2f\n", i, elapsed);
*/

void	*trace_thread(void *dt)
{
	t_3di		p1;
	t_3di		col;
	pthread_t	id;
	t_data		*d;
	int			i;

	d = (t_data*)dt;
	i = -1;
	id = pthread_self();
	while (++i < THRD_N)
		if (pthread_equal(id, d->pth[i]))
			break ;
	p1.y = (i * YS) / THRD_N - 1;
	while (++p1.y < ((i + 1) * YS) / THRD_N && (p1.x = -1))
		while (++p1.x < XS)
		{
			col = trace_one(d, (v_dvop(v_dsop(d->ox, (p1.x - XS / 2), '*')
				, v_dsop(d->oy, (p1.y - YS / 2), '*'), '+')), -1, -1);
			draw_pixel(d, p1.x, p1.y, (col.x << 16) + (col.y << 8) + col.z);
		}
	pthread_exit(NULL);
}

/*
** Split work into THRD_N threads and call function to trace for each thread
** Join threads after complition before continue
** time measurement:
** struct timeval start,finish;
** double elapsed;
** gettimeofday(&start, NULL);
** printf("\n");
** gettimeofday(&finish, NULL);
** elapsed = (finish.tv_sec - start.tv_sec);
** elapsed += (finish.tv_usec - start.tv_usec) / 1000000.0;
** printf("THRD: %d TIME: %.2f\n", i, elapsed);
*/

void	raytrace(t_data *d)
{
	int		i;

	(d && d->img_p) ? mlx_destroy_image(d->mlx, d->img_p) : 0;
	d->img_p = mlx_new_image(d->mlx, XS, YS);
	d->img_p0 = mlx_get_data_addr(d->img_p, &(d->bpp), &(d->ls), &(d->endian));
	i = -1;
	while (++i < THRD_N)
		pthread_create(&(d->pth[i]), NULL, trace_thread, (void *)d);
	i = -1;
	while (++i < THRD_N)
		pthread_join(d->pth[i], NULL);
}
