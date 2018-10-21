mkdir -p libbsn/build
mkdir -p odv/sensornode/collector/build
mkdir -p odv/centralhub/processor/build
mkdir -p odv/sensornode/sender/build
mkdir -p odv/sensornode/filter/build
mkdir -p odv/centralhub/processor/build
mkdir -p odv/monitor/resource/build
mkdir -p odv/persister/txt/build
mkdir -p odv/sensornode/controller/build
python3 make_generator.py
make