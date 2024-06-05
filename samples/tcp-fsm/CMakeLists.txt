add_executable(tcp-original tcp-original.cpp)
target_compile_options(tcp-original PUBLIC -w)

add_executable(tcp-sw tcp-sw.cpp)
target_link_libraries(tcp-sw PRIVATE Microsoft.GSL::GSL)

add_executable(tcp-fsm tcp-fsm.cpp)
target_link_libraries(tcp-fsm PRIVATE Microsoft.GSL::GSL)