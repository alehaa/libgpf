# This file is part of libGPF.
#
# libGPF is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
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

# include libgpf header
include_directories("${PROJECT_SOURCE_DIR}/src/libgpf")

# add executable
add_executable (gpfview gpfview.c)

# link libgpf
target_link_libraries(gpfview gpf)

# install binary
install(TARGETS gpfview DESTINATION bin)
