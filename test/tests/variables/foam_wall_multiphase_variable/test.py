"""Analytical phase-weighted quantities on all six cube boundaries."""

import unittest

import numpy as np
from read_hippo_data import get_exodus_times, read_moose_exodus_data


class TestMultiphaseWallVariables(unittest.TestCase):
    def test_quantities(self):
        times = np.asarray(get_exodus_times("main_out.e"))
        np.testing.assert_allclose(times, [0, 0.01, 0.02, 0.03], rtol=1e-7, atol=1e-12)
        for time in times[1:]:
            coords, temperature = read_moose_exodus_data(
                "main_out.e", time, "temperature"
            )
            xyz = np.column_stack([coords[c] for c in ("x", "y", "z")])
            self.assertEqual(len(xyz), 96)
            cell = xyz.copy()
            normal = np.zeros_like(xyz)
            memberships = np.zeros(len(xyz), dtype=int)
            for axis in range(3):
                for location, sign in ((0.0, -1.0), (1.0, 1.0)):
                    mask = np.isclose(xyz[:, axis], location, atol=1e-12, rtol=0)
                    self.assertEqual(mask.sum(), 16)
                    memberships += mask
                    normal[mask, axis] = sign
                    cell[mask, axis] -= sign * 0.125
            np.testing.assert_array_equal(memberships, 1)
            x, y, z = xyz.T
            cx, cy, cz = cell.T
            tw = 315 + 1.75 * (x * y + y * z + z * x) * time
            gradients = np.column_stack((y + z, x + z, x + y)) * time
            flux = 4.75 * np.sum(gradients * normal, axis=1)
            expected = {
                "temperature": tw,
                "flux": flux,
                "bulk": 315 + 1.75 * (cx * cy + cy * cz + cz * cx) * time,
                "htc": flux / (tw - 290),
            }
            for name, reference in expected.items():
                positions, values = read_moose_exodus_data("main_out.e", time, name)
                np.testing.assert_array_equal(
                    np.column_stack([positions[c] for c in ("x", "y", "z")]), xyz
                )
                self.assertEqual(values.shape, reference.shape)
                self.assertTrue(np.isfinite(values).all())
                np.testing.assert_allclose(
                    values,
                    reference,
                    rtol=1e-7,
                    atol=1e-12,
                    err_msg=f"{name} at time {time}",
                )
