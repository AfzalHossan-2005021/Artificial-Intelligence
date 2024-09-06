#include <bits/stdc++.h>
using namespace std;

class search_node {
    public:
        vector<vector<char>> board;
        int move_count, blank_row, blank_col;
        string path = "";
        search_node(vector<vector<char>> board, int move_count, int blank_row, int blank_col){
            this->board = board;
            this->move_count = move_count;
            this->blank_row = blank_row;
            this->blank_col = blank_col;
        }
};

// Hamming distance is the number of tiles that are not in their final position
int hamming_distance(vector<vector<char>> &board){
    int k = board.size();
    int hamming_distance = 0;
    for(int i = 0; i < k; i++){
        for(int j = 0; j < k; j++){
            if(board[i][j] == 0) continue;
            if(board[i][j] != i*k + j + 1){
                hamming_distance++;
            }
        }
    }
    return hamming_distance;
}

// Manhattan distance is the sum of the distances of each tile from its final position
int manhattan_distance(vector<vector<char>> &board){
    int k = board.size();
    int manhattan_distance = 0;
    for(int i = 0; i < k; i++){
        for(int j = 0; j < k; j++){
            if(board[i][j] == 0) continue;
            int actual_row = (board[i][j] - 1) / k;
            int actual_col = (board[i][j] - 1) % k;
            manhattan_distance += abs(i - actual_row) + abs(j - actual_col);
        }
    }
    return manhattan_distance;
}

int priority_function(search_node *node, int (*func) (vector<vector<char>> &board)){
    return node->move_count + func(node->board);
}

// Hash function for board state
long long hash_board(vector<vector<char>> &board){
    long long hash_value = 0;
    long long base = 7;
    long long mult = 1;
    for(int i = 0; i < board.size(); i++){
        for(int j = 0; j < board.size(); j++){
            hash_value += mult * board[i][j];
            mult *= base;
        }
    }
    return hash_value;
}

void print_path(search_node *node, int index){
    search_node tmp = *node;
    if(index > -1){
        if(node->path[index] == 'U'){
            swap(node->board[node->blank_row][node->blank_col], node->board[node->blank_row + 1][node->blank_col]);
            node->blank_row++;
        } else if(node->path[index] == 'D'){
            swap(node->board[node->blank_row][node->blank_col], node->board[node->blank_row - 1][node->blank_col]);
            node->blank_row--;
        } else if(node->path[index] == 'L'){
            swap(node->board[node->blank_row][node->blank_col], node->board[node->blank_row][node->blank_col + 1]);
            node->blank_col++;
        } else if(node->path[index] == 'R'){
            swap(node->board[node->blank_row][node->blank_col], node->board[node->blank_row][node->blank_col - 1]);
            node->blank_col--;
        }
        print_path(node, index - 1);
    }
    for(int i = 0; i < tmp.board.size(); i++){
        for(int j = 0; j < tmp.board.size(); j++){
            if(tmp.board[i][j] == 0){
                cout << "* ";
                continue;
            }
            cout << (int)tmp.board[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// A* search algorithm
void A_star_search(int k, vector<vector<char>> &grid, vector<vector<char>> &goal, int blank_row, int blank_col, int (*heuristic_function) (vector<vector<char>> &board)){
    priority_queue<pair<int, search_node*>, vector<pair<int, search_node*>>, greater<pair<int, search_node*>>> open_list;
    set<long long> checked_states;
    search_node *initial_search_node = new search_node(grid, 0, blank_row, blank_col);
    open_list.push({priority_function(initial_search_node, heuristic_function), initial_search_node});
    checked_states.insert(hash_board(grid));

    // Neighbor board creation function
    auto create_Neighbor_board = [&](search_node *current_node, int old_row, int old_col, int new_row, int new_col){
        search_node *new_node = new search_node(current_node->board, current_node->move_count + 1, new_row, new_col);
        swap(new_node->board[old_row][old_col], new_node->board[new_row][new_col]);
        if(checked_states.find(hash_board(new_node->board)) == checked_states.end()){
            open_list.push({priority_function(new_node, heuristic_function), new_node});
            checked_states.insert(hash_board(new_node->board));
            if(new_row == old_row + 1){
                new_node->path = current_node->path + "D";
            } else if(new_row == old_row - 1){
                new_node->path = current_node->path + "U";
            } else if(new_col == old_col + 1){
                new_node->path = current_node->path + "R";
            } else if(new_col == old_col - 1){
                new_node->path = current_node->path + "L";
            }
        } else {
            delete new_node;
        }
    };

    // Expanding the nodes
    long long expanded_nodes = 0, explored_nodes = 0;
    search_node path_last_node = *initial_search_node;
    long long goal_hash = hash_board(goal);
    while(!open_list.empty()){
        search_node *current_node = open_list.top().second;
        open_list.pop();
        int row = current_node->blank_row;
        int col = current_node->blank_col;

        // Creating the neighbor boards
        if(row > 0){
            create_Neighbor_board(current_node, row, col, row - 1, col);
        }
        if(row < k - 1){
            create_Neighbor_board(current_node, row, col, row + 1, col);
        }
        if(col > 0){
            create_Neighbor_board(current_node, row, col, row, col - 1);
        }
        if(col < k - 1){
            create_Neighbor_board(current_node, row, col, row, col + 1);
        }
        expanded_nodes++;
        if(hash_board(current_node->board) == goal_hash){
            path_last_node = *current_node;
            delete current_node;
            break;
        }
        delete current_node;
    }

    while(!open_list.empty()){
        delete open_list.top().second;
        open_list.pop();
        explored_nodes++;
    }

    // Determining the path to reach the goal state using the parent pointers
    cout << "The optimal cost to reach the goal state: " << path_last_node.path.size() << endl << endl;

    // Printing the steps
    cout << "The steps are as follow: " << endl;
    print_path(&path_last_node, path_last_node.path.size() - 1);

    cout << "Explored nodes: " << explored_nodes << endl;
    cout << "Expanded nodes: " << expanded_nodes << endl;
    cout << endl;
}

int main() {
    int k;
    cout << "Enter grid size, k: ";
    cin >> k;
    cout << "Enter the initial board position:" << endl;
    vector<vector<char>> grid(k, vector<char>(k));
    int blank_row, blank_col;
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            string s;
            cin >> s;
            if(s == "*"){
                grid[i][j] = 0;
                blank_row = i;
                blank_col = j;
            } else {
                grid[i][j] = stoi(s);
            }
        }
    }

    /***Check if the given board state is solvable or not***/

    // Linearizing the grid
    vector<int> linearized_grid(k*k + 1);
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            linearized_grid[i*k + j + 1] = grid[i][j];
        }
    }

    // Count the number of inversions
    int inversions = 0;
    for(int i = 1; i <= k*k; i++){
        for(int j = i + 1; j <= k*k; j++){
            if(linearized_grid[i] == 0 || linearized_grid[j] == 0){
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
        cout << endl << "The given board state is solvable." << endl << endl;
    } else {
        cout << endl << "The given board state is not Solvable." << endl;
        return 0;
    }

    // Determining goal state
    vector<vector<char>> goal(k, vector<char>(k));
    for(int i = 0; i < k; i++){
        for(int j = 0; j < k; j++){
            goal[i][j] = i*k + j + 1;
        }
    }
    goal[k-1][k-1] = 0;

    // cout << "Optimal solution using Hamming distance heuristic: " << endl << endl;
    // A_star_search(k, grid, goal, blank_row, blank_col, &hamming_distance);

    cout << "Optimal solution using manhattan distance heuristic: " << endl << endl;
    A_star_search(k, grid, goal, blank_row, blank_col, &manhattan_distance);

    return 0;
}