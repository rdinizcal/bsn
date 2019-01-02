bsn_folder=$PWD

gnome-terminal --working-directory=${bsn_folder}/odv/sim/ -e 'odsupercomponent --cid=111 --managed=simulation'      & sleep 3s

# execute manager node (must be executed first)
gnome-terminal --working-directory=${bsn_folder} -e "./odv/sim/controller/build/manager --cid=111"                  & sleep 1s

# execute sensor node
gnome-terminal --working-directory=${bsn_folder} -e "./odv/sim/sensors/ecg/build --cid=111"                         & sleep 1s
gnome-terminal --working-directory=${bsn_folder} -e "./odv/sim/sensors/thermometer/build --cid=111"                 & sleep 1s
gnome-terminal --working-directory=${bsn_folder} -e "./odv/sim/sensors/oximeter/build --cid=111"                    & sleep 1s
gnome-terminal --working-directory=${bsn_folder} -e "./odv/sim/sensors/shygmomanometer/build --cid=111"             & sleep 1s

# execute central hub
gnome-terminal --working-directory=${bsn_folder} -e './odv/sim/centralhub/build/centralhub --cid=111'               & sleep 1s




