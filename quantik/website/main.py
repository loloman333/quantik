from flask import Flask, jsonify, request, send_from_directory
# from flask_cors import CORS
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
        response = {}
                
        response['timestamp'] = request.form.get('timestamp')
        response['error'] = ''
        
        encodings = request.form.getlist('encodings[]')
        
        cleaned_encodings = []
        for encoding in encodings:
            encoding = -1 if encoding == '' else int(encoding)
            cleaned_encodings.append(encoding)                                        
        
        codes = getCodesForEncodings(cleaned_encodings)
        response['codes'] = codes
        
        return jsonify(response)
    else:
        return False

def getCodesForEncodings(encodings):
    if len(encodings) == 0: return []
    
    level = 16 - sum(_.count(0) for _ in decode(encodings[0])) 
    filename = f'../data/level{level}.qtk'
    
    codes = []
    with open(filename, 'rb') as f:
        for encoding in encodings:
            
            if encoding == -1: 
                codes.append(-1)
                continue
            
            start = 0
            end = int(os.path.getsize(filename) / 9)
            
            found = 1
            round = 0
            while found != 0 and start <= end:
                round += 1
                entry = int((start + end) // 2)
                f.seek(entry * 9)
                
                entry_state = int.from_bytes(f.read(8), 'little')
                found = entry_state - encoding
                
                if found == 0:
                    codes.append(int.from_bytes(f.read(1), 'little'))
                elif found < 0:
                    start = entry + 1
                else:
                    end = entry - 1
                    
            if (start > end): 
                codes.append(-1)
    
    return codes

def decode(encoding):
    state = [[0 for _ in range(4)] for _ in range(4)]

    for i in range(16):
        remainder = encoding % 9
        encoding //= 9
        x = 3 - (i // 4)
        y = 3 - (i % 4)
        state[x][y] = remainder

    return state

app.run(debug=True)