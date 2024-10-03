cmake_minimum_required(VERSION 3.14)
project(PlatformerSFML)

set(CMAKE_CXX_STANDARD 17)

add_executable(PlatformerSFML src/main.cpp
        src/Game.h
        src/Game.cpp
        src/GameObject.h
        src/GameObject.cpp
        src/main.cpp
        src/Vector2.cpp
        src/Vector2.h
        src/Map.cpp
        src/Map.h
        src/Map.h
        src/Dynamic.h
        src/Dynamic.cpp
)

include_directories(headers c:/SFML/include)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
set(CMAKE_EXE_LINKER_FLAGS "-static")
find_package(SFML REQUIRED system window graphics)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(PlatformerSFML ${SFML_LIBRARIES})
endif ()

file(COPY Data DESTINATION ${CMAKE_BINARY_DIR})

# Copy dlls to build directory
if(WIN32)
    file(GLOB BINARY_DEP_DLLS "${SFML_INCLUDE_DIR}/../bin/*.dll")
    file(COPY ${BINARY_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif ()





#cmake_minimum_required(VERSION 3.14)
#project(SpaceInvadersSFML)
#
#set(CMAKE_CXX_STANDARD 14)
#
#if(WIN32)
#    set(SFML_STATIC_LIBRARIES TRUE)
#    set(CMAKE_EXE_LINKER_FLAGS "-static")
#endif()
#
#
#find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)
#set(SOURCE_FILES src/main.cpp src/Game.cpp src/Game.h src/Vector2.h src/Vector2.cpp src/GameObject.h src/GameObject.cpp)
#add_executable(SpaceInvadersSFML ${SOURCE_FILES})
#
#set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
#
#target_link_libraries (SpaceInvadersSFML sfml-graphics sfml-window sfml-system sfml-network sfml-audio)
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Data/)