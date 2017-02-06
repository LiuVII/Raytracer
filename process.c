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
