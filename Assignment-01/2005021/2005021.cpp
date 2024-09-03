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
}