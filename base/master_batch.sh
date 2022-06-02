#! bin/zsh

Root_geant4_data_dir="/data/liontea/sonde/geant4"
Root_geant4_date=`date +%Y-%m-%d`
cores="10"
Nps="100000000"
position="100"

sh ./makeAndExecute.sh ${Root_geant4_data_dir} ${Root_geant4_date} ${cores} ${Nps} ${position}
