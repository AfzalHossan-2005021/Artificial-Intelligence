#include <bits/stdc++.h>
using namespace std;

int main() {
    int k;
    cout << "Enter grid size, k: ";
    cin >> k;
    cout << "Enter the grid (Each row in seperate line): " << endl;
    vector<vector<int>> grid(k, vector<int>(k));
    int blank_row;
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            string s;
            cin >> s;
            if(s == "*"){
                grid[i][j] = -1;
                blank_row = i;
            } else {
                grid[i][j] = stoi(s);
            }
        }
    }
    vector<int> linearized_grid(k*k + 1);
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            linearized_grid[i*k + j + 1] = grid[i][j];
        }
    }
    int inversions = 0;
    for(int i = 1; i <= k*k; i++){
        for(int j = i + 1; j <= k*k; j++){
            if(linearized_grid[i] == -1 || linearized_grid[j] == -1){
                continue;
            }
            if(linearized_grid[i] > linearized_grid[j]){
                inversions++;
            }
        }
    }
    bool is_solvable;
    if(k & 1){  // size k is an odd integer
        if(inversions & 1){
            is_solvable = false;
        } else {
            is_solvable = true;
        }
    } else {    // size k is an even integer
        int blank_row_from_bottom = k - blank_row;  // blank row is counted from bottom && 1-indexed
        if((inversions + blank_row_from_bottom) & 1){
            is_solvable = true;
        } else {
            is_solvable = false;
        }
    }
    if(is_solvable){
        cout << "The given board state is solvable" << endl;
    } else {
        cout << "The given board state is not Solvable" << endl;
    }
}