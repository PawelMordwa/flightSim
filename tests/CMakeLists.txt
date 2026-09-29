# --- wspolny kod pomocniczy testow -------------------------------------------
# Czytnik CSV z danymi referencyjnymi NASA. Osobny target, zeby nie kompilowac
# go po raz drugi w kazdym executable testowym.
add_library(nasa_reference STATIC
    nasa_reference.cpp
)

# PUBLIC, bo naglowek nasa_reference.hpp jest czescia interfejsu tej biblioteki -
# kazdy test, ktory sie z nia linkuje, musi go widziec.
target_include_directories(nasa_reference PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# --- test 1: zachowanie energii i momentu pedu (check-case 2) -----------------
add_executable(flightsimTests
    test_dragless_tumbling_brick.cpp
)

target_link_libraries(flightsimTests PRIVATE flightsim_core)

add_test(NAME dragless_tumbling_brick COMMAND flightsimTests)

# --- test 2: porownanie p/q/r z danymi NASA (check-case 3) --------------------
add_executable(test_checkcase3_vs_nasa
    test_checkcase3_vs_nasa.cpp
)

target_link_libraries(test_checkcase3_vs_nasa PRIVATE flightsim_core nasa_reference)

# Kazdy z 5 niezaleznych symulatorow uczestniczacych w badaniu NESC dostaje
# osobny wpis w CTest. Sciezka do danych idzie jako argument uruchomienia,
# liczona od katalogu ZRODLOWEGO - dzieki temu test dziala niezaleznie od tego,
# z jakiego katalogu roboczego zostal odpalony.
foreach(sim_id 01 02 04 05 06)
    add_test(
        NAME checkcase3_vs_nasa_sim_${sim_id}
        COMMAND test_checkcase3_vs_nasa
                ${CMAKE_CURRENT_SOURCE_DIR}/checkCase3Data/Atmos_03_sim_${sim_id}.csv
    )
endforeach()

# --- test 3: model atmosfery wzgledem US-1976 z danych NASA -------------------
add_executable(test_atmosphere_vs_nasa
    test_atmosphere_vs_nasa.cpp
)

target_link_libraries(test_atmosphere_vs_nasa PRIVATE flightsim_core nasa_reference)

set(checkcase11_dir
    ${CMAKE_CURRENT_SOURCE_DIR}/checkCase11Data/Atmos_11_TrimCheckSubsonicF16/Atmos_11_TrimCheckSubsonicF16)

add_test(
    NAME atmosphere_vs_nasa
    COMMAND test_atmosphere_vs_nasa
            ${CMAKE_CURRENT_SOURCE_DIR}/checkCase3Data/Atmos_03_sim_01.csv
            ${checkcase11_dir}/Atmos_11_sim_02.csv
)
