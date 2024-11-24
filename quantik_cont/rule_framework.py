from enum import Enum

class COLOR(Enum):
    BLACK = 1
    WHITE = 2
    
class SHAPE(Enum):
    SQUARE = 1
    TRIANGLE = 2
    CIRCLE = 3
    DIAMOND = 4
    
class FILTER_TYPE(Enum):
    COLOR = 1
    SHAPE = 2
    ROW = 3
    COL = 4
    QUADRANT = 5
    
class VARIABLE_TYPE(Enum):
    ROOT = 1
    COLOR = COLOR
    SHAPE = SHAPE
    ROW = 2
    COL = 3
    QUADRANT = 4
    
class ACTION_TYPE(Enum):
    SET_SHAPE = 1
    SET_ROW = 2
    SET_COL = 3
    SET_QUADRANT = 4

class Piece():
    def __init__(self, color: COLOR, shape: SHAPE, row: int, col: int, quadrant: int = None):
        if not 0 <= row <= 3: raise ValueError("Row must be in range [0,3]")
        if not 0 <= col <= 3: raise ValueError("Col must be in range [0,3]")
        quadrant = row // 2 * 2 + col // 2 if quadrant is None else quadrant
        if not 0 <= quadrant <= 3: raise ValueError("Sector must be in range [0,3]")
        
        self.color = color
        self.shape = shape
        self.row = row
        self.col = col
        self.quadrant = quadrant
        
class Filter():
    pass
        
class Condition():
    pass

class Choice():
    pass

class Variable():
    
    def __init__(self, type: VARIABLE_TYPE, type_value, origin: "Rule"):
        self.type = type
        self.type_value = type_value
        self.origin = origin
        
class Rule():
    def __init__(self, 
    filter: Filter, 
    conditions: list[Condition], 
    choices: list[Choice],
    variables: list[Variable], 
    children: list["Rule"]
    ):
        assert len(conditions) == len(choices) == len(children)
        
        self.filter = filter
        self.conditions = conditions
        self.choices = choices
        self.variables = variables
        self.children = children
        
        
    def run(self, pieces: set[Piece], possible_moves: set[Piece]):
        
        filtered_pieces = self.filter.apply(pieces)
        
        for set in filtered_pieces:
            for condition, choice, child in zip(self.conditions, self.choices, self.children):
                if condition.holds(set):
                    possible_moves = choice.apply(possible_moves)
                    possible_moves = child.run(set, possible_moves)
                    
        return possible_moves

class RuleFramework():
    def __init__(self):       
        self.depth: int = 1
        self.root: Rule = Rule(
            None, 
            [], 
            [], 
            [Variable(VARIABLE_TYPE.ROOT, None, self)],
            []
        )
        
        self.all_black_moves: list[Piece] = []
        self.all_white_moves: list[Piece] = []
        
        self.compute_all_moves()

    def compute_all_moves(self):
        for row in range(4):
            for col in range(4):
                quadrant = (row // 2) * 2 + (col // 2)
                for color in COLOR:
                    for shape in SHAPE:
                        if color == COLOR.BLACK:
                            self.all_black_moves.append(Piece(color, shape, row, col, quadrant))
                        else:
                            self.all_white_moves.append(Piece(color, shape, row, col, quadrant))
                            

    def get_all_moves(self, for_black):
        return self.all_black_moves if for_black else self.all_white_moves
        
        
    def run(self, rootset: set[Piece]):
        
        possible_moves = self.get_all_moves(True) # TODO: color
        
        possible_moves = self.root.run(rootset, possible_moves)
        
        move = possible_moves.pop()                
        return move     
        
        
if __name__ == "__main__":
    print("Rule Framework")
    rootset = set[Piece]([
        Piece(COLOR.BLACK, SHAPE.SQUARE, 0, 0),
        Piece(COLOR.WHITE, SHAPE.CIRCLE, 0, 1),
    ])
    