import matplotlib.pyplot as plt
import numpy as np
import math
import os
import ussa1976

from governing_equations import flat_earth_eom
from numerical_integration import numerical_integration
from tools.Interpolators import fastInterp1
from vehicle_models.sphere import spheres

# PART 1 Initialization simulation

# Athmospheric data
atmosphere = ussa1976.compute()

alt_m     = atmosphere["z"].values
rho_kgpm3 = atmosphere["rho"].values
c_mps     = atmosphere["cs"].values
g_mps     = ussa1976.core.compute_gravity(alt_m)

amod = { 
    "alt_m" : alt_m, \
    "rho_kgpm3" : rho_kgpm3, \
    "c_mps" : c_mps, \
    "g_mps" : g_mps
}

# Define aircraft model
vmod = spheres.BowlingBall()

# Initial conditions
u_b_mps   = 0.001
v_b_mps   = 0
w_b_mps   = 0
p_b_rps   = 0
q_b_rps   = 0
r_b_rps   = 0
phi_rad   = 0*math.pi/180
theta_rad = -90*math.pi/180
psi_rad   = 0
px_n_m    = 0
py_n_m    = 0
pz_n_m    = -5000

x0 = np.array([
    u_b_mps,
    v_b_mps,
    w_b_mps,
    p_b_rps,
    q_b_rps,
    r_b_rps,
    phi_rad,
    theta_rad,
    psi_rad,
    px_n_m,
    py_n_m,
    pz_n_m,
])

nx0 = x0.size

# Simulation time
t0_s = 0
tf_s = 50
ts_s = 0.005

# PART 2: Iterate over governing equations

# Prepare solution array
t_s = np.arange( t0_s, tf_s + ts_s, ts_s); nt_s = t_s.size
x = np.zeros((nx0, nt_s))
x[:,0] = x0

# Numerical integration
t_s, x = numerical_integration.forward_euler(flat_earth_eom.flat_earth_eom, t_s, x, ts_s, vmod, amod)


# Data post-processing
True_Airspeed_mps = np.zeros((nt_s,1))

for i, element in enumerate(t_s):
    True_Airspeed_mps[i,0] = math.sqrt(x[0,i]**2 + x[1,i]**2 + x[2,i]**2)

Altitude_m = np.zeros((nt_s,1))
Cs_mps = np.zeros((nt_s,1))
Rho_kgpm3 = np.zeros((nt_s,1))

for i, element in enumerate(t_s):
    Altitude_m[i,0] = -x[11,i]
    Cs_mps[i,0] = fastInterp1(amod["alt_m"], amod["c_mps"], Altitude_m[i,0])
    Rho_kgpm3[i,0] = fastInterp1(amod["alt_m"], amod["rho_kgpm3"], Altitude_m[i,0])

Alpha_rad = np.zeros((nt_s,1))

for i, element in enumerate(t_s):
    Alpha_rad[i,0] = math.atan2(x[2,i], x[0,i])

Beta_rad = np.zeros((nt_s,1))

for i, element in enumerate(t_s):
    if True_Airspeed_mps[i,0] == 0:
        v_over_VT = 0
    else:
        v_over_VT = max(-1.0, min(1.0, x[1,i]/True_Airspeed_mps[i,0]))

    Beta_rad[i,0] = math.asin(v_over_VT)

Mach = np.zeros((nt_s,1))

for i, element in enumerate(t_s):
    Mach[i,0] = True_Airspeed_mps[i,0]/Cs_mps[i,0]


# PART 3: Plot results

# Create subplots and layout
fig, axes = plt.subplots(2, 4, figsize=(10,6))
fig.set_facecolor('black')

# Axial velocity u^b_CM/n
axes[0, 0].plot(t_s, x[0,:], color='yellow')
axes[0, 0].set_xlabel('Time [s]', color='white')
axes[0, 0].set_ylabel('u [m/s]', color='white')
axes[0, 0].set_facecolor('black')
axes[0, 0].tick_params(colors='white', labelcolor='white')

# Axial velocity v^b_CM/n
axes[0, 1].plot(t_s, x[1,:], color='yellow')
axes[0, 1].set_xlabel('Time [s]', color='white')
axes[0, 1].set_ylabel('v [m/s]', color='white')
axes[0, 1].set_facecolor('black')
axes[0, 1].tick_params(colors='white', labelcolor='white')

# Axial velocity w_b_CM/n
axes[0, 2].plot(t_s, x[2,:], color='yellow')
axes[0, 2].set_xlabel('Time [s]', color='white')
axes[0, 2].set_ylabel('w [m/s]', color='white')
axes[0, 2].set_facecolor('black')
axes[0, 2].tick_params(colors='white', labelcolor='white')

# Roll angle phi
axes[0, 3].plot(t_s, x[6,:], color='yellow')
axes[0, 3].set_xlabel('Time [s]', color='white')
axes[0, 3].set_ylabel('phi [rad]', color='white')
axes[0, 3].set_facecolor('black')
axes[0, 3].tick_params(colors='white', labelcolor='white')

# Roll rate p^b
axes[1, 0].plot(t_s, x[3,:], color='yellow')
axes[1, 0].set_xlabel('Time [s]', color='white')
axes[1, 0].set_ylabel('p [rad/s]', color='white')
axes[1, 0].set_facecolor('black')
axes[1, 0].tick_params(colors='white', labelcolor='white')

# Roll rate q^b
axes[1, 1].plot(t_s, x[4,:], color='yellow')
axes[1, 1].set_xlabel('Time [s]', color='white')
axes[1, 1].set_ylabel('q [rad/s]', color='white')
axes[1, 1].set_facecolor('black')
axes[1, 1].tick_params(colors='white', labelcolor='white')

# Roll rate r^b
axes[1, 2].plot(t_s, x[5,:], color='yellow')
axes[1, 2].set_xlabel('Time [s]', color='white')
axes[1, 2].set_ylabel('r [rad/s]', color='white')
axes[1, 2].set_facecolor('black')
axes[1, 2].tick_params(colors='white', labelcolor='white')

# Pitch angle, theta
axes[1, 3].plot(t_s, x[7,:], color='yellow')
axes[1, 3].set_xlabel('Time [s]', color='white')
axes[1, 3].set_ylabel('theta [rad]', color='white')
axes[1, 3].set_facecolor('black')
axes[1, 3].tick_params(colors='white', labelcolor='white')

plt.tight_layout()
os.makedirs('fig', exist_ok=True)
plt.savefig('fig/sim.png')
plt.show()