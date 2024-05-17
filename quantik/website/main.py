from flask import Flask, jsonify, request, send_from_directory
from flask_cors import CORS
import json
from math import floor
import mmap
import os

app = Flask(__name__)
CORS(app)

@app.route("/")
def index():
    return send_from_directory(app.static_folder, 'index.html')

@app.route("/moveinfo", methods=['GET', 'POST'])
def hello_world():
    if request.method == 'POST':
        data = json.loads(request.data)
        states = data['states']
        timestamp = data['timestamp']

        moveInfos = []
        
        response = {}
        response['error'] = ''
        response['timestamp'] = timestamp
        
        for state in states:
            moveInfos.append(getCodeForState(state))
        
        response['moveInfos'] = moveInfos
        return jsonify(response)
    else:
        return False

def getCodeForState(encoding):
    state = decode(encoding)
    level = 16 - sum(_.count(0) for _ in state)
    filename = f'level{level}.qtk'
    
    with open(filename, 'rb') as file:
        # file = mmap.mmap()
        pass
    
    with open(filename, 'rb') as f:
        start = 0
        end = os.path.getsize(filename) / 9
        while start <= end:
            entry = int((start + end) // 2)
            f.seek(entry * 9)
            entry_state = int.from_bytes(f.read(8), 'big')
            
            found = entry_state - encoding
            if found == 0:
                return int.from_bytes(f.read(1), 'big')
            if found < 0:
                if start == end - 1:
                    start = end
                else:
                    start = entry
            else:
                end = entry
    
    assert False
    return -1

def decode(encoding):
    state = [[0] * 4] * 4

    for i in range(16):
        remainder = encoding % 9
        encoding /= 9
        x = i // 4
        y = i % 4
        state[x][y] = remainder

    return state

# Not Mine
def loadClassificationOfState(state, rows, cols):
    find_entry = 0
    filename = 'data/' + rows + 'x' + cols
    with open(filename, 'rb') as f:
        file = mmap.mmap(f.fileno(), 0)
        start = 0
        end = os.path.getsize(filename) / 9
        while start != end:
            entry = floor(start + end) / 2
            entry_state = 0
            for byte in range(8):
                entry_state << 8
                entry_state |= file[entry * 9 + byte]
            find_entry = entry_state - state
            if find_entry == 0:
                classification = file[entry * 9 + 8]
                file.close()
                return classification
            if find_entry < 0:
                if start == end - 1:
                    start = end
                else:
                    start = entry
            else:
                end = entry
    file.close()
    return 0

app.run(debug=True)