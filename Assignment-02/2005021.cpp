#include<bits/stdc++.h>

using namespace std;

const int MAX_DEPTH = 10;
int heuristic_type;

const int TOTAL_BINS = 14;
const int INITIAL_STONES = 4;

const int PLAYER_A = 7;
const int PLAYER_B = 14;

int PLAYER_A_additional_moves = 0;
int PLAYER_B_additional_moves = 0;

int PLAYER_A_stones_captured = 0;
int PLAYER_B_stones_captured = 0;

int W1 = 4, W2 = 3, W3 = 2, W4 = 1;

class Mancala;

int getBestMove(Mancala state, int player, int depth = MAX_DEPTH);

class Mancala{
public:
    vector<int> board;
    bool moveAgain = false;
    Mancala(const vector<int> tmpBoard){
        this->board = tmpBoard;
    }
    Mancala(const Mancala& other) {
        this->board = other.board;
        this->moveAgain = other.moveAgain;
    }
    int getStorageStones(int player){
        return board[player];
    }
    int getBinStones(int player){
        int sum = 0;
        for(int i = player - 6; i < player; i++){
            sum += board[i];
        }
        return sum;
    }
    bool isGameOver(){
        return getBinStones(PLAYER_A) == 0 || getBinStones(PLAYER_B) == 0;
    }
    bool isBinEmpty(int bin){
        return board[bin] == 0;
    }
    void move(int player, int bin, int depth, bool manualMove = false){
        if(bin < player - 6 || bin >= player || isBinEmpty(bin)){
            return;
        }
        moveAgain = false;
        int stones = board[bin];
        board[bin] = 0;
        while(stones > 0){
            bin++;
            if(bin > TOTAL_BINS) bin = 1;
            if(player == PLAYER_A && bin == PLAYER_B) continue;
            if(player == PLAYER_B && bin == PLAYER_A) continue;
            board[bin]++;
            stones--;
        }
        if(bin == player && getBinStones(player) > 0){ // last stone in player's storage
            if(manualMove){
                moveAgain = true;
                if(player == PLAYER_A){
                    PLAYER_A_additional_moves++;
                } else {
                    PLAYER_B_additional_moves++;
                }
            } else {
                move(player, getBestMove(*this, player, depth -1), depth - 1);
            }
        } else if(board[bin] == 1){ // last stone in an empty bin
            if((player == PLAYER_A && bin > 0 && bin < PLAYER_A) || (player == PLAYER_B && bin > PLAYER_A && bin < PLAYER_B)){ // player's side
                int oppositeBin = 2 * player - bin;
                if(oppositeBin > TOTAL_BINS){
                    oppositeBin -= TOTAL_BINS;
                }
                if(board[oppositeBin] > 0){
                    if(manualMove){
                        if(player == PLAYER_A){
                            PLAYER_A_stones_captured += board[oppositeBin];
                        } else {
                            PLAYER_B_stones_captured += board[oppositeBin];
                        }
                    }
                    board[player] += board[oppositeBin];
                    board[player] += board[bin];
                    board[oppositeBin] = 0;
                    board[bin] = 0;
                }
            }
        }
        if(getBinStones(player) == 0){
            int opponent = (player == PLAYER_A ? PLAYER_B : PLAYER_A);
            board[opponent] += getBinStones(opponent);
            for(int i = opponent - 6; i < opponent; i++){
                board[i] = 0;
            }
        }
    }
    void printBoard(){
        for(int i = 13; i >= 8; i--){
            cout << "\t" << board[i];
        }
        cout << endl;
        cout << board[PLAYER_B];
        for(int i = 1; i <= 7; i++){
            cout << "\t";
        }
        cout << board[PLAYER_A] << endl;
        for(int i = 1; i <= 6; i++){
            cout << "\t" << board[i];
        }
        cout << endl << endl;
    }
};

int heuristic_1(Mancala state){
    return state.getStorageStones(PLAYER_A) - state.getStorageStones(PLAYER_B);
}

int heuristic_2(Mancala state){
    return W1 * (state.getStorageStones(PLAYER_A) - state.getStorageStones(PLAYER_B)) + W2 * (state.getBinStones(PLAYER_A) - state.getBinStones(PLAYER_B));
}

int heuristic_3(Mancala state){
    return W1 * (state.getStorageStones(PLAYER_A) - state.getStorageStones(PLAYER_B)) + W2 * (state.getBinStones(PLAYER_A) - state.getBinStones(PLAYER_B)) + W3 * (PLAYER_A_additional_moves - PLAYER_B_additional_moves);
}

int heuristic_4(Mancala state){
    return W1 * (state.getStorageStones(PLAYER_A) - state.getStorageStones(PLAYER_B)) + W2 * (state.getBinStones(PLAYER_A) - state.getBinStones(PLAYER_B)) + W3 * (PLAYER_A_additional_moves - PLAYER_B_additional_moves) + W4 * (PLAYER_A_stones_captured - PLAYER_B_stones_captured);
}

int heuristic(Mancala state){
    if(heuristic_type == 1){
        return heuristic_1(state);
    } else if(heuristic_type == 2){
        return heuristic_2(state);
    } else if(heuristic_type == 3){
        return heuristic_3(state);
    } else {
        return heuristic_4(state);
    }
}

int AdversarialSearch(Mancala state, int player, int alpha, int beta, int depth) {
    if(state.isGameOver() || depth <= 0){
        return heuristic(state);
    }
    if(player == PLAYER_A){
        int value = INT_MIN;
        for(int i = player - 6; i < player; i++){
            if(state.isBinEmpty(i)) continue;
            Mancala childState = state;
            childState.move(player, i, depth);
            value = max(value, AdversarialSearch(childState, PLAYER_B, alpha, beta, depth - 1));
            if(value > beta) break;
            alpha = max(alpha, value);
        }
        return value;
    } else {
        int value = INT_MAX;
        for(int i = player - 6; i < player; i++){
            if(state.isBinEmpty(i)) continue;
            Mancala childState = state;
            childState.move(player, i, depth);
            value = min(value, AdversarialSearch(childState, PLAYER_A, alpha, beta, depth - 1));
            if(value < alpha) break;
            beta = min(beta, value);
        }
        return value;
    }
}

int getBestMove(Mancala state, int player, int depth){
    int bestMove = -1, alpha = INT_MIN, beta = INT_MAX;
    if(player == PLAYER_A){
        for(int i = player - 6; i < player; i++){
            if(state.isBinEmpty(i)) continue;
            Mancala nextState = state;
            nextState.move(player, i, depth);
            int value = AdversarialSearch(nextState, PLAYER_B, alpha, beta, depth - 1);
            if(value > alpha){
                alpha = value;
                bestMove = i;
            }
        }
    } else {
        for(int i = player - 6; i < player; i++){
            if(state.isBinEmpty(i)) continue;
            Mancala nextState = state;
            nextState.move(player, i, depth);
            int value = AdversarialSearch(nextState, PLAYER_A, alpha, beta, depth - 1);
            if(value < beta){
                beta = value;
                bestMove = i;
            }
        }
    }
    return bestMove;
}

int main(){
    int playingMode;
    cout << "Welcome to Mancala!" << endl;
    cout << "Playing mode -\n\t1: Player vs Computer\n\t2: Computer vs Computer\n\t3: Suggest best move\nEnter your choice: ";
    cin >> playingMode;
    if(playingMode != 1 && playingMode != 2 && playingMode != 3){
        cout << "Invalid choice!" << endl;
        return 0;
    }
    cout << "Enter heuristic type: ";
    cin >> heuristic_type;
    if(playingMode == 1){ // Player vs Computer
        vector<int> initialState(TOTAL_BINS+1, INITIAL_STONES);
        initialState[PLAYER_A] = initialState[PLAYER_B] = 0;
        Mancala game(initialState);
        cout << endl << "Initial board:" << endl;
        game.printBoard();
        while(!game.isGameOver()){
            player_move:
            int move;
            cout << "Enter move: ";
            cin >> move;
            if(move < 1 || move > 6 || game.isBinEmpty(move)){
                cout << "Invalid move!" << endl;
                continue;
            }
            game.move(PLAYER_A, move, MAX_DEPTH, true);
            cout << "After your move: " << move << endl;
            game.printBoard();
            if(game.isGameOver()) break;
            if(game.moveAgain) goto player_move;
            computer_move:
            move = getBestMove(game, PLAYER_B);
            game.move(PLAYER_B, move, MAX_DEPTH, true);
            cout << "After computer's move: " << move << endl;
            game.printBoard();
            cout << endl;
            if(game.isGameOver()) break;
            if(game.moveAgain) goto computer_move;
        }
        if(game.getStorageStones(PLAYER_A) == game.getStorageStones(PLAYER_B)){
            cout << "It's a tie!" << endl;
        } else {
            cout << "Winner: " << (game.getStorageStones(PLAYER_A) > game.getStorageStones(PLAYER_B) ? "You" : "Computer") << endl;
        }
    } else if(playingMode == 2){ // Computer vs Computer
        vector<int> initialState(TOTAL_BINS+1, INITIAL_STONES);
        initialState[PLAYER_A] = initialState[PLAYER_B] = 0;
        Mancala game(initialState);
        cout << endl << "Initial board:" << endl;
        game.printBoard();
        while(!game.isGameOver()){
            computer1_move:
            int move = getBestMove(game, PLAYER_A);
            game.move(PLAYER_A, move, MAX_DEPTH, true);
            cout << "After computer 1's move: " << move << endl;
            game.printBoard();
            cout << endl;
            if(game.isGameOver()) break;
            if(game.moveAgain) goto computer1_move;
            computer2_move:
            move = getBestMove(game, PLAYER_B);
            game.move(PLAYER_B, move, MAX_DEPTH, true);
            cout << "After computer 2's move: " << move << endl;
            game.printBoard();
            cout << endl;
            if(game.isGameOver()) break;
            if(game.moveAgain) goto computer2_move;
        }
        if(game.getStorageStones(PLAYER_A) == game.getStorageStones(PLAYER_B)){
            cout << "It's a tie!" << endl;
        } else {
            cout << "Winner: " << (game.getStorageStones(PLAYER_A) > game.getStorageStones(PLAYER_B) ? "Computer 1" : "Computer 2") << endl;
        }
    } else { // Suggest best move
        cout << "Input board state: ";
        vector<int> boardState(TOTAL_BINS+1);
        for(int i = 1; i <= TOTAL_BINS; i++){
            cin >> boardState[i];
        }
        cout << "Best move: " << getBestMove(boardState, PLAYER_A, MAX_DEPTH) << endl;
    }
    return 0;
}