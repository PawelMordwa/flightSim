"""Utility helpers for the flight simulation project."""

from __future__ import annotations

import numpy as np


def fastInterp1(x, y, xq):
	"""Fast 1D linear interpolation with edge clamping."""

	x = np.asarray(x, dtype=float)
	y = np.asarray(y, dtype=float)
	xq_arr = np.asarray(xq, dtype=float)

	if x.ndim != 1 or y.ndim != 1:
		raise ValueError("fastInterp1 expects 1D sample arrays")
	if x.size != y.size:
		raise ValueError("fastInterp1 expects x and y to have the same length")
	if x.size < 2:
		raise ValueError("fastInterp1 requires at least two samples")

	xq_clamped = np.clip(xq_arr, x[0], x[-1])
	indices = np.searchsorted(x, xq_clamped, side="right")
	indices = np.clip(indices, 1, x.size - 1)

	x0 = x[indices - 1]
	x1 = x[indices]
	y0 = y[indices - 1]
	y1 = y[indices]

	slope = (y1 - y0) / (x1 - x0)
	result = y0 + slope * (xq_clamped - x0)

	if np.isscalar(xq) or np.shape(xq) == ():
		return float(result)
	return result


__all__ = ["fastInterp1"]
