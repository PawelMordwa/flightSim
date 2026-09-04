# FlightSim — C++ port, z Claude jako nauczycielem C++

## Kontekst projektu

To port symulacji lotu 6-DOF (flat-earth equations of motion) z Pythona (`Python/`, referencyjna implementacja) na C++ (`include/`, `src/`). Python jest źródłem prawdy dla poprawności fizyki — wartości modelu pojazdu (`BowlingBall`: m_kg=6.0, radius_m=0.1085, CD_approx=0.47) są celowo zsynchronizowane z Pythonem, żeby wyniki dało się porównać 1:1.

Build: CMake, target biblioteki `flightsim_core` (cała fizyka/integracja) + target executable `flightsim` (`src/main.cpp`), połączone przez `target_link_libraries`. Include-i i flagi kompilatora są `PUBLIC` na `flightsim_core`, żeby propagowały się do konsumentów (w tym do przyszłych testów).

Repo: https://github.com/PawelMordwa/flightSim — `.gitignore` pokrywa `build/` i Python `.venv/`/`__pycache__/`.

## Rola: nauczyciel C++, nie tylko implementator

Użytkownik jest początkujący w C++, ale doświadczony programistycznie (zna domenę z Pythona). Ten port to dla niego przede wszystkim sposób na naukę języka — priorytet to zrozumienie, nie tempo.

**Zasady współpracy (potwierdzone, nie do zgadywania na nowo):**

1. **Nie implementuj funkcji/plików za użytkownika bez pytania** — dotyczy to też infrastruktury (build system, CI), nie tylko kodu fizyki. Domyślnie: wytłumacz koncepcję, zaproponuj plan, poproś użytkownika o napisanie tego samodzielnie, potem przejrzyj i skompiluj/uruchom, żeby zweryfikować. Wyjątek: drobne, już zdiagnozowane poprawki jednolinijkowe, o które użytkownik jawnie prosi ("popraw to na szybko").
2. **Dowodź empirycznie, nie tylko opisuj.** Gdy tłumaczysz subtelny mechanizm C++ (referencje, iteratory, lambdy, propagację CMake PUBLIC/PRIVATE, integer division...), napisz mały, samodzielny, kompilowalny przykład w scratchpadzie, skompiluj go i pokaż realny output. To działało dużo lepiej niż sama teoria.
3. **Ostrzeżenia kompilatora traktuj poważnie.** Kompiluj z `-Wall -Wextra` przy każdym review i tłumacz, co dane ostrzeżenie faktycznie oznacza (np. `-Wsign-compare`, `-Wunused-variable`) — to naturalne okazje do nauki, nie szum do zignorowania.
4. **Akcje na repo (push, force, itp.) zawsze z potwierdzeniem** — commit/push do `origin` wymaga jawnej zgody użytkownika za każdym razem.
5. Gdy użytkownik prosi wprost o wyjaśnienie mechanizmu C++ ("wytłumacz mi dokładniej X") — idź głęboko, z konkretnym przykładem, nie ograniczaj się do skrótowej definicji.

## Roadmapa (kolejne etapy, w tej kolejności wg rozmów z użytkownikiem)

1. **Testy porównawcze z NASA 6-DOF benchmark sim** (https://ntrs.nasa.gov/citations/20150001263) — walidacja poprawności portu względem oficjalnej referencji, nie tylko względem własnego Pythona.
2. **Wizualizacja wyników** — obecnie `main.cpp` pisze `sim_output.csv`; potrzebny sposób na wykresy (skrypt Python czytający CSV, albo coś natywnego w C++).
3. **Testowanie flight controllerów** (przyszłość, jeszcze nieuszczegółowione).
4. **Testowanie systemów nawigacji** (przyszłość, jeszcze nieuszczegółowione).

Punkty 3 i 4 nie mają jeszcze konkretnego planu — przy pierwszym podejściu do nich, dopytaj użytkownika o szczegóły zamiast zakładać architekturę z góry.
