from flask import Flask, request, session, jsonify
import subprocess
import os
import signal
from time import sleep

commands = [
    ['./odv/sim/controller/build/manager', '--cid=111'],
    ['./odv/sim/sensors/ecg/build/ecg', '--cid=111'],
    ['./odv/sim/sensors/thermometer/build/thermometer', '--cid=111'],
    ['./odv/sim/sensors/oximeter/build/oximeter', '--cid=111'],
    ['./odv/sim/sensors/sphygmomanometer/build/bloodpressure', '--cid=111'],
    ['./odv/sim/centralhub/build/centralhub', '--cid=111']
]

app = Flask(__name__)
processes_pids = []


def stop_execution():
    global processes_pids

    for pid in processes_pids:
        os.kill(pid, signal.SIGKILL)
        processes_pids = []        

def start_execution():
    global processes_pids

    # Initialize odsupercomponent
    configuration_path = os.getcwd()  + '/odv/sim'    
    od_process = subprocess.Popen(['odsupercomponent','--cid=111'], stdout=subprocess.PIPE, cwd=configuration_path)
    processes_pids.append(od_process.pid)

    # Initialize sensors
    for command in commands:
        process = subprocess.Popen(command, stdout=subprocess.PIPE)
        processes_pids.append(process.pid)

    # Returns all the pids started
    return processes_pids

def check_status(pid):
    p = os.popen(' '.join(['ps','-p', str(pid), '-o', 'cmd'])).read()
    if ( p == "CMD\n" ):
        return('inexistent')

    else:        
        if 'defunct' in p:            
            return('zombie')
        else:
            return('active')

# Get all categories available
def get_configs():
    configuration_map = {}
    path = 'odv/sim/configs'
    # Get all folders from configs
    # Each one is a categorie
    categories = next(os.walk(path))[1]

    for categorie in categories:
        # Explore each subfolder to get all subcategories
        new_path = path + '/' + categorie
        configuration_map[categorie] = next(os.walk(new_path))[1]

    # Returns a map associating categorie and subcategories
    return configuration_map

@app.route('/config')
def config():    
    return jsonify(get_configs())

# Show active processes
@app.route('/status')
def status():
    global processes_pids

    if processes_pids == []:
        return 'Inactive'

    response = ''
    for pid in processes_pids:
        response += str(pid)  + check_status(pid) + '<br>'

    return response

# Start bsn execution
@app.route('/start')
def start():
    global processes_pids
    try:
        processes_pids = start_execution()
        print(processes_pids)
        return 'ok'    
    except:
        return 'error'

# Sop bsn execution
@app.route('/stop')
def stop():    
    global processes_pids
    try:
        stop_execution()
        return 'ok'
    except:
        processes_pids = []
        return 'error'

@app.route('/')
def hello_world():
    return '<h1>Welcome to bsn</h1>'
    
app.run(debug=True)