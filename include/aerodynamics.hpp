struct AeroForces {
  float drag_N;
  float side_N;
  float lift_N;
};

struct BodyForces {
  float Fx_N;
  float Fy_N;
  float Fz_N;
};

AeroForces aero_forces(float q_bar, float Aref_m2, float CD, float CY,
                       float CL);
BodyForces wind_to_body(AeroForces f, float alpha_rad, float beta_rad);