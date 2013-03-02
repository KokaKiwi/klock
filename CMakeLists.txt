cmake_minimum_required(VERSION 2.8)

project(klock)

find_package(X11 REQUIRED)

set(CMAKE_C_FLAGS "-g")

include_directories(include)

add_executable(klock
    klock.c)
target_link_libraries(klock
    ${X11_LIBRARIES}
    crypt)

install(TARGETS klock
    DESTINATION "bin"
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
                GROUP_EXECUTE WORLD_READ WORLD_EXECUTE SETUID)
