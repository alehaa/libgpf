/* This file is part of sel2gpf.
 *
 * sel2gpf is free software: you can redistribute it and/or modify it under
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

	// read file
	unsigned char buffer[21];
	for (size_t i = 0; i < 21; i++)
		buffer[i] = 0;

	while (fread(&buffer[20], 1, 1, fh) == 1) {
		// search for beginning pattern
		if (buffer[0] == 255 && buffer[1] == 170 && buffer[2] == 255) {
			// print raw output
			//for (size_t i = 0; i < 21; i++)
			//	printf("%.2X ", buffer[i]);
			//printf("\n");


			// validate checksum
			unsigned char checksum = 0;
			for (size_t i = 0; i < 20; i++)
				checksum ^= buffer[i];

			// checksum missmatch
			if (checksum != buffer[20]) continue;


			// get time
			struct tm time_s;
			time_s.tm_hour = buffer[3];
			time_s.tm_min = buffer[4];
			time_s.tm_sec = buffer[5];
			time_s.tm_mon = buffer[7] - 1;
			time_s.tm_mday = buffer[6];
			time_s.tm_year = buffer[8] - 1900;

			time_t tmp_time = mktime(&time_s);
			struct tm *wp_time = localtime(&tmp_time);


			// get velocity
			float v = buffer[9] * 1.852;

			// lat & lon
			float lat = buffer[11] + ((buffer[12] + (buffer[13] / 100.0) + (buffer[14] / 10000.0)) / 60);
			float lon = buffer[15] + ((buffer[16] + (buffer[17] / 100.0) + (buffer[18] / 10000.0)) / 60);
		/*	if (64 == (64 & buffer[10])) lat *= -1;
			if (128 == (128 & buffer[10])) lat *= -1;
		*/


			// contacts
			uint8_t contact[4];
			uint8_t p = 1;
			for (unsigned char k = 0; k < 4; k++) {
				if ((buffer[19] & p) == p) contact[k] = 1;
				else contact[k] = 0;

				p = p << 1;
			}


			// print
			char timebuff[100];
			strftime(timebuff, 100, "%c", wp_time);
			printf("%s\tv: %.6f km/h\t lat: %.6f  lon: %.6f  activated contacts:%s%s%s%s\n",
				timebuff,
				v,
				lat,
				lon,
				contact[0] ? " c1" : "",
				contact[1] ? " c2" : "",
				contact[2] ? " c3" : "",
				contact[3] ? " c4" : ""
			);

			// flush buffer
			for (size_t i = 0; i < 21; i++)
				buffer[i] = 0;

			// skip rest of loop
			continue;
		}

//		if (buffer[0] == 255 && buffer[1] == 44 && buffer[2] == 120 && buffer[3] == 132 && buffer[4] == 238 && buffer[5] == 22) {
		if (buffer[0] == 255) {
			for (size_t i = 0; i < 7; i++)
				printf("%u ", buffer[i]);
			printf("\n");
		}

		// swap items in buffer
		for (size_t i = 0; i < 20; i++)
			buffer[i] = buffer[i + 1];
	}
}
