import numpy as np
import pandas as pd
from scipy.interpolate import interp1d

# Load data for drift velocity
vel = pd.read_csv('aux/drift.csv')
fVel = interp1d(vel['efield'], vel['drift'], kind='linear', fill_value='extrapolate')

def EleField(rho, deltaV=1500, r_c=20, r_a=0.25):
    # deltaV in V
    # All distances in mm
    # Return V/mm
    return deltaV/(np.log(r_c/r_a)*rho)

def To_Td(efield):
  # efield in V/cm
  n0 = 2.687*1e19 # cm-3
  return (efield/n0)*1e17

def To_Vcm(td):
  # td in Townsend
  n0 = 2.687*1e19 # cm-3
  return (td*n0)/1e17

def EleDriftVelocity(rho, deltaV=1500):
  # Distance (rho) in mm
  # Velocity in mm/us

  # Determine electric field at location rho
  efield = EleField(rho,deltaV=deltaV)
  # Convert to V/cm then to td
  efield_td = To_Td(efield*10)
  # Extract interpolated value in cm/s
  drift_cms = fVel(efield_td)
  # Convert to mm/us
  drift_mmus = drift_cms*1e-5
  return drift_mmus

def EleDriftTime(rho, deltaV=1500, step=0.05, r_a=0.25):
  # Distance (rho) in mm
  # Time in us
  rho_steps = np.arange(rho,r_a,-step)

  time = 0
  for r in rho_steps:
      speed = EleDriftVelocity(r,deltaV=deltaV)
      this_time = step/speed
      time += this_time
  return time
