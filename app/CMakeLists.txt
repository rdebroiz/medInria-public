################################################################################
#
# medInria
#
# Copyright (c) INRIA 2013 - 2014. All rights reserved.
# See LICENSE.txt for details.
# 
#  This software is distributed WITHOUT ANY WARRANTY; without even
#  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#  PURPOSE.
#
################################################################################

add_subdirectory(medInria)

option(MEDINRIA_BUILD_TOOLS "Build tools" OFF)
if(MEDINRIA_BUILD_TOOLS)
  add_subdirectory(medPluginGenerator)
endif()

