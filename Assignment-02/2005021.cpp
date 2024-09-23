#include<bits/stdc++.h>

using namespace std;

const int TOTAL_BINS = 14;

const int PLAYER_A = 7;
const int PLAYER_B = 14;

class Mancala{
public:
    vector<int> board;
    Mancala(const vector<int> tmpBoard){
        this->board = tmpBoard;
    }
    Mancala(const Mancala& other) {
        this->board = other.board;
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

int main(){
    vector<int> board = {0, 4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 0};
    Mancala state(board);
    state.printBoard();
    return 0;
}