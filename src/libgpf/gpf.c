/* This file is part of libGPF.
 *
 * libGPF is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
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

/** \file
 *
 * \copyright 2014 Alexander Haase <alexander.haase@rwth-aachen.de>
 *
 * \date 10.07.2014
 */

#include <stdio.h>

#include "gpf.h"


/** \brief Get next waypoint in file.
 *
 *
 * \param fh Pointer to a gpf_file_t object, that identifies the stream.
 * \param wp Pointer to the waypoint struct, where found data shouls be copied
 *  in.
 *
 * \return Returns 1 on success, 0 if end of file reached and -1 if an error has
 *  occured.
 */
int gpf_read (FILE *fh, gpf_waypoint_t *wp)
{
	/* get actual position in fh, for if it will be neccesary to jump back to
	 * this position (or a position relative to this) */
	long actual_pos = ftell(fh);
	if (actual_pos < 0) return -1;

	/* fill buffer with data
	 *
	 * Buffer will be filled with data from fh. Checks if end of file reached,
	 * or other error happened and returns on failure.
	 */
	size_t buffer_size = 21;
	unsigned char buffer[buffer_size];
	if (fread(&buffer, sizeof(char), buffer_size, fh) != buffer_size) {
		// end of file reached
		if (feof(fh)) return 0;

		// an error occured
		else return -1;
	}


	/* is there a spacer element?
	 *
	 * Spacers (currently) have no specific pattern, but start with 255 and are
	 * 6 bytes long. So buffer[6-8] must match "255 170 255". Then we have to
	 * jump to this position and re-read the buffer.
	 */
	if (buffer[0] == 255 && (buffer[1] != 170 || buffer[2] != 255) && buffer[6] == 255 && buffer[7] == 170 && buffer[8] == 255) {
		// jump to new position
		fseek(fh, actual_pos + 6, SEEK_SET);

		// re-read data into buffer
		if (fread(&buffer, sizeof(char), buffer_size, fh) != buffer_size) {
			// end of file reached
			if (feof(fh)) return 0;

			// an error occured
			else return -1;
		}
	}


	/* is content of buffer a waypoint?
	 *
	 * Waypoints have to start with "255 170 255"
	 */
	if (buffer[0] == 255 && buffer[1] == 170 && buffer[2] == 255) {
		// calculate checksum
		unsigned char checksum = 0;
		for (size_t i = 0; i < 20; i++)
			checksum ^= buffer[i];

		// validate checksum
		if (checksum != buffer[20]) return -1;


		// get time of waypoint
		wp->wp_time.tm_hour = buffer[3];
		wp->wp_time.tm_min = buffer[4];
		wp->wp_time.tm_sec = buffer[5];
		wp->wp_time.tm_mon = buffer[7] - 1;
		wp->wp_time.tm_mday = buffer[6];
		wp->wp_time.tm_year = buffer[8] + 100;
		mktime(&wp->wp_time);

		// get velocity
		wp->wp_velocity = buffer[9] * 1.852;

		// lat & lon
		wp->wp_lat = buffer[11] + ((buffer[12] + (buffer[13] / 100.0) + (buffer[14] / 10000.0)) / 60);
		wp->wp_lon = buffer[15] + ((buffer[16] + (buffer[17] / 100.0) + (buffer[18] / 10000.0)) / 60);

		// contacts
		wp->wp_contacts = buffer[19];


		// return match
		return 1;
	}


	// an error occured
	return -1;
}
