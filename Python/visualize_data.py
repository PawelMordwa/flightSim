import argparse
import matplotlib.pyplot as plt
import pandas as pd
import os

parser = argparse.ArgumentParser(description="Script for visualization CSV file.")

parser.add_argument(
    '--path',
    type=str,
    default="../build/sim_output.csv",
)

args = parser.parse_args()

path = args.path

data = pd.read_csv(path)

t_s = data["t_s"]

# Create subplots and layout
fig, axes = plt.subplots(2, 4, figsize=(10,6))
fig.set_facecolor('black')

# Axial velocity u^b_CM/n
axes[0, 0].plot(t_s, data["u"], color='yellow')
axes[0, 0].set_xlabel('Time [s]', color='white')
axes[0, 0].set_ylabel('u [m/s]', color='white')
axes[0, 0].set_facecolor('black')
axes[0, 0].tick_params(colors='white', labelcolor='white')

# Axial velocity v^b_CM/n
axes[0, 1].plot(t_s, data["v"], color='yellow')
axes[0, 1].set_xlabel('Time [s]', color='white')
axes[0, 1].set_ylabel('v [m/s]', color='white')
axes[0, 1].set_facecolor('black')
axes[0, 1].tick_params(colors='white', labelcolor='white')

# Axial velocity w_b_CM/n
axes[0, 2].plot(t_s, data["w"], color='yellow')
axes[0, 2].set_xlabel('Time [s]', color='white')
axes[0, 2].set_ylabel('w [m/s]', color='white')
axes[0, 2].set_facecolor('black')
axes[0, 2].tick_params(colors='white', labelcolor='white')

# Roll angle phi
axes[0, 3].plot(t_s, data["phi"], color='yellow')
axes[0, 3].set_xlabel('Time [s]', color='white')
axes[0, 3].set_ylabel('phi [rad]', color='white')
axes[0, 3].set_facecolor('black')
axes[0, 3].tick_params(colors='white', labelcolor='white')

# Roll rate p^b
axes[1, 0].plot(t_s, data["p"], color='yellow')
axes[1, 0].set_xlabel('Time [s]', color='white')
axes[1, 0].set_ylabel('p [rad/s]', color='white')
axes[1, 0].set_facecolor('black')
axes[1, 0].tick_params(colors='white', labelcolor='white')

# Roll rate q^b
axes[1, 1].plot(t_s, data["q"], color='yellow')
axes[1, 1].set_xlabel('Time [s]', color='white')
axes[1, 1].set_ylabel('q [rad/s]', color='white')
axes[1, 1].set_facecolor('black')
axes[1, 1].tick_params(colors='white', labelcolor='white')

# Roll rate r^b
axes[1, 2].plot(t_s, data["r"], color='yellow')
axes[1, 2].set_xlabel('Time [s]', color='white')
axes[1, 2].set_ylabel('r [rad/s]', color='white')
axes[1, 2].set_facecolor('black')
axes[1, 2].tick_params(colors='white', labelcolor='white')

# Pitch angle, theta
axes[1, 3].plot(t_s, data["theta"], color='yellow')
axes[1, 3].set_xlabel('Time [s]', color='white')
axes[1, 3].set_ylabel('theta [rad]', color='white')
axes[1, 3].set_facecolor('black')
axes[1, 3].tick_params(colors='white', labelcolor='white')

plt.tight_layout()
os.makedirs('fig', exist_ok=True)
plt.savefig('fig/sim.png')
plt.show()