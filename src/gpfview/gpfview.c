/* This file is part of libGPF.
 *
 * libGPF is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
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

/** \brief Application to decode an .llv file
 *
 *
 * \copyright 2014 Alexander Haase <alexander.haase@rwth-aachen.de>
 *
 * \date 10.07.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#include <gpf.h>


int main (int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s [file]\n", argv[0]);
		exit(EXIT_FAILURE);
	}


	// open file
	gpf_file_t *fh = gpf_open(argv[1]);
	if (fh == NULL) {
		fprintf(stderr, "Error opening file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	// read data from file
	int c;
	gpf_waypoint_t wpoint;
	while ((c = gpf_read(fh, &wpoint)) >= 0) {
		// skip end of route
		if (c == 0) {
			printf("+++ End of path\n\n");
			continue;
		}

		// print
		char timebuff[100];
		strftime(timebuff, 100, "%c", &(wpoint.wp_time));
		printf("%s  v: %f km/h \tlat: %f  lon: %f contacts:%s%s%s%s\n",
			timebuff,
			wpoint.wp_velocity,
			wpoint.wp_lat,
			wpoint.wp_lon,

			(wpoint.wp_contacts & 1) ? " 1" : "",
			(wpoint.wp_contacts & 2) ? " 2" : "",
			(wpoint.wp_contacts & 4) ? " 3" : "",
			(wpoint.wp_contacts & 8) ? " 4" : ""
		);
	}

	// close file
	gpf_close(fh);

	if (c == -1) return EXIT_SUCCESS;
	else return EXIT_FAILURE;
}
