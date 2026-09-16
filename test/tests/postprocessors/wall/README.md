# Wall postprocessors

The local OpenFOAM fixture is copied from side_average and uses its postprocessorTest solver: a
10 x 1 x 1 box scaled to depth 2, graded along x, with T=x*t and unit conductivity. Compare wall
temperature averages, average heat flux, integrated heat flux and average HTC
against analytical values at every time step. Temperature and heat-flux results
also match the existing side-average/integral postprocessors.

Non-unit end areas distinguish integrals from averages; unequal patch areas test weighting; opposite end patches test signs and
cancellation. HTC uses distinct fixed bulk temperatures on the two ends and
reversed boundary order to check the mapping. The two-rank run includes ranks
with no faces on an end patch and verifies the global reductions. Run and verify
serially before reusing the output for the parallel run. A malformed HTC list
must fail with an error explaining that exactly one bulk temperature user object is required per boundary.
