project(distributions)

add_executable(distributions main.cpp)

install(TARGETS distributions RUNTIME DESTINATION bin)
