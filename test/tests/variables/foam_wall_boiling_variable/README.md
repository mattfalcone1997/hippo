# Boiling wall heat flux

This integration test derives from the OpenFOAM 14
`tutorials/multiphaseEuler/wallBoilingIATE` case. It runs the real
`multiphaseEuler` solver, including IATE, wall boiling, bulk phase change,
and the tutorial's turbulence, thermophysical transport models and thermodynamic property tables.

Both phases use `multiphaseExternalTemperature` on `wall`, specifying the
same total mixture heat flux of **73890 W/m²**. The two specifications do
not double the heat input.

## Short startup

The original tutorial delays heating until 0.5 s. Here heat is applied
immediately. The last two steps, 0.0009 and 0.001 s, are checked.
This is a short transient regression, not a converged physical validation
of the tutorial's boiling correlations or heat-flux partition.
The checks require measurable boiling and detect double counting of the boiling heat flux.

## Running

From the repository root, with Hippo built:

```bash
source external/openfoam/OpenFOAM-14/etc/bashrc
unset FOAM_SIGFPE FOAM_SETNAN
./run_tests -j4 -C test/tests/variables/foam_wall_boiling_variable/
```
