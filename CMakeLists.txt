cmake_minimum_required (VERSION 3.8)
project (kvasir CXX)

# C++ standard
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

# Tune the warning level
if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall")
elseif (MSVC)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
endif ()

# Qt library
set (CMAKE_AUTOMOC ON) # enable MOC automatically for Qt targets
set (CMAKE_AUTORCC ON) # enable RCC automatically for Qt targets
find_package (Qt5 COMPONENTS Core Gui Multimedia SerialPort Sql REQUIRED)

set (SOURCES
    config.h
    config.cpp    
    logger.h
    logger.cpp
    main.cpp
	scanner.h
    scanner.cpp
    uniden.h
)

add_executable (kvasir ${SOURCES})
target_link_libraries (kvasir Qt5::Core Qt5::SerialPort Qt5::Multimedia Qt5::Network Qt5::Gui Qt5::Sql)

# To make debugging easier
add_custom_command(TARGET kvasir POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE_DIR:kvasir>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core> $<TARGET_FILE_DIR:kvasir>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::SerialPort> $<TARGET_FILE_DIR:kvasir>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Multimedia> $<TARGET_FILE_DIR:kvasir>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Network> $<TARGET_FILE_DIR:kvasir>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Sql> $<TARGET_FILE_DIR:kvasir>
)