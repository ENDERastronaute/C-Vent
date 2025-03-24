
add_executable(UnitTests test_events.cpp)

target_link_libraries(UnitTests PRIVATE C_Vent Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(UnitTests)