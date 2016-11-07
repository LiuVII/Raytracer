/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfilipch <mfilipch@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/23 15:27:50 by mfilipch          #+#    #+#             */
/*   Updated: 2016/09/23 15:27:52 by mfilipch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYTRACER_H
# define RAYTRACER_H
# include "window.h"

# define FOV (M_PI / 3.0)
# define GR_S 256
# define PP_DST (XS / 2.0 / tan(FOV / 2.0))
# define ANIX (FOV / (double)XS)
# define ANIY (FOV / (double)YS)
# define PP_CX (XS / 2)
# define PP_CY (YS / 2)
# define PP_SCL sqrt(XS * XS + YS * YS)
# define TEXT_S GR_S

void			jump(t_data *d);
int				draw_win(t_data *d);
void			draw_3dmap(t_data *d, t_3d p1, double dist, double nesw);
void			draw_line_im(t_data *d, t_3d p1, t_3d p2, t_img img);
void			load_txtr(t_data *d);
void			destroy_txtr(t_data *d);

#endif
