bsn_folder=/home/rdiniz/projects/seams19/bsn

mkdir -p libbsn/build
gnome-terminal --working-directory=${bsn_folder}/libbsn/build -- bash -c 'cmake ..; sudo make install; bash'

mkdir -p odv/sensornode/collector/build
gnome-terminal --working-directory=${bsn_folder}/odv/sensornode/collector/build -- bash -c 'cmake ..; sudo make; bash'
mkdir -p odv/sensornode/controllerAPI/controllerAPIReceiver/build
gnome-terminal --working-directory=${bsn_folder}/odv/sensornode/controllerAPI/controllerAPIReceiver/build -- bash -c 'cmake ..; sudo make; bash'
mkdir -p odv/sensornode/controllerAPI/controllerAPISender/build
gnome-terminal --working-directory=${bsn_folder}/odv/sensornode/controllerAPI/controllerAPISender/build-- bash -c 'cmake ..; sudo make; bash'
mkdir -p odv/sensornode/filter/build
gnome-terminal --working-directory=${bsn_folder}/odv/sensornode/filter/build -- bash -c 'cmake ..; sudo make; bash'
mkdir -p odv/sensornode/sender/build
gnome-terminal --working-directory=${bsn_folder}/odv/sensornode/sender/build -- bash -c 'cmake ..; sudo make; bash'


mkdir -p odv/centralhub/listener/build
gnome-terminal --working-directory=${bsn_folder}/odv/centralhub/listener/build -- bash -c 'cmake ..; sudo make; bash'
mkdir -p odv/centralhub/bsn/processor/build
gnome-terminal --working-directory=${bsn_folder}/odv/centralhub/processor/build -- bash -c 'cmake ..; sudo make; bash'

mkdir -p odv/monitor/resource/build
gnome-terminal --working-directory=${bsn_folder}/odv/monitor/resource/build -- bash -c 'cmake ..; sudo make; bash'

mkdir -p odv/persister/txt/build
gnome-terminal --working-directory=${bsn_folder}/odv/persister/txt/build -- bash -c 'cmake ..; sudo make; bash'


