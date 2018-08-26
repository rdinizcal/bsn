terminal_name=''
SO_name="$(cat /etc/issue)"
echo "${SO_name}"

if [[ $SO_name = *"Deepin"* ]]; then
  echo "It's Deepin!"
  terminal_name='deepin-terminal'
elif [[ $SO_name = *"Ubuntu"* ]]; then
  echo "It's Garbage!"
  terminal_name='gnome-terminal'
elif [[ $SO_name = *"Mint"* ]]; then
  echo "It's Garbage!"
  terminal_name='gnome-terminal'
fi

${terminal_name} -e ./odv/sensornode/collector/build/DataCollectorApp --cid=111  &
${terminal_name} -e ./odv/sensornode/converter/build/Converter --cid=111 &
${terminal_name} -e ./odv/sensornode/filter/build/Filter --cid=111 &
