export MG4_NEVENTS=100
export MG4_RANDOMSEED=100
export MG4_VISUAL=1
export MG4_HOLETYPE=3
export MG4_HOLEWIDTH=1
# export MG4_OUTDIR="/global/project/projectdirs/lz/users/sdporzio/Data/AlphaSourceVUVLamp"
export MG4_OUTDIR="/Users/sdporzio/Data/Temp"
export MG4_FILENAME="${MG4_HOLEWIDTH}-w_${MG4_NEVENTS}-ev_${MG4_RANDOMSEED}-s.root"


if [ "$GAL" == "/Users/sdporzio/MOUNT/GALILEO" ]
then
  source /Users/sdporzio/Software/geant4.10.06.p01/install/bin/geant4.sh
else
  source /cvmfs/lz.opensciencegrid.org/external/Geant4/10.06.p01/x86_64-centos7-gcc8-opt/bin/geant4.sh
fi
cd build
cmake ../
make -j 4 && ./main ${MG4_NEVENTS} ${MG4_RANDOMSEED} ${MG4_VISUAL}
