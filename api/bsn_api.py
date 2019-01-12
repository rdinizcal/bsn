from flask import Flask, request, session, jsonify
import requests
import subprocess
import os
import signal
import json
from time import sleep

"""
    This file is used for the remote configuration and execution of the BSN.
    It will be done by the BSN's server, written in NodeJs.
"""

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
main_dir = os.getcwd()

def stop_execution():
    global processes_pids
    # Send a Sigkill sign for all the processes pids
    for pid in processes_pids:
        os.kill(pid, signal.SIGKILL)
        processes_pids = []        

def start_execution(path):
    global processes_pids

    # Initialize odsupercomponent
    configuration_path = os.getcwd()  + '/odv/sim/configs/' + path
    od_process = subprocess.Popen(['odsupercomponent','--cid=111'], stdout=subprocess.PIPE, cwd=configuration_path)
    processes_pids.append(od_process.pid)

    # Initialize sensors
    for command in commands:
        process = subprocess.Popen(command, stdout=subprocess.PIPE)
        processes_pids.append(process.pid)

    # Returns all the pids started
    return processes_pids

# Show all the processes status(if active or dead)
def check_status(pid):
    p = os.popen(' '.join(['ps','-p', str(pid), '-o', 'cmd'])).read()
    if ( p == "CMD\n" ):
        return('inexistent')

    else:        
        if 'defunct' in p:            
            return('zombie')
        else:
            return('active')

# Get all categories and subcategories available
def get_configs():        
    configuration_map = {}
    path = 'odv/sim/configs'
    # Get all folders from configs
    # Each one is a categorie    
    categories = ''
    for path, dirs, files in os.walk(path):        
        categories = dirs
        break

    for categorie in categories:
        # Explore each subfolder to get all subcategories
        new_path = path + '/' + categorie
        configuration_map[categorie] = next(os.walk(new_path))[1]

    # Returns a map associating categorie and subcategories
    return configuration_map

# Check if requested path of configuration exists
def is_configuration_available(path):
    # Check if there is a configuration in specified folder
    path = os.getcwd() + '/odv/sim/configs/' + path + '/configuration'
    return os.path.exists(path)

# Returns all configurations available
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
    path = request.args.get('path')    
    if is_configuration_available(path) is False:
        return 'path error'
    try:        
        processes_pids = start_execution(path)
        print(processes_pids)
        return 'ok'    
    except Exception as e: 
        print(e)
        return 'execution error'

# Stop bsn execution
@app.route('/stop')
def stop():    
    global processes_pids
    try:
        stop_execution()
        return 'ok'
    except Exception as e: 
        print(e)
        processes_pids = []
        return 'error'

# Receive a JSON with the configuration file for BSN
@app.route('/new_conf', methods=['POST'])
def new_conf():
    """
    Used for remote configuration. A Json with all
    the required information will be sent to this route
    """
    try:        
        json_file = request.get_json()
        
        os.chdir(main_dir)
        os.chdir("odv/sim/configs/custom")    
        # Create the new directory inside the user's custom
        os.makedirs(json_file['name'], exist_ok=True)
        os.chdir(json_file['name'])
        
        # Create the configuration file with the content sent
        config = open('configuration', 'w')
        config.write(str(json_file['content']))
        config.close()
        os.chdir(main_dir)
        return 'ok'
    except Exception as e: 
        print(e)
        os.chdir(main_dir)
        return "error"


@app.route('/')
def hello_world():
    return '<h1>Welcome to bsn</h1>'
    
app.run(debug=True)
