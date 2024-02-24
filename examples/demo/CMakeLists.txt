

add_executable(shared_ex main.cpp)

target_link_libraries(shared_ex PRIVATE xhttp::xhttp)