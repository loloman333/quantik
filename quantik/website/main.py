from flask import Flask, jsonify, request, send_from_directory
from flask_cors import CORS
import json
from math import floor
import mmap
import os

app = Flask(__name__)
# CORS(app, resources={r"/moveinfo": {"origins": "http://localhost:5000"}})
# CORS(app)

@app.route("/")
def index():
    return send_from_directory(app.static_folder, 'index.html')

@app.route("/moveinfo", methods=['GET', 'POST'])
def moveinfo():
    if request.method == 'POST':
        app.logger.info("post fia di")

        response = {}
                
        response['timestamp'] = request.form.get('timestamp')
        response['error'] = ''
        
        encodings = request.form.getlist('encodings[]')
        app.logger.info(f"We need to find these encodings: {encodings}")
        codes = []
        for enc_list in encodings:
            code_list = []
            for enc in enc_list.split(","):
                if enc == '': continue
                enc = int(enc)
                app.logger.debug(f"Looking for code for state {enc:016X} with board {decode(enc)}")
                code_list.append(getCodeForState(enc))
            codes.append(code_list)
        
        response['codes'] = codes
        
        app.logger.info("return")
        app.logger.info(codes)
        return jsonify(response)
    else:
        return False

def getCodeForState(encoding):
    state = toCanonicalState(decode(encoding))
    level = 16 - sum(_.count(0) for _ in state)
    filename = f'../../data/level{level}.qtk'
    
    with open(filename, 'rb') as f:
        start = 0
        end = os.path.getsize(filename) / 9
        app.logger.debug(f"Looking in file ::: Näm: {filename}; Säz: {os.path.getsize(filename)}")
        while start <= end:
            entry = int((start + end) // 2)
            f.seek(entry * 9)
            entry_state = int.from_bytes(f.read(8), 'little')
            
            found = entry_state - encoding
            if found == 0:
                return int.from_bytes(f.read(1), 'little')
            if found < 0:
                start = entry + 1
            else:
                end = entry - 1

    return -1

def decode(encoding):
    state = [[0 for _ in range(4)] for _ in range(4)]

    for i in range(16):
        remainder = encoding % 9
        encoding //= 9
        x = 3 - (i // 4)
        y = 3 - (i % 4)
        state[x][y] = remainder

    return state

def toCanonicalState(state):
    return state

app.run(debug=True)