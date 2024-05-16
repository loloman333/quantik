from flask import Flask, jsonify, request
from flask_cors import CORS
import json
from math import comb, floor
import mmap
import os


app = Flask(__name__)
CORS(app)

# TODO: mmap file (equivalent to random access file)
@app.route("/", methods=['GET', 'POST'])
def hello_world():
    if request.method == 'POST':
        data = json.loads(request.data)
        states = data['states']
        timestamp = data['timestamp']

        moveInfos = []
        response['error'] = ''
        response['timestamp'] = timestamp
        try:
            nextStates = getNextStates(board, rows, cols, base)
            for state in nextStates:
                moveInfos.append(loadClassificationOfState(state, rows, cols))
        except Exception as e:
            response['error'] = "Can't handle request, exception occured: " + e
        response = {}
        response['moveInfos'] = moveInfos
        return jsonify(response)
    else:
        return

def getNextStates(state, rows, cols, base):
    states = []
    board = decode(state, rows, cols, base)
    if state < pow(base, cols):
        for i in range(cols):
            if board[i][rows - 1] == 2:
                return []
        for i in range(cols):
            for j in range(1, rows):
                if board[i][j] == 1:
                    if i > 0 and board[i - 1][j - 1] == 2:
                        board[i][j] = 0
                        board[i - 1][j - 1] = 1
                        states.append(pow(base, cols) + encode(board, rows, cols, base))
                        board[i][j] = 1
                        board[i - 1][j - 1] = 2
                    if board[i][j - 1] == 0:
                        board[i][j] = 0
                        board[i][j - 1] = 1
                        states.append(pow(base, cols) + encode(board, rows, cols, base))
                        board[i][j] = 1
                        board[i][j - 1] = 0
                    if i < cols - 1 and board[i + 1][j - 1] == 2:
                        board[i][j] = 0
                        board[i + 1][j - 1] = 1
                        states.append(pow(base, cols) + encode(board, rows, cols, base))
                        board[i][j] = 1
                        board[i + 1][j - 1] = 2
    else:
        for i in range(cols):
            if board[i][0] == 1:
                return []
        for i in range(cols):
            for j in range(rows - 1):
                if board[i][j] == 2:
                    if i > 0 and board[i - 1][j + 1] == 1:
                        board[i][j] = 0
                        board[i - 1][j + 1] = 2
                        states.append(encode(board, rows, cols, base))
                        board[i][j] = 2
                        board[i - 1][j + 1] = 1
                    if board[i][j + 1] == 0:
                        board[i][j] = 0
                        board[i][j + 1] = 2
                        states.append(encode(board, rows, cols, base))
                        board[i][j] = 2
                        board[i][j + 1] = 0
                    if i < cols - 1 and board[i + 1][j + 1] == 1:
                        board[i][j] = 0
                        board[i + 1][j + 1] = 2
                        states.append(encode(board, rows, cols, base))
                        board[i][j] = 2
                        board[i + 1][j + 1] = 1
    return states

def decode(encoding, rows, cols, base):
    board = []
    for i in range(cols):
        board.append([0] * rows)
    columns = decodeColumns(encoding, cols, base)
    for i in range(cols):
        if columns[i] == 0:
            for j in range(rows):
                board[i][j] = 0
        elif columns[i] <= rows:
            board[i][columns[i] - 1] = 1
        elif columns[i] <= 2 * rows:
            board[i][columns[i] - 1 - rows] = 2
        else:
            if columns[i] <= 2 * rows + comb(rows, 2):
                calc = resolve(columns[i] - 2 * rows - 1, rows)
                board[i][calc[0]] = 1
                board[i][calc[1]] = 1
            elif columns[i] <= 2 * rows + 2 * comb(rows, 2):
                calc = resolve(columns[i] - 2 * rows - comb(rows, 2) - 1, rows)
                board[i][calc[0]] = 1
                board[i][calc[1]] = 2
            elif columns[i] <= 2 * rows + 3 * comb(rows, 2):
                calc = resolve(columns[i] - 2 * rows - 2 * comb(rows, 2) - 1, rows)
                board[i][calc[0]] = 2
                board[i][calc[1]] = 1
            else:
                calc = resolve(columns[i] - 2 * rows - 3 * comb(rows, 2) - 1, rows)
                board[i][calc[0]] = 2
                board[i][calc[1]] = 2
    return board

def decodeColumns(encoding, cols, base):
    result = []
    for i in range(cols):
        result.append(floor(encoding / pow(base, cols - 1 - i)) % base)
    return result

def resolve(column, rows):
    position = 0
    for i in range(rows - 1):
        position += (rows - 1 - i)
        if column < position:
            return (i, column - position + rows)
        
def encode(board, rows, cols, base):
    result = 0
    for i in range(cols):
        found = False
        start = 0
        for j in range(rows):
            if board[i][j] == 1:
                for k in range(rows - j - 1):
                    if board[i][j + k + 1] == 1:
                        result += (start + k + 1 + 2 * rows) * pow(base, cols - 1 - i)
                        found = True
                        break
                    if board[i][j + k + 1] == 2:
                        result += (start + k + 1 + 2 * rows + comb(rows, 2)) * pow(base, cols - 1 - i)
                        found = True
                        break
                if not found:
                    result += (j + 1) * pow(base, cols - 1 - i)
                break
            elif board[i][j] == 2:
                for k in range(rows - j - 1):
                    if board[i][j + k + 1] == 1:
                        result += (start + k + 1 + 2 * rows + 2 * comb(rows, 2)) * pow(base, cols - 1 - i)
                        found = True
                        break
                    if board[i][j + k + 1] == 2:
                        result += (start + k + 1 + 2 * rows + 3 * comb(rows, 2)) * pow(base, cols - 1 - i)
                        found = True
                        break
                if not found:
                    result += (j + 1 + rows) * pow(base, cols - 1 - i)
                break
            start += (rows - j - 1)
    return result

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