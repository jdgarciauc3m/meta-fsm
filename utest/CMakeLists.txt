add_executable(utest enum.cpp state.cpp
        machine.cpp
        transition.cpp)
target_link_libraries(utest PRIVATE GTest::gtest_main Microsoft.GSL::GSL)