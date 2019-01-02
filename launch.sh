bsn_folder=/home/rdiniz/projects/seams19/bsn

# Execute thermometer node
gnome-terminal --working-directory=${bsn_folder}/odv/sensornode/configs/thermometer -e 'odsupercomponent --cid=111 --managed=simulation'    & sleep 3s
gnome-terminal --working-directory=${bsn_folder} -e "./odv/sensornode/collector/build/collector --cid=111"                                  & sleep 1s
gnome-terminal --working-directory=${bsn_folder} -e "./odv/sensornode/filter/build/filter --cid=111"                                        & sleep 1s
gnome-terminal --working-directory=${bsn_folder} -e "./odv/sensornode/sender/build/sender --cid=111 --id=8080"                              & sleep 1s
gnome-terminal --working-directory=${bsn_folder} -e "./odv/monitor/resource/build/resource --cid=111"                                       & sleep 1s
gnome-terminal --working-directory=${bsn_folder} -e "./odv/sensornode/controllerAPI/controllerAPIReceiver/build/controllerapireceiver --cid=111 --id=2020" & sleep 1s

# Execute manager node
gnome-terminal --working-directory=${bsn_folder}/odv/control/configs -e 'odsupercomponent --cid=250 --managed=simulation'                   & sleep 3s
gnome-terminal --working-directory=${bsn_folder} -e "./odv/control/sensor/build/sensor --cid=250 --id=2000"                                 & sleep 1s
gnome-terminal --working-directory=${bsn_folder} -e "./odv/control/manager/build/manager --cid=250"                                         & sleep 1s

#gnome-terminal --working-directory=${bsn_folder} -e "./odv/sensornode/controllerAPI/controllerAPISender/build/controllerapisender --cid=111 --id=2000"     & sleep 1s
#gnome-terminal --working-directory=${bsn_folder} -e "./odv/control/actuator/build/actuator --cid=250 --id=2020"                             & sleep 1s




