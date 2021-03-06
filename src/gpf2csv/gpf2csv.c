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

/** \file
 *
 * \brief Application to view contents of an *.gpf file.
 *
 * \details This application shows the contents of an *.gpf file. All entrys for
 *  a waypoint will be shown in one line.
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


/** \brief Main loop function
 *
 */
int main (int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s [file]\n", argv[0]);
		exit(EXIT_FAILURE);
	}


	// open file
	FILE *fh = fopen(argv[1], "r");
	if (fh == NULL) {
		fprintf(stderr, "Error opening file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	// read data from file
	int c;
	gpf_waypoint_t wpoint;
	while ((c = gpf_read(fh, &wpoint)) > 0) {
		// print
	//	char timebuff[100];
	//	strftime(timebuff, 100, "%c", &(wpoint.wp_time));
		printf("%d;%f;%f;%f;%d;%d;%d;%d\n",
			(unsigned int) mktime(&(wpoint.wp_time)),
			wpoint.wp_velocity,
			wpoint.wp_lat,
			wpoint.wp_lon,

			(wpoint.wp_contacts & 1) ? 1 : 0,
			(wpoint.wp_contacts & 2) ? 1 : 0,
			(wpoint.wp_contacts & 4) ? 1 : 0,
			(wpoint.wp_contacts & 8) ? 1 : 0
		);
	}

	// close file
	fclose(fh);

	if (c == 0) return EXIT_SUCCESS;
	else {
		printf("An error occured: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
}
