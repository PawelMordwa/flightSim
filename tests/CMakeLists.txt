add_executable(flightsimTests
    test_dragless_tumbling_brick.cpp
)

target_link_libraries(flightsimTests PRIVATE flightsim_core)

add_test(NAME dragless_tumbling_brick COMMAND flightsimTests)