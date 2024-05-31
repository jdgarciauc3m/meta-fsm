add_executable(simple-sw-log simple-sw-log.cpp)
target_link_libraries(simple-sw-log PRIVATE Microsoft.GSL::GSL)
add_executable(simple-sw-bench simple-sw-bench.cpp)
target_link_libraries(simple-sw-bench PRIVATE Microsoft.GSL::GSL)

add_executable(simple-fsm-log simple-fsm-log.cpp)
target_link_libraries(simple-fsm-log PRIVATE Microsoft.GSL::GSL)
add_executable(simple-fsm-bench simple-fsm-bench.cpp)
target_link_libraries(simple-fsm-bench PRIVATE Microsoft.GSL::GSL)
