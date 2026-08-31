# Minimalna wersja CMake wymagana do zbudowania tego projektu.
cmake_minimum_required(VERSION 3.16)

# Nazwa projektu i jezyk (CXX = C++). To tworzy zmienne typu
# PROJECT_NAME, PROJECT_SOURCE_DIR itp.
project(FlightSim LANGUAGES CXX)

# Standard C++ ktorego uzywamy (C++17) i wymuszenie go bez fallbacku
# na starszy standard, jesli kompilator by go nie wspieral.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(flightsim_core STATIC
    src/model_equations.cpp
    src/numerical_integration.cpp
    src/simulation_setup.cpp
    src/vehicle_model.cpp
)

# Definiujemy cel budowania (target) typu executable o nazwie
# "flightsim", zlozony z podanych plikow zrodlowych.
add_executable(flightsim
    src/main.cpp
)


# Mowimy kompilatorowi, gdzie szukac plikow naglowkowych (#include),
# ktorych nie ma w tym samym katalogu co plik .cpp. PRIVATE oznacza,
# ze ta sciezka jest potrzebna tylko do zbudowania samego "flightsim",
# nie eksportujemy jej dalej (nie ma tu innych targetow, ktore by z niej korzystaly).
target_include_directories(flightsim_core PUBLIC include)

# Wlacz ostrzezenia kompilatora - pomagaja wylapac bledy typu
# "unused variable" czy "sign-compare", ktore widzielismy wczesniej.
target_compile_options(flightsim_core PUBLIC -Wall -Wextra)

target_link_libraries(flightsim PRIVATE flightsim_core)