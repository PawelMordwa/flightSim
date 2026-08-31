import math
import numpy as np

from tools.Interpolations import fastInterp1

def flat_earth_eom(t, x, vmod, amod):
    """
    Arguments
    t - time [s]
    x - state vector at time t
    x[0]  = u_b_mps
    x[1]  = v_b_mps
    x[2]  = w_b_mps
    x[3]  = p_b_rps
    x[4]  = q_b_rps
    x[5]  = r_b_rps
    x[6]  = phi_rad
    x[7]  = theta_rad
    x[8]  = psi_rad
    x[9]  = px_n_m 
    x[10] = py_n_m
    x[11] = pz_n_m
    
    Return:
        dx
    """

    dx = np.zeros(12)



    # Asign curent state to individual variables
    u_b_mps = x[0]
    v_b_mps = x[1]
    w_b_mps = x[2]
    p_b_rps = x[3]
    q_b_rps = x[4]
    r_b_rps = x[5]
    phi_rad = x[6]
    theta_rad = x[7]
    psi_rad = x[8]
    px_n_m = x[9]
    py_n_m = x[10]
    pz_n_m = x[11]

    # Euler angles trigonometric functions
    c_phi = math.cos(phi_rad)
    c_theta =math.cos(theta_rad) 
    c_psi = math.cos(psi_rad)
    s_phi = math.sin(phi_rad)
    s_theta =math.sin(theta_rad)
    s_psi = math.sin(psi_rad)
    t_theta =math.tan(theta_rad) 

    # Asign phisycal properties from aircraft model
    m_kg = vmod["m_kg"]
    Jxz_b_kgm2 = vmod["Jxz_b_kgm2"]
    Jxx_b_kgm2 = vmod["Jxx_b_kgm2"]
    Jyy_b_kgm2 = vmod["Jyy_b_kgm2"]
    Jzz_b_kgm2 = vmod["Jzz_b_kgm2"]

    # Altitude
    h_m = -pz_n_m

    # Atmosphere model
    rho_interp_kgpm3 = fastInterp1(amod["alt_m"], amod["rho_kgpm3"], h_m)
    c_interp_mps = fastInterp1(amod["alt_m"], amod["c_mps"], h_m)

    # Air data
    tru_airspeed_mps = math.sqrt(u_b_mps**2 + v_b_mps**2 + w_b_mps**2)
    qbar_kgmps2 = 0.5*rho_interp_kgpm3*tru_airspeed_mps**2

    if tru_airspeed_mps == 0:
        v_over_VT = 0
    else:
        v_over_VT = max(-1.0, min(1.0, v_b_mps/tru_airspeed_mps))

    alpha_rad = math.atan2(w_b_mps, u_b_mps)
    beta_rad = math.asin(v_over_VT)
    s_alpha = math.sin(alpha_rad)
    c_alpha = math.cos(alpha_rad)
    s_beta = math.sin(beta_rad)
    c_beta = math.cos(beta_rad)

    # Gravity (can interp)
    gz_n_mps2 = fastInterp1(amod["alt_m"], amod["g_mps"], h_m)

    # Gravity in body cordinate system
    gx_b_mps2 = -math.sin(theta_rad) * gz_n_mps2
    gy_b_mps2 = math.sin(phi_rad) * math.cos(theta_rad) * gz_n_mps2
    gz_b_mps2 = math.cos(phi_rad) * math.cos(theta_rad) * gz_n_mps2

    # Aero force
    drag_N = vmod["CD_approx"]*qbar_kgmps2*vmod["Aref_m2"]
    side_N = 0
    lift_N = 0    

    # External forces
    Fx_b_N = -(c_alpha*c_beta*drag_N - c_alpha*s_beta*side_N - s_alpha*lift_N)
    Fy_b_N = -(s_beta*drag_N + c_beta*side_N)
    Fz_b_N = -(s_alpha*c_beta*drag_N - s_alpha*s_beta*side_N + c_alpha*lift_N)

    # External moments
    L_b_Nm = 0
    M_b_Nm = 0
    N_b_Nm = 0

    # Denominator in roll and yaw rate equations
    den = Jxx_b_kgm2 * Jzz_b_kgm2 - Jxz_b_kgm2**2

    # Translational equations
    dx[0] = 1/m_kg * Fx_b_N + gx_b_mps2 - w_b_mps * q_b_rps + v_b_mps * r_b_rps
    dx[1] = 1/m_kg * Fy_b_N + gy_b_mps2 - u_b_mps * r_b_rps + w_b_mps * p_b_rps
    dx[2] = 1/m_kg * Fz_b_N + gz_b_mps2 - v_b_mps * p_b_rps + u_b_mps * q_b_rps

    # Rotational equations

    dx[3] = (Jxz_b_kgm2 * (Jxx_b_kgm2- Jyy_b_kgm2 + Jzz_b_kgm2) * p_b_rps * q_b_rps - \
            (Jzz_b_kgm2 * (Jzz_b_kgm2 - Jyy_b_kgm2) + Jxz_b_kgm2**2) * q_b_rps * r_b_rps + \
            Jzz_b_kgm2 * L_b_Nm + Jxz_b_kgm2 * N_b_Nm ) / den
    dx[4] = ((Jzz_b_kgm2 - Jxx_b_kgm2) * r_b_rps * p_b_rps - Jxz_b_kgm2 * (p_b_rps**2 - r_b_rps**2) + M_b_Nm) / Jyy_b_kgm2
    dx[5] = (-Jxz_b_kgm2 * (Jxx_b_kgm2- Jyy_b_kgm2 + Jzz_b_kgm2) * q_b_rps * r_b_rps + \
            (Jxx_b_kgm2 * (Jxx_b_kgm2 - Jyy_b_kgm2) + Jxz_b_kgm2**2) * p_b_rps * q_b_rps + \
            Jxz_b_kgm2 * L_b_Nm + Jxx_b_kgm2 * N_b_Nm ) / den
    
    # Kinematic equations
    dx[6] = p_b_rps + math.sin(phi_rad) * math.tan(theta_rad) * q_b_rps + math.cos(phi_rad) * math.tan(theta_rad) * r_b_rps
    dx[7] = math.cos(phi_rad) * q_b_rps - math.sin(phi_rad) * r_b_rps
    dx[8] = math.sin(phi_rad)/math.cos(theta_rad) * q_b_rps + math.cos(phi_rad)/math.cos(theta_rad) * r_b_rps

    # Position equations
    dx[9] = c_theta*c_psi*u_b_mps + (-c_phi*s_psi + s_phi*s_theta*c_psi)*v_b_mps + (s_phi*s_psi + c_phi*s_theta*c_psi)*w_b_mps
    dx[10] = c_theta*s_psi*u_b_mps + (c_phi*c_psi + s_phi*s_theta*s_psi)*v_b_mps + (-s_phi*c_psi + c_phi*s_theta*s_psi)*w_b_mps
    dx[11] = -s_theta*u_b_mps + s_phi*c_theta*v_b_mps + c_phi*c_theta*w_b_mps

    return dx