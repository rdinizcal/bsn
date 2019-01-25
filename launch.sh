bsn_folder=$PWD

gnome-terminal --working-directory=${bsn_folder}/odv/sim/ -e 'sudo odsupercomponent --cid=111 --managed=simulation_rt --freq=1'    & sleep 3s

# execute controller node (must be executed first)
gnome-terminal --working-directory=${bsn_folder}/odv/sim/controller/build -e "./controller --cid=111"                              & sleep 1s

# execute monitor node (must be executed first)
gnome-terminal --working-directory=${bsn_folder}/odv/sim/monitor/build -e "./monitor --cid=111"                              & sleep 1s

# execute sensor node
#gnome-terminal --working-directory=${bsn_folder} -e "./odv/sim/sensors/ecg/build/ecg --cid=111"                             & sleep 1s
#gnome-terminal --working-directory=${bsn_folder} -e "./odv/sim/sensors/thermometer/build/thermometer --cid=111"             & sleep 1s
#gnome-terminal --working-directory=${bsn_folder} -e "./odv/sim/sensors/oximeter/build/oximeter --cid=111"                   & sleep 1s
#gnome-terminal --working-directory=${bsn_folder} -e "./odv/sim/sensors/sphygmomanometer/build/bloodpressure --cid=111"      & sleep 1s
${bsn_folder}/odv/sim/sensors/ecg/build/ecg --cid=111                             & sleep 1s
${bsn_folder}/odv/sim/sensors/thermometer/build/thermometer --cid=111             & sleep 1s
${bsn_folder}/odv/sim/sensors/oximeter/build/oximeter --cid=111                   & sleep 1s
${bsn_folder}/odv/sim/sensors/sphygmomanometer/build/bloodpressure --cid=111      & sleep 1s

# execute central hub
gnome-terminal --working-directory=${bsn_folder} -e './odv/sim/centralhub/build/centralhub --cid=111'                       & sleep 30s

# jokers instances to clog the centralhub channel
${bsn_folder}/odv/sim/sensors/joker/build/joker --cid=111 --id=0 & sleep 1s
${bsn_folder}/odv/sim/sensors/joker/build/joker --cid=111 --id=1 & sleep 30s
${bsn_folder}/odv/sim/sensors/joker/build/joker --cid=111 --id=2 & sleep 1s
${bsn_folder}/odv/sim/sensors/joker/build/joker --cid=111 --id=3 & sleep 


