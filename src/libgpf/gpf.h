/* This file is part of SelnexGPF.
 *
 * SelnexGPF is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Copyright (C)
 *  2014 Alexander Haase <alexander.haase@rwth-aachen.de>
 */

/** \brief Library to decode selnex GPF-Files
 *
 *
 * \copyright 2014 Alexander Haase <alexander.haase@rwth-aachen.de>
 *
 * \date 10.07.2014
 */


#ifndef SELNEXGPF_DECODE_H
#define SELNEXGPF_DECODE_H

#include <time.h>
#include <stdint.h>
#include <stdio.h>


/** \brief Struct that stores all information about one specific waypoint
 *
 */
typedef struct gpf_waypoint_s {
	struct tm wp_time;
	/**< Struct containing the time when this waypoint was passed */

	float wp_lat;
	/**< Latitude of this waypoint */

	float wp_lon;
	/**< Longitude of this waypoint */

	float wp_velocity;
	/**< Velocity of tracking-device at this waypoiint */

	uint8_t wp_contacts;
	/**< Integer that contains bitwise the state of the four contacts of the
 	 * tracking device. 2^0 will be contact 0, 2^1 contact 1, etc. */
} gpf_waypoint_t;

/* \brief File handle for gpf-file.
 */
typedef FILE gpf_file_t;



/*
 * functions
 */
gpf_file_t * gpf_open (const char *filename);

int gpf_read (gpf_file_t *fh, gpf_waypoint_t *wp);

int gpf_close (gpf_file_t *fh);

#endif