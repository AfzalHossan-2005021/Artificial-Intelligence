#include <bits/stdc++.h>
using namespace std;

class search_node {
    public:
        vector<vector<int>> board;
        int move_count;
        search_node *parent;
        int blank_row, blank_col;
        search_node(vector<vector<int>> board, int move_count, search_node *parent, int blank_row, int blank_col){
            this->board = board;
            this->move_count = move_count;
            this->parent = parent;
            this->blank_row = blank_row;
            this->blank_col = blank_col;
        }
        bool equal_board(const vector<vector<int>> &other) {
            bool is_equal = true;
            for(int i = 0; i < board.size(); i++){
                for(int j = 0; j < board.size(); j++){
                    if(board[i][j] != other[i][j]){
                        is_equal = false;
                        break;
                    }
                }
            }
            return is_equal;
        }
};

// Hamming distance is the number of tiles that are not in their final position
int hamming_distance(vector<vector<int>> &board){
    int k = board.size();
    int hamming_distance = 0;
    for(int i = 0; i < k; i++){
        for(int j = 0; j < k; j++){
            if(board[i][j] == -1) continue;
            if(board[i][j] != i*k + j + 1){
                hamming_distance++;
            }
        }
    }
    return hamming_distance;
}

// Manhattan distance is the sum of the distances of each tile from its final position
int manhattan_distance(vector<vector<int>> &board){
    int k = board.size();
    int manhattan_distance = 0;
    for(int i = 0; i < k; i++){
        for(int j = 0; j < k; j++){
            if(board[i][j] == -1) continue;
            int actual_row = (board[i][j] - 1) / k;
            int actual_col = (board[i][j] - 1) % k;
            manhattan_distance += abs(i - actual_row) + abs(j - actual_col);
        }
    }
    return manhattan_distance;
}

int priority_function(search_node *node, int (*func) (vector<vector<int>> &board)){
    return node->move_count + func(node->board);
}

int main() {
    int k;
    cout << "Enter grid size, k: ";
    cin >> k;
    cout << "Enter the the initial board position (Each row in seperate line): " << endl;
    vector<vector<int>> grid(k, vector<int>(k));
    int blank_row, blank_col;
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            string s;
            cin >> s;
            if(s == "*"){
                grid[i][j] = -1;
                blank_row = i;
                blank_col = j;
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
        return 0;
    }

    vector<vector<int>> goal(k, vector<int>(k));
    for(int i = 0; i < k; i++){
        for(int j = 0; j < k; j++){
            goal[i][j] = i*k + j + 1;
        }
    }
    goal[k-1][k-1] = -1;

    // Using priority queue to implement A* search using hamming distance as heuristic
    priority_queue<pair<int, search_node*>, vector<pair<int, search_node*>>, greater<pair<int, search_node*>>> open_list;
    search_node *initial_search_node = new search_node(grid, 0, NULL, blank_row, blank_col);
    open_list.push({priority_function(initial_search_node, &manhattan_distance), initial_search_node});
    queue<search_node*> closed_list;
    search_node *path_last_node = NULL;
    while(!open_list.empty()){
        search_node *current_node = open_list.top().second;
        open_list.pop();
        closed_list.push(current_node);
        if(current_node->equal_board(goal)) {
            path_last_node = current_node;
            break;
        }
        int row = current_node->blank_row;
        int col = current_node->blank_col;
        if(row > 0){
            search_node *new_node = new search_node(current_node->board, current_node->move_count + 1, current_node, row - 1, col);
            swap(new_node->board[row][col], new_node->board[row - 1][col]);
            if(current_node->parent == NULL || !new_node->equal_board(current_node->parent->board)){
                open_list.push({priority_function(new_node, &manhattan_distance), new_node});
            }
        }
        if(row < k - 1){
            search_node *new_node = new search_node(current_node->board, current_node->move_count + 1, current_node, row + 1, col);
            swap(new_node->board[row][col], new_node->board[row + 1][col]);
            if(current_node->parent == NULL || !new_node->equal_board(current_node->parent->board)){
                open_list.push({priority_function(new_node, &manhattan_distance), new_node});
            }
        }
        if(col > 0){
            search_node *new_node = new search_node(current_node->board, current_node->move_count + 1, current_node, row, col - 1);
            swap(new_node->board[row][col], new_node->board[row][col - 1]);
            if(current_node->parent == NULL || !new_node->equal_board(current_node->parent->board)){
                open_list.push({priority_function(new_node, &manhattan_distance), new_node});
            }
        }
        if(col < k - 1){
            search_node *new_node = new search_node(current_node->board, current_node->move_count + 1, current_node, row, col + 1);
            swap(new_node->board[row][col], new_node->board[row][col + 1]);
            if(current_node->parent == NULL || !new_node->equal_board(current_node->parent->board)){
                open_list.push({priority_function(new_node, &manhattan_distance), new_node});
            }
        }
    }
    cout << closed_list.size() << " moves required to reach the goal state" << endl;

    // Printing the path
    stack<search_node*> path;

    while(path_last_node != NULL){
        path.push(path_last_node);
        path_last_node = path_last_node->parent;
    }
    cout << path.size() - 1 << " moves in the path" << endl;
    cout << "Path: " << endl;
    while(!path.empty()){
        search_node *current_node = path.top();
        path.pop();
        for(int i = 0; i < k; i++){
            for(int j = 0; j < k; j++){
                if(current_node->board[i][j] == -1){
                    cout << "* ";
                    continue;
                }
                cout << current_node->board[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}