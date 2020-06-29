PTFE_NEVENTS=2000000
PTFE_RANDOMSEED=100
PTFE_VISUAL=0

export PTFE_ANAROOT="sim.root"
export PTFE_SURFTYPE="3"
export PTFE_DEPTH="10" #nm
export PTFE_ONSURFACE="1" #nm



if [ "$GAL" == "/Users/sdporzio/MOUNT/GALILEO" ]
then
  source /Users/sdporzio/Software/geant4.10.06.p01/install/bin/geant4.sh
else
  source /cvmfs/lz.opensciencegrid.org/external/Geant4/10.06.p01/x86_64-centos7-gcc8-opt/bin/geant4.sh
fi
cd build
cmake ../
make -j 4 && ./main ${PTFE_NEVENTS} ${PTFE_RANDOMSEED} ${PTFE_VISUAL}
