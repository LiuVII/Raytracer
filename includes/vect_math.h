/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect_math.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfilipch <mfilipch@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/23 15:27:50 by mfilipch          #+#    #+#             */
/*   Updated: 2016/09/23 15:27:52 by mfilipch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECT_MATH_H
# define VECT_MATH_H

double	v_imod(t_3di p);
double	v_imodsq(t_3di p);
t_3di	v_isop(t_3di p1, double x, char c);
t_3di	v_ivop(t_3di p1, t_3di p2, char c);
double	v_iscal(t_3di p1, t_3di p2);
t_3di	v_id2v(t_3di p1, t_3di p2);
double	v_dmod(t_3d p);
double	v_dmodsq(t_3d p);
t_3d	v_dsop(t_3d p1, double x, char c);
t_3d	v_dvop(t_3d p1, t_3d p2, char c);
double	v_dscal(t_3d p1, t_3d p2);
t_3d	v_dd2v(t_3d p1, t_3d p2);
t_3d	v_i2d(t_3di pi);
t_3di	v_d2i(t_3d pd);

#endif
