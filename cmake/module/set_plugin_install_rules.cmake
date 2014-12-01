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

macro(set_plugin_install_rules
  target
  )

qt5_use_modules(${target} Declarative)
qt5_use_modules(${target} Core)
qt5_use_modules(${target} Gui)
qt5_use_modules(${target} Quick)
qt5_use_modules(${target} Widgets)
qt5_use_modules(${target} Sql)
qt5_use_modules(${target} Network)
qt5_use_modules(${target} Xml)
qt5_use_modules(${target} OpenGL)


################################################################################
#
# Usage: set_plugin_install_rules(target)
# set rules for the plugin designed by the target
#
################################################################################

install(TARGETS ${target}
  RUNTIME DESTINATION plugins
  LIBRARY DESTINATION plugins
  ARCHIVE DESTINATION lib
  )

endmacro()
