

add_executable(request request.cpp)

target_link_libraries(request PRIVATE xhttp::xhttp)

add_executable(response response.cpp)

target_link_libraries(response PRIVATE xhttp::xhttp)