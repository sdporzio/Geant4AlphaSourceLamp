export MG4_NEVENTS=100000
export MG4_RANDOMSEED=100
export MG4_VISUAL=0
export MG4_HOLETYPE=${1}
export MG4_HOLEWIDTH=${2}
export MG4_OUTDIR="/global/project/projectdirs/lz/users/sdporzio/Data/AlphaSourceVUVLamp"
export MG4_FILENAME="t-${MG4_HOLETYPE}_w-${MG4_HOLEWIDTH}_ev-${MG4_NEVENTS}_s-${MG4_RANDOMSEED}.root"

# Shifter
# /usr/bin/shifter --module=cvmfs --image=luxzeplin/offline_hosted:centos7 bash --rcfile ~/.bashrc.ext

# Baccarat
cd /global/project/projectdirs/lz/users/sdporzio/Software/Baccarat_5.2.7_WireReflectivity/baccarat
source setup.sh
cd /global/project/projectdirs/lz/users/sdporzio/AlphaSourceLamp

# Geant4
source /cvmfs/lz.opensciencegrid.org/external/Geant4/10.06.p01/x86_64-centos7-gcc8-opt/bin/geant4.sh

cd build
log "./main ${MG4_NEVENTS} ${MG4_RANDOMSEED} ${MG4_VISUAL}"
