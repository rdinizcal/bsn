terminal_name=''
SO_name="$(cat /etc/issue)"
echo "${SO_name}"

if [[ $SO_name = *"Deepin"* ]]; then  
  terminal_name='deepin-terminal'
elif [[ $SO_name = *"Ubuntu"* ]]; then
  terminal_name='gnome-terminal'
fi

# ${terminal_name} -e ./odv/sensornode/collector/build/DataCollectorApp --cid=111  &
# ${terminal_name} -e ./odv/sensornode/converter/build/Converter --cid=111 &
# ${terminal_name} -e ./odv/sensornode/filter/build/Filter --cid=111 &

${terminal_name} -e ./odv/centralhub/listener/build/tcp_listenerApp --cid=222 --id=8080 &
${terminal_name} -e ./odv/centralhub/listener/build/tcp_listenerApp --cid=222 --id=8081 &
${terminal_name} -e ./odv/centralhub/listener/build/tcp_listenerApp --cid=222 --id=8082 &

# ${terminal_name} -e ./odv/centralhub/receiver/build/DataTriggeredApp --cid=222 &
