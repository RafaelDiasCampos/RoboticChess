import armController
import engineController
from enum import Enum

class RoboticChess:
    class Color(Enum):
        WHITE = 0
        BLACK = 1

    class GameResult(Enum):
        WHITE_WINS = 0
        BLACK_WINS = 1
        DRAW = 2

    def __init__(self, port, baudrate=9600, engine_path=None, color=Color.WHITE, difficulty=15, players=1):
        #self.arm = armController.ArmController(port, baudrate)
        if engine_path is None:
            self.engine = engineController.EngineController(difficulty=difficulty)
        else:
            self.engine = engineController.EngineController(difficulty=difficulty, path=engine_path)
        self.color = color
        self.players = players

        self.current_square = None

    def convert_square(self, square):
        if square is None:
            return None
        
        if self.color == RoboticChess.Color.WHITE:
            return square
        else:
            row = square[1]
            col = square[0]

            row = 9 - int(row)
            col = chr(ord('h') - ord(col) + ord('a'))

            return f"{col}{row}"
        
    def square_to_notation(self, square):
        x = square[0]
        y = square[1]

        row = y
        column = chr(ord('a') + x)

        notation = f"{column}{row}"

        return self.convert_square(notation)

    def player_move(self):
        print(self.engine.get_board_visual())

        move = self.engine.get_move_from_user()
        self.engine.play_move(move)

    def capture_piece(self, captured_piece_square):
        captured_piece_square = self.convert_square(captured_piece_square)
        if captured_piece_square is not None:
            if self.arm.move_to_square(captured_piece_square):
                self.arm.grab_piece()
                self.arm.discard_piece()
                self.arm.release_piece()
            else:
                print("Error: Could not move to captured piece square")

    def move_rook_castle(self, move):
        if move[2] == 'g':
            rook_move =f'h{move[1]}f{move[1]}'
        else:
            rook_move =f'a{move[1]}d{move[1]}'
        
        self.move_piece(rook_move)

    def move_piece(self, move):
        initial_square = self.convert_square(move[:2])
        final_square = self.convert_square(move[2:])      

        if self.arm.move_to_square(initial_square):
            self.arm.grab_piece()
            if self.arm.move_to_square(final_square):
                self.arm.release_piece()
                if self.engine.is_castle(move):
                    self.move_rook_castle(move)
            else:
                print("Error: Could not move to final square")
                self.arm.move_to_square(initial_square)
                self.arm.release_piece()     
        else:
            print("Error: Could not move to initial square")

    def human_select_square(self):
        movement = self.arm.get_joystick_movement()

        while movement != armController.JoystickMovement.SELECT:
            if movement == armController.JoystickMovement.UP:
                if self.current_square[1] < 7:
                    self.current_square = (self.current_square[0], self.current_square[1] + 1)
            elif movement == armController.JoystickMovement.DOWN:
                if self.current_square[1] > 0:
                    self.current_square = (self.current_square[0], self.current_square[1] - 1)
            elif movement == armController.JoystickMovement.LEFT:
                if self.current_square[0] > 0:
                    self.current_square = (self.current_square[0] - 1, self.current_square[1])
            elif movement == armController.JoystickMovement.RIGHT:
                if self.current_square[0] < 7:
                    self.current_square = (self.current_square[0] + 1, self.current_square[1])

            print(f"Current square: {self.current_square}")
            self.arm.move_to_square(self.current_square)
            movement = self.arm.get_joystick_movement()

    def engine_move(self):
        move = self.engine.get_move_from_engine()

        captured_piece_square = self.engine.get_captured_piece_square(move)
        self.capture_piece(captured_piece_square)
          
        self.move_piece(move)
        
        self.arm.reset()

        self.engine.play_move(move)

    def human_move(self):
        if self.color == RoboticChess.Color.WHITE:
            ally_pieces = self.engine.get_white_pieces()
        else:
            ally_pieces = self.engine.get_black_pieces()

        legal_moves = self.engine.get_legal_moves()

        self.current_square = (0, 0)
        self.arm.move_to_square(self.current_square)

        # Wait for user to select a piece to grab
        self.human_select_square()
        while self.engine.get_piece_at_square(self.square_to_notation(self.current_square)) not in ally_pieces:
            self.human_select_square()

        initial_square = self.current_square
        self.arm.grab_piece()

        # Wait for user to select a square to move to
        self.human_select_square()
        final_square = self.current_square
        move = f"{self.square_to_notation(initial_square)}{self.square_to_notation(final_square)}"
        while move not in legal_moves or initial_square == final_square:
            self.human_select_square()
            final_square = self.current_square
            move = f"{self.convert_square(initial_square)}{self.convert_square(final_square)}"
        
        if initial_square == final_square:
            self.arm.release_piece()
            return self.human_move()
        
        # If move is a capture, return the piece to its original square, handle the capture and then grab the piece again
        captured_piece_square = self.engine.get_captured_piece_square(move)
        if captured_piece_square is not None:
            self.arm.move_to_square(initial_square)
            self.arm.release_piece()

            self.capture_piece(captured_piece_square)

            self.arm.move_to_square(initial_square)
            self.arm.grab_piece()
        
        self.arm.move_to_square(final_square)
        self.arm.release_piece()
        if self.engine.is_castle(move):
            self.move_rook_castle(move)

        self.arm.reset()

        self.engine.play_move(move)

    def play_game(self):
        if self.players == 1:
            robot_move_function = self.engine_move
        else:
            robot_move_function = self.human_move

        if self.color == RoboticChess.Color.WHITE:
            robot_move_function()

        while True:
            if self.engine.game_over():
                if self.engine.is_draw():
                    return RoboticChess.GameResult.DRAW
                if self.color == RoboticChess.Color.WHITE:
                    return RoboticChess.GameResult.WHITE_WINS
                else:
                    return RoboticChess.GameResult.BLACK_WINS
                
            self.player_move()

            if self.engine.game_over():
                if self.engine.is_draw():
                    return RoboticChess.GameResult.DRAW
                if self.color == RoboticChess.Color.WHITE:
                    return RoboticChess.GameResult.BLACK_WINS
                else:
                    return RoboticChess.GameResult.WHITE_WINS
                
            robot_move_function()
        

def main():
    players = input("Enter number of players (1/2): ")
    if players != '1' and players != '2':
        print("Invalid number of players")
        return

    color_input = input("Enter color for robot (w/b): ")
    if color_input != 'w' and color_input != 'b':
        print("Invalid color")
        return
    
    difficulty_input = input("Enter difficulty (1-20): ")
    if not difficulty_input.isdigit() or int(difficulty_input) < 1 or int(difficulty_input) > 20:
        print("Invalid difficulty")
        return

    if color_input == 'w':
        color = RoboticChess.Color.WHITE
    else:
        color = RoboticChess.Color.BLACK
    roboticChess = RoboticChess("COM4", color=color, difficulty=int(difficulty_input), players=int(players))

    roboticChess.play_game()

if __name__ == "__main__":
    main()