# This file is part of libGPF.
#
# libGPF is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along
# with this program. If not, see <http://www.gnu.org/licenses/>.
#
#
# Copyright (C)
#   2013-2014 Alexander Haase <alexander.haase@rwth-aachen.de>
#

# set C99 standard
set (CMAKE_C_FLAGS "-std=c99")


# build shared library
add_library(gpf SHARED gpf.c)

set_target_properties(gpf PROPERTIES
	VERSION 1.0.0
	SOVERSION 1
)


# build static library
add_library(gpfStatic STATIC gpf.c)

set_target_properties(gpfStatic PROPERTIES OUTPUT_NAME gpf)


# install library
install(TARGETS gpf DESTINATION lib)
install(TARGETS gpfStatic DESTINATION lib)

# install header
install(FILES gpf.h DESTINATION include)
