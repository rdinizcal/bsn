bsn_folder=$PWD

gnome-terminal --working-directory=${bsn_folder}/odv/sim/ -e 'sudo odsupercomponent --cid=111 --managed=simulation_rt --freq=1'      & sleep 3s

<<<<<<< HEAD
# execute manager node (must be executed first)
gnome-terminal --working-directory=${bsn_folder}/odv/sim/controller/build -e "./manager --cid=111"                          & sleep 1s
=======
# execute controller node (must be executed first)
gnome-terminal --working-directory=${bsn_folder}/odv/sim/controller/build -e "./controller --cid=111"                           & sleep 1s
>>>>>>> 5774d7c... manager -> controller refactoring

# execute sensor node
gnome-terminal --working-directory=${bsn_folder} -e "./odv/sim/sensors/ecg/build/ecg --cid=111"                              & sleep 1s
gnome-terminal --working-directory=${bsn_folder} -e "./odv/sim/sensors/thermometer/build/thermometer --cid=111"              & sleep 1s
gnome-terminal --working-directory=${bsn_folder} -e "./odv/sim/sensors/oximeter/build/oximeter --cid=111"                    & sleep 1s
gnome-terminal --working-directory=${bsn_folder} -e "./odv/sim/sensors/sphygmomanometer/build/bloodpressure --cid=111"       & sleep 1s


# execute central hub
gnome-terminal --working-directory=${bsn_folder} -e './odv/sim/centralhub/build/centralhub --cid=111'                        & sleep 1s




