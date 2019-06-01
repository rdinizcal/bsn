bsn_folder=$PWD

mkdir -p libbsn/build
gnome-terminal --working-directory=${bsn_folder}/libbsn/build -- bash -c 'cmake ..; sudo make install;' & sleep 5s

mkdir -p odv/sim/controller/build
gnome-terminal --working-directory=${bsn_folder}/odv/sim/controller/build -- bash -c 'cmake ..; sudo make;'

mkdir -p odv/sim/monitor/build
gnome-terminal --working-directory=${bsn_folder}/odv/sim/monitor/build -- bash -c 'cmake ..; sudo make;'

mkdir -p odv/sim/centralhub/build
gnome-terminal --working-directory=${bsn_folder}/odv/sim/centralhub/build -- bash -c 'cmake ..; sudo make;'

#tasks do sensor de ECG
mkdir -p odv/sim/sensors/ecg/collect/build
gnome-terminal --working-directory=${bsn_folder}/odv/sim/sensors/ecg/collect/build -- bash -c 'cmake ..; sudo make;'

mkdir -p odv/sim/sensors/ecg/filter/build
gnome-terminal --working-directory=${bsn_folder}/odv/sim/sensors/ecg/filter/build -- bash -c 'cmake ..; sudo make;'

mkdir -p odv/sim/sensors/ecg/transfer/build
gnome-terminal --working-directory=${bsn_folder}/odv/sim/sensors/ecg/transfer/build -- bash -c 'cmake ..; sudo make;'


#tasks do sensor Oxímetro
mkdir -p odv/sim/sensors/oximeter/collect/build
gnome-terminal --working-directory=${bsn_folder}/odv/sim/sensors/oximeter/collect/build -- bash -c 'cmake ..; sudo make;'

mkdir -p odv/sim/sensors/oximeter/filter/build
gnome-terminal --working-directory=${bsn_folder}/odv/sim/sensors/oximeter/filter/build -- bash -c 'cmake ..; sudo make;'

mkdir -p odv/sim/sensors/oximeter/transfer/build
gnome-terminal --working-directory=${bsn_folder}/odv/sim/sensors/oximeter/transfer/build -- bash -c 'cmake ..; sudo make;'

#tasks do sensor de pressão
mkdir -p odv/sim/sensors/sphygmomanometer/collect/build
gnome-terminal --working-directory=${bsn_folder}/odv/sim/sensors/sphygmomanometer/collect/build -- bash -c 'cmake ..; sudo make;'

mkdir -p odv/sim/sensors/sphygmomanometer/filter/build
gnome-terminal --working-directory=${bsn_folder}/odv/sim/sensors/sphygmomanometer/filter/build -- bash -c 'cmake ..; sudo make;'

mkdir -p odv/sim/sensors/sphygmomanometer/transfer/build
gnome-terminal --working-directory=${bsn_folder}/odv/sim/sensors/sphygmomanometer/transfer/build -- bash -c 'cmake ..; sudo make;'

#tasks do sensor de temperatura
mkdir -p odv/sim/sensors/thermometer/collect/build
gnome-terminal --working-directory=${bsn_folder}/odv/sim/sensors/thermometer/collect/build -- bash -c 'cmake ..; sudo make;'

mkdir -p odv/sim/sensors/thermometer/filter/build
gnome-terminal --working-directory=${bsn_folder}/odv/sim/sensors/thermometer/filter/build -- bash -c 'cmake ..; sudo make;'

mkdir -p odv/sim/sensors/thermometer/transfer/build
gnome-terminal --working-directory=${bsn_folder}/odv/sim/sensors/thermometer/transfer/build -- bash -c 'cmake ..; sudo make;'


#####


mkdir -p odv/sim/sensors/joker/build
gnome-terminal --working-directory=${bsn_folder}/odv/sim/sensors/joker/build -- bash -c 'cmake ..; sudo make;'
