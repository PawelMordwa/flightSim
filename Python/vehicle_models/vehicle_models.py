"""Simple vehicle model definitions used by the simulation."""

from __future__ import annotations

from dataclasses import dataclass
import math


@dataclass(frozen=True)
class BowlingBall:
	"""A compact spherical body model with dictionary-style access."""

	m_kg: float = 6.0
	radius_m: float = 0.1085
	CD_approx: float = 0.47

	def __post_init__(self):
		object.__setattr__(self, "Aref_m2", math.pi * self.radius_m**2)
		inertia = 2.0 / 5.0 * self.m_kg * self.radius_m**2
		object.__setattr__(self, "Jxx_b_kgm2", inertia)
		object.__setattr__(self, "Jyy_b_kgm2", inertia)
		object.__setattr__(self, "Jzz_b_kgm2", inertia)
		object.__setattr__(self, "Jxz_b_kgm2", 0.0)

	def __getitem__(self, key):
		return getattr(self, key)

	def to_dict(self):
		return {
			"m_kg": self.m_kg,
			"radius_m": self.radius_m,
			"CD_approx": self.CD_approx,
			"Aref_m2": self.Aref_m2,
			"Jxx_b_kgm2": self.Jxx_b_kgm2,
			"Jyy_b_kgm2": self.Jyy_b_kgm2,
			"Jzz_b_kgm2": self.Jzz_b_kgm2,
			"Jxz_b_kgm2": self.Jxz_b_kgm2,
		}


class spheres:
	BowlingBall = BowlingBall


__all__ = ["BowlingBall", "spheres"]
