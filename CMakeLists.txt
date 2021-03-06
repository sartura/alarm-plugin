cmake_minimum_required(VERSION 2.8.12)
project(sysrepo-alarm-plugin)

set(CMAKE_C_FLAGS         "${CMAKE_C_FLAGS} -Wall -Wpedantic -std=gnu11")
set(CMAKE_C_FLAGS_RELEASE "-DNDEBUG -O2")
set(CMAKE_C_FLAGS_DEBUG   "-g -O0")

set(YANG_MODEL "snabb-softwire-v1" CACHE STRING "YANG model to use.")
set(PLUGIN 0 CACHE BOOL "Build a plugin.")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

set(SOURCES
	src/alarm.c)

if (PLUGIN)
	add_library(${CMAKE_PROJECT_NAME} MODULE ${SOURCES})
else()
	add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
endif()

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES PREFIX "")

# dependencies - libyang
find_package(LibYANG REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} ${LIBYANG_LIBRARIES})
include_directories(${LIBYANG_INCLUDE_DIRS})

find_package(SYSREPO REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} ${SYSREPO_LIBRARIES})
include_directories(${SYSREPO_INCLUDE_DIRS})

# get sysrepo plugins directory
if (NOT SR_PLUGINS_DIR)
    if (PKG_CONFIG_FOUND)
        execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} "--variable=SR_PLUGINS_DIR" "libsysrepo" OUTPUT_VARIABLE SR_PLUGINS_DIR)
        string(STRIP ${SR_PLUGINS_DIR} SR_PLUGINS_DIR)
    endif()
endif()
if (NOT SR_PLUGINS_DIR)
    message(FATAL_ERROR "Cannot get sysrepo plugins directory due to missing pkg-config, set SR_PLUGINS_DIR manually.")
endif()

# find programs
if (NOT SYSREPOCTL_EXECUTABLE)
    find_program(SYSREPOCTL_EXECUTABLE sysrepoctl)
endif()
if (NOT SYSREPOCTL_EXECUTABLE)
    message(FATAL_ERROR "Unable to find sysrepoctl, set SYSREPOCTL_EXECUTABLE manually.")
endif()

if (PLUGIN)
	install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${SR_PLUGINS_DIR})
else()
	install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION bin)

	# istall systemd service if it does not exist.
	if(NOT EXISTS "/lib/systemd/system/sysrepo-alarm-plugin.service")
		set(CMD "cp ${CMAKE_SOURCE_DIR}/systemd/sysrepo-alarm-plugin.service /lib/systemd/system/ && systemctl enable sysrepo-alarm-plugin")
		execute_process(COMMAND bash "-c" "${CMD}" OUTPUT_QUIET RESULT_VARIABLE ret)
		if (NOT ${ret} EQUAL 0)
			message(FATAL_ERROR "Error: ${ret}")
		else()
			message("-- Systemd service installed and enabled")
		endif()
	endif()
endif()
