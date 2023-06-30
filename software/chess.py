from stockfish import Stockfish

stockfish = Stockfish(path="stockfish_15.1_win_x64_avx2/stockfish-windows-2022-x86-64-avx2.exe")

def getLegalMoves():
    stockfish._put('go perft 1')
    lines = []
    while True:
        text = stockfish._read_line()
        splitted_text = text.split(":")        
        if splitted_text[0] == "Nodes searched":
            break
        if len(splitted_text[0]) > 0:
            lines.append(splitted_text[0])
    return lines

def isDraw():
    eval = stockfish.get_evaluation()
    if eval["type"] == "mate":
        return False
    return True

def getMoveFromUser():
    moves = getLegalMoves()
    for move in moves:
        print(move)

    move = input("Enter move: ")
    while move not in moves:
        move = input("Invalid move. Enter move: ")

    return move

def getMoveFromStockfish():
    return stockfish.get_best_move()

def main():
    while True:
        print(stockfish.get_board_visual())

        move = getMoveFromUser()
        stockfish.make_moves_from_current_position([move])

        best_move = getMoveFromStockfish
        stockfish.make_moves_from_current_position([best_move])

if __name__ == "__main__":
    main()