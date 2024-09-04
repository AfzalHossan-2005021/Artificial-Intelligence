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

// A* search algorithm
void A_star_search(int k, vector<vector<int>> &grid, vector<vector<int>> &goal, int blank_row, int blank_col, int (*heuristic_function) (vector<vector<int>> &board)){
    priority_queue<pair<int, search_node*>, vector<pair<int, search_node*>>, greater<pair<int, search_node*>>> open_list;
    search_node *initial_search_node = new search_node(grid, 0, NULL, blank_row, blank_col);
    open_list.push({priority_function(initial_search_node, heuristic_function), initial_search_node});

    // Neighbor board creation function
    auto create_Neighbor_board = [&](search_node *current_node, int old_row, int old_col, int new_row, int new_col){
        search_node *new_node = new search_node(current_node->board, current_node->move_count + 1, current_node, new_row, new_col);
        swap(new_node->board[old_row][old_col], new_node->board[new_row][new_col]);
        if(current_node->parent == NULL || !new_node->equal_board(current_node->parent->board)){
            open_list.push({priority_function(new_node, heuristic_function), new_node});
        } else {
            delete new_node;
        }
    };

    // Expanding the nodes
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
    }

    // Determining the path to reach the goal state using the parent pointers
    stack<search_node*> path;
    while(path_last_node != NULL){
        path.push(path_last_node);
        path_last_node = path_last_node->parent;
    }

    cout << "The optimal cost to reach the goal state: " << path.size() - 1 << endl << endl;
    
    // Printing the steps
    cout << "The steps are as follow: " << endl;
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

    cout << "Explored nodes: " << open_list.size() << endl;
    cout << "Expanded nodes: " << closed_list.size() << endl;
    cout << endl;
}

int main() {
    int k;
    cout << "Enter grid size, k: ";
    cin >> k;
    cout << "Enter the initial board position:" << endl;
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
        cout << endl << "The given board state is solvable." << endl << endl;
    } else {
        cout << endl << "The given board state is not Solvable." << endl;
        return 0;
    }

    // Determining goal state
    vector<vector<int>> goal(k, vector<int>(k));
    for(int i = 0; i < k; i++){
        for(int j = 0; j < k; j++){
            goal[i][j] = i*k + j + 1;
        }
    }
    goal[k-1][k-1] = -1;

    cout << "Optimal solution using Hamming distance heuristic: " << endl << endl;
    A_star_search(k, grid, goal, blank_row, blank_col, &hamming_distance);

    cout << "Optimal solution using manhattan distance heuristic: " << endl << endl;
    A_star_search(k, grid, goal, blank_row, blank_col, &manhattan_distance);

    return 0;
}