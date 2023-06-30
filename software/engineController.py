from stockfish import Stockfish

class EngineController:
    def __init__(self, path="stockfish_15.1_win_x64_avx2/stockfish-windows-2022-x86-64-avx2.exe", difficulty=15):
        self.engine = Stockfish(path=path)
        self.engine.set_skill_level(difficulty)

    def get_legal_moves(self):
        self.engine._put('go perft 1')
        lines = []
        while True:
            text = self.engine._read_line()
            splitted_text = text.split(":")        
            if splitted_text[0] == "Nodes searched":
                break
            if len(splitted_text[0]) > 0:
                lines.append(splitted_text[0])
        return lines
    
    def game_over(self):
        moves = self.get_legal_moves()
        if len(moves) == 0:
            return True
        return False

    def is_draw(self):
        eval = self.engine.get_evaluation()
        if eval["type"] == "mate":
            return False
        return True

    def get_move_from_user(self):
        moves = self.get_legal_moves()
        move = input("Enter move: ")
        while move not in moves:
            move = input("Invalid move. Enter move: ")

        return move

    def get_move_from_engine(self):
        return self.engine.get_best_move()
    
    def get_board_visual(self):
        return self.engine.get_board_visual()
    
    def play_move(self, move):
        self.engine.make_moves_from_current_position([move])

    def get_captured_piece_square(self, move):
        capture_type = self.engine.will_move_be_a_capture(move)
        start_square = move[:2]
        end_square = move[2:]
        if capture_type == self.engine.Capture.NO_CAPTURE:
            return None
        elif capture_type == self.engine.Capture.DIRECT_CAPTURE:
            return end_square
        elif capture_type == self.engine.Capture.EN_PASSANT:
            return f"{end_square[0]}{start_square[1]}"
        
        return None
    
    def is_castle(self, move):
        move_piece = self.engine.get_what_is_on_square(move[:2])

        if move_piece in [Stockfish.Piece.WHITE_KING, Stockfish.Piece.BLACK_KING]:
            if abs(ord(move[0]) - ord(move[2])) == 2 and move[1] == move[3]:
                return True
            
    def get_piece_at_square(self, square):
        return self.engine.get_what_is_on_square(square)
    
    def get_white_pieces(self):
        pieces = [Stockfish.Piece.WHITE_PAWN, Stockfish.Piece.WHITE_KNIGHT, Stockfish.Piece.WHITE_BISHOP, Stockfish.Piece.WHITE_ROOK, Stockfish.Piece.WHITE_QUEEN, Stockfish.Piece.WHITE_KING]
        return pieces
    
    def get_black_pieces(self):
        pieces = [Stockfish.Piece.BLACK_PAWN, Stockfish.Piece.BLACK_KNIGHT, Stockfish.Piece.BLACK_BISHOP, Stockfish.Piece.BLACK_ROOK, Stockfish.Piece.BLACK_QUEEN, Stockfish.Piece.BLACK_KING]
        return pieces

def main():
    engine = EngineController()
    while True:
        print(engine.get_board_visual())

        move = engine.get_move_from_user()
        engine.play_move(move)

        best_move = engine.get_move_from_engine()
        engine.get_captured_piece_square(best_move)
        engine.play_move(best_move)

if __name__ == "__main__":
    main()