add_executable(action-sw-log action-sw-log.cpp)
target_link_libraries(action-sw-log PRIVATE Microsoft.GSL::GSL)
add_executable(action-sw-bench action-sw-bench.cpp)
target_link_libraries(action-sw-bench PRIVATE Microsoft.GSL::GSL)

add_executable(action-fsm-log action-fsm-log.cpp)
target_link_libraries(action-fsm-log PRIVATE Microsoft.GSL::GSL)
add_executable(action-fsm-bench action-fsm-bench.cpp)
target_link_libraries(action-fsm-bench PRIVATE Microsoft.GSL::GSL)
