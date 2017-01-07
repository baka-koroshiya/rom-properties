PROJECT(rom-properties-gnome)

# GNOME (Nautilus) frontend for rom-properties.

FIND_PACKAGE(GLib2 2.26.0)
FIND_PACKAGE(GdkPixbuf2)
# GTK3 minimum is 3.0.0, so no version check is needed.
FIND_PACKAGE(GTK3)
IF(GLib2_FOUND AND GdkPixbuf2_FOUND AND GTK3_FOUND)
	# Find the Nautilus extension library.
	# TODO: Minimum version, and ensure it's 3, not 4?
	FIND_PACKAGE(LibNautilusExtension 3.0.0)
	IF(NOT LibNautilusExtension_FOUND)
		# Nautilus extension library was not found.
		# Disable the GNOME frontend.
		SET(BUILD_GNOME OFF CACHE "" INTERNAL FORCE)
	ENDIF(NOT LibNautilusExtension_FOUND)
ELSE(GLib2_FOUND AND GdkPixbuf2_FOUND AND GTK3_FOUND)
	# GTK+ 3.x (or required dependencies) were not found.
	# Disable the GNOME frontend.
	SET(BUILD_GNOME OFF CACHE "" INTERNAL FORCE)
ENDIF(GLib2_FOUND AND GdkPixbuf2_FOUND AND GTK3_FOUND)

# Sources and headers. (GTK+ common)
STRING(REGEX REPLACE "([^;]+)" "../\\1" rom-properties-gnome_SRCS "${rom-properties-gtk_SRCS}")
STRING(REGEX REPLACE "([^;]+)" "../\\1" rom-properties-gnome_H "${rom-properties-gtk_H}")

# Sources and headers. (GNOME-specific)
SET(rom-properties-gnome_SRCS
	${rom-properties-gnome_SRCS}
	rom-properties-plugin.c
	rom-properties-provider.cpp
	)
SET(rom-properties-gnome_H
	${rom-properties-gnome_H}
	rom-properties-provider.hpp
	)

IF(BUILD_GNOME)
	ADD_LIBRARY(rom-properties-gnome MODULE
		${rom-properties-gnome_SRCS}
		)
	SET_TARGET_PROPERTIES(rom-properties-gnome PROPERTIES PREFIX "")
	DO_SPLIT_DEBUG(rom-properties-gnome)
	TARGET_INCLUDE_DIRECTORIES(rom-properties-gnome
		PUBLIC	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
		PRIVATE	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/..>
			$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
			$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/src>
		)
	TARGET_LINK_LIBRARIES(rom-properties-gnome romdata8 cachemgr8)
	TARGET_LINK_LIBRARIES(rom-properties-gnome Gnome::libnautilus-extension)
	TARGET_LINK_LIBRARIES(rom-properties-gnome Gtk3::gtk3 GdkPixbuf2::gdkpixbuf2 GLib2::glib2)
	ADD_DEFINITIONS(-DG_LOG_DOMAIN=\"rom-properties-gnome\")
ENDIF(BUILD_GNOME)

#######################
# Install the plugin. #
#######################

# TODO: Update for GNOME.
IF(BUILD_GNOME)
	# FIXME: ${LibNautilusExtension_EXTENSION_DIR} always uses the system prefix.
	# This should be adjusted to ${CMAKE_INSTALL_PREFIX}.
	INSTALL(TARGETS rom-properties-gnome
		LIBRARY DESTINATION "${LibNautilusExtension_EXTENSION_DIR}"
		COMPONENT "plugin"
		)

	# Check if a split debug file should be installed.
	IF(INSTALL_DEBUG)
		# FIXME: Generator expression $<TARGET_PROPERTY:${_target},PDB> didn't work with CPack-3.6.1.
		GET_TARGET_PROPERTY(DEBUG_FILENAME rom-properties-gnome PDB)
		IF(DEBUG_FILENAME)
			INSTALL(FILES "${DEBUG_FILENAME}"
				DESTINATION "lib/debug/${LibNautilusExtension_EXTENSION_DIR}"
				COMPONENT "debug"
				)
		ENDIF(DEBUG_FILENAME)
	ENDIF(INSTALL_DEBUG)
ENDIF(BUILD_GNOME)