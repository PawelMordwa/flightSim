# flightSim

A 6-DOF (six degrees of freedom) flat-earth flight simulation, ported from a
Python reference implementation to C++.

The Python version in `Python/` is the source of truth for the physics. The C++
port in `include/` and `src/` is built to reproduce its results one-to-one, so
every change can be checked against a known-good reference.

> **Status:** work in progress. Preparation for my master's thesis, and a way to
> learn modern C++ on a problem I already understand well.

## What it does

- Integrates the rigid-body equations of motion (translation and rotation, flat-earth)
- Includes a simple aerodynamic drag model
- Writes the trajectory to `sim_output.csv`
- Ships with a test vehicle (a bowling ball: m = 6.0 kg, r = 0.1085 m, C_D ≈ 0.47)
  whose parameters are kept identical in the Python and C++ versions

## Project layout

| Path | Contents |
|---|---|
| `Python/` | Reference implementation (source of truth for the physics) |
| `include/`, `src/` | C++ port |
| `tests/` | Check case data and compatibility test |
| `Documentation/` | NASA check cases description |
| `CMakeLists.txt` | Build: `flightsim_core` (physics and integration) + `flightsim` (executable) |

## Build and run

```bash
cmake -S . -B build
cmake --build build
./build/flightsim        # writes sim_output.csv
```

## Verification

C++ output is compared against the Python reference using the same initial
conditions and vehicle parameters.

| Case | Max difference (C++ vs Python) |
|---|---|
| [e.g. ballistic drop, 10 s] | [value] |
| [e.g. ...] | [value] |

[Add one plot of the trajectory here: it is the first thing a visitor will look at.]

## Roadmap

1. Validate against NASA's published 6-DOF benchmark cases
   (https://ntrs.nasa.gov/citations/20150001263)
2. Visualisation of results (currently CSV only)
3. Testing flight controllers in the loop
4. Testing navigation systems

## Known limitations

- Flat-earth model only (no rotating or curved-earth effects)
- Not yet validated against an independent reference
