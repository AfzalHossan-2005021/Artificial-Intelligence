#include<bits/stdc++.h>

using namespace std;

const int TOTAL_BINS = 14;

const int PLAYER_A = 7;
const int PLAYER_B = 14;

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

int heuristic(Mancala state){
    return state.getStorageStones(PLAYER_A) - state.getStorageStones(PLAYER_B);
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
    vector<int> board = {0, 4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 0};
    Mancala state(board);
    state.printBoard();
    cout << getBestMove(state, PLAYER_A, 5) << endl;
    return 0;
}