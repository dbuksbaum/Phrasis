# - Try to find the link-grammar library
# Once done this will define
#
#  LINK_GRAMMER_FOUND - system has the LibArt
#  LINK_GRAMMER_INCLUDE_DIR - the LibArt include directory
#  LINK_GRAMMER_LIBRARIES - The libraries needed to use LibArt
#
# Copyright (c) 2006, Jacob Rideout, <jacob@jacobrideout.net>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

FIND_PATH(LINK_GRAMMER_INCLUDE_DIR link-grammar/link-includes.h )

FIND_LIBRARY(LINK_GRAMMER_LIBRARIES NAMES link-grammar )


IF (LINK_GRAMMER_INCLUDE_DIR AND LINK_GRAMMER_LIBRARIES)
   SET(LINK_GRAMMER_FOUND TRUE)
ENDIF (LINK_GRAMMER_INCLUDE_DIR AND LINK_GRAMMER_LIBRARIES)

IF (LINK_GRAMMER_FOUND)

  IF (NOT link-grammar_FIND_QUIETLY)
    MESSAGE(STATUS "Found link-grammar: ${LINK_GRAMMER_LIBRARIES}")
  ENDIF (NOT link-grammar_FIND_QUIETLY)

ELSE (LINK_GRAMMER_FOUND)

  IF (link-grammar_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could NOT find link-grammar")
  ENDIF (link-grammar_FIND_REQUIRED)

ENDIF (LINK_GRAMMER_FOUND)

MARK_AS_ADVANCED(LINK_GRAMMER_INCLUDE_DIR LINK_GRAMMER_LIBRARIES)
