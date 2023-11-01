# Install script for directory: C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/onboardsdk")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/onboardsdk/lib/cmake/djiosdk/DJIOSDKConfig.cmake;C:/Program Files (x86)/onboardsdk/lib/cmake/djiosdk/DJIOSDKConfigVersion.cmake")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "C:/Program Files (x86)/onboardsdk/lib/cmake/djiosdk" TYPE FILE FILES
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/cmake-modules/DJIOSDKConfig.cmake"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/cmake-modules/DJIOSDKConfigVersion.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "shlib" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files (x86)/onboardsdk/lib/djiosdk-core.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files (x86)/onboardsdk/lib" TYPE STATIC_LIBRARY FILES "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/project_folder/libs/Debug/djiosdk-core.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files (x86)/onboardsdk/lib/djiosdk-core.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files (x86)/onboardsdk/lib" TYPE STATIC_LIBRARY FILES "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/project_folder/libs/Release/djiosdk-core.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files (x86)/onboardsdk/lib/djiosdk-core.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files (x86)/onboardsdk/lib" TYPE STATIC_LIBRARY FILES "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/project_folder/libs/MinSizeRel/djiosdk-core.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files (x86)/onboardsdk/lib/djiosdk-core.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files (x86)/onboardsdk/lib" TYPE STATIC_LIBRARY FILES "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/project_folder/libs/RelWithDebInfo/djiosdk-core.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_ack.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_broadcast.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_camera.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_camera_manager.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_command.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_control.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_error.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_flight_controller.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_gimbal.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_hardware_sync.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_hotpoint.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_mfio.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_mission_base.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_mission_manager.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_mission_type.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_mobile_communication.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_mobile_device.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_payload_device.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_psdk_manager.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_status.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_subscription.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_telemetry.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_telemetry_doc.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_type.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_vehicle.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_vehicle_callback.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_version.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_virtual_rc.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_waypoint.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_hard_driver.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_log.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_memory.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_platform_manager.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_thread_manager.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_flight_actions_module.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_flight_assistant_module.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_flight_link.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_camera_module.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_payload_base.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_payload_link.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_psdk_module.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/linux_serial_device.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/posix_thread.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/posix_thread_manager.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_aes.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_crc.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_open_protocol.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_protocol_base.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_circular_buffer.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_macros.hpp;C:/Program Files (x86)/onboardsdk/include/djiosdk/dji_singleton.hpp")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "C:/Program Files (x86)/onboardsdk/include/djiosdk" TYPE FILE FILES
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_ack.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_broadcast.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_camera.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_camera_manager.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_command.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_control.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_error.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_flight_controller.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_gimbal.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_hardware_sync.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_hotpoint.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_mfio.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_mission_base.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_mission_manager.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_mission_type.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_mobile_communication.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_mobile_device.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_payload_device.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_psdk_manager.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_status.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_subscription.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_telemetry.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_telemetry_doc.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_type.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_vehicle.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_vehicle_callback.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_version.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_virtual_rc.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/api/inc/dji_waypoint.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/hal/inc/dji_hard_driver.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/hal/inc/dji_log.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/hal/inc/dji_memory.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/hal/inc/dji_platform_manager.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/hal/inc/dji_thread_manager.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/modules/inc/flight/dji_flight_actions_module.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/modules/inc/flight/dji_flight_assistant_module.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/modules/inc/flight/dji_flight_link.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/modules/inc/payload/dji_camera_module.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/modules/inc/payload/dji_payload_base.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/modules/inc/payload/dji_payload_link.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/modules/inc/payload/dji_psdk_module.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/platform/linux/inc/linux_serial_device.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/platform/linux/inc/posix_thread.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/platform/linux/inc/posix_thread_manager.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/protocol/inc/dji_aes.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/protocol/inc/dji_crc.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/protocol/inc/dji_open_protocol.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/protocol/inc/dji_protocol_base.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/utility/inc/dji_circular_buffer.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/utility/inc/dji_macros.hpp"
    "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/osdk-core/utility/inc/dji_singleton.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}C:/Program Files (x86)/onboardsdk/lib/cmake/djiosdk/djiosdkTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}C:/Program Files (x86)/onboardsdk/lib/cmake/djiosdk/djiosdkTargets.cmake"
         "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/project_folder/osdk-core/CMakeFiles/Export/17f80bb2658fe3ee7580838c3649d379/djiosdkTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}C:/Program Files (x86)/onboardsdk/lib/cmake/djiosdk/djiosdkTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}C:/Program Files (x86)/onboardsdk/lib/cmake/djiosdk/djiosdkTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/onboardsdk/lib/cmake/djiosdk/djiosdkTargets.cmake")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "C:/Program Files (x86)/onboardsdk/lib/cmake/djiosdk" TYPE FILE FILES "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/project_folder/osdk-core/CMakeFiles/Export/17f80bb2658fe3ee7580838c3649d379/djiosdkTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files (x86)/onboardsdk/lib/cmake/djiosdk/djiosdkTargets-debug.cmake")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files (x86)/onboardsdk/lib/cmake/djiosdk" TYPE FILE FILES "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/project_folder/osdk-core/CMakeFiles/Export/17f80bb2658fe3ee7580838c3649d379/djiosdkTargets-debug.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files (x86)/onboardsdk/lib/cmake/djiosdk/djiosdkTargets-minsizerel.cmake")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files (x86)/onboardsdk/lib/cmake/djiosdk" TYPE FILE FILES "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/project_folder/osdk-core/CMakeFiles/Export/17f80bb2658fe3ee7580838c3649d379/djiosdkTargets-minsizerel.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files (x86)/onboardsdk/lib/cmake/djiosdk/djiosdkTargets-relwithdebinfo.cmake")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files (x86)/onboardsdk/lib/cmake/djiosdk" TYPE FILE FILES "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/project_folder/osdk-core/CMakeFiles/Export/17f80bb2658fe3ee7580838c3649d379/djiosdkTargets-relwithdebinfo.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files (x86)/onboardsdk/lib/cmake/djiosdk/djiosdkTargets-release.cmake")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files (x86)/onboardsdk/lib/cmake/djiosdk" TYPE FILE FILES "C:/Users/rdybs/Desktop/gitnstuff/P5/Onboard-SDK/project_folder/osdk-core/CMakeFiles/Export/17f80bb2658fe3ee7580838c3649d379/djiosdkTargets-release.cmake")
  endif()
endif()

