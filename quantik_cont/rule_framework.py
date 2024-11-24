from enum import Enum

class COLOR(Enum):
    BLACK = 1
    WHITE = 2
    
class SHAPE(Enum):
    TRIANGLE = 1
    WHITE = 2

class Piece():
    def __init__(self, color: COLOR, row: int, col: int, quadrant: int):
        if not 0 <= row <= 3:
            raise ValueError("Row must be in range [0,3]")
        if not 0 <= col <= 3:
            raise ValueError("Col must be in range [0,3]")
        if not 0 <= quadrant <= 3:
            raise ValueError("Sector must be in range [0,3]")

class Ruleset():
    def __init__(self, rootset: set[Piece]):
        self.rootset = rootset
        self.filters = []
        self.rules = []
        self.actions = []
        
if __name__ == "__main__":
    print("Rule Framework")
    rootset = set[Piece]([
        Piece(COLOR.BLACK, 0, 0, 0),
        Piece(COLOR.WHITE, 0, 0, 1),
    ])