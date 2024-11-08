import os
import math
import random


# Function to calculate the distance between two cities
def distance(city1, city2):
    return math.sqrt((city1[0] - city2[0])**2 + (city1[1] - city2[1])**2)


# Function to calculate the distance between two nodes
def tour_distance(tour, distance_matrix):
    return sum(distance_matrix[tour[i]][tour[i+1]] for i in range(len(tour)-1)) + distance_matrix[tour[-1]][tour[0]]


# Function to read a .tsp file and return the number of cities and the average distance between cities
def read_tsp_file(filename):
    with open(filename, 'r') as file:
        # skip first 6 lines
        for i in range(6):
            file.readline()

        # read the number of cities from the 7th line   
        lines = file.readlines()

        # each line contains the city number, x coordinate, and y coordinate
        cities = []
        for line in lines:
            # check for EOF
            if line.strip() == 'EOF':
                break
            # read a integer and two floats from the line
            city = line.split()
            cities.append((float(city[1]), float(city[2]), int(city[0]) - 1))

        return cities
    

# Function to convert a list of edges to a tour
def edges_to_tour(edges):
    # Step 1: Create a mapping from each node_id to its neighbors
    connections = {}
    for (node1, node2) in edges:
        id1, id2 = node1[2], node2[2]  # Extract node ids
        if id1 not in connections:
            connections[id1] = []
        if id2 not in connections:
            connections[id2] = []
        connections[id1].append(id2)
        connections[id2].append(id1)
    
     # Step 2: Find the starting node (a node with only one connection)
    start_node = None
    for node, neighbors in connections.items():
        if len(neighbors) == 1:  # Node has only one neighbor, so it’s an endpoint of an open path
            start_node = node
            break
    if start_node is None:
        # If no node with only one connection is found, pick any node to start (suitable for closed loops)
        start_node = next(iter(connections))

    # Step 3: Build the tour by following connections
    tour = [start_node]
    current_node = start_node
    prev_node = None
    
    while True:
        # Get neighbors of the current node
        neighbors = connections[current_node]
        # Choose the next node that isn't the previous node
        next_node = neighbors[0] if neighbors[0] != prev_node else neighbors[1]
        # Stop if we've returned to the start in a closed loop
        if next_node == start_node:
            break
        tour.append(next_node)
        # Update prev_node and current_node
        prev_node, current_node = current_node, next_node
        # Stop if there are no more unvisited neighbors (for open paths)
        if len(connections[current_node]) == 1:
            break
    
    return tour


################################################ Nearest Neighbour Heuristic ################################################
#   Step 1. Start with any node as the beginning nodes                                                                      #
#   Step 2. Find the unvisited node closest to the last node added to the path. Add this node to the path.                  #
#   Step 3. Repeat Step 2 until all nodes are contained in the path. Then join the first and last nodes.                    #
#############################################################################################################################
def nearest_neighbour(cities):
    # list to store the visited cities
    visited = []
    # list to store the unvisited cities
    unvisited = cities.copy()
    # first add a random city to the visited list
    rand_num = random.randint(0, len(unvisited)-1)
    initial_city = unvisited.pop(rand_num)
    visited.append(initial_city)

    # loop until all cities are visited
    while unvisited:
        # add the nearest city to the the last visited city to the visited list and remove it from the unvisited list
        last_visited_city = visited[-1]
        nearest_city = min(unvisited, key=lambda city: distance(last_visited_city, city))
        visited.append(nearest_city)
        unvisited.remove(nearest_city)

    # add the first city to the visited list to complete the tour
    visited.append(initial_city)

    # convert the tour to edge list
    selected_edges = []
    for i in range(len(visited)-1):
        selected_edges.append((visited[i], visited[i+1]))

    return selected_edges


######################################### Insertion Heuristics (Cheapest Neighbour) #########################################
#   Step 1. Start with a sub-graph consisting of node i only.                                                               #
#   Step 2. Find node r such that cir is minimal and form sub-tour i-r-i.                                                   #
#   Step 3. Find (i, j) in sub-tour and r not, such that cir + crj - cij is minimal. Insert r between i and j.              #
#############################################################################################################################
def insertion_heuristic(cities):
    # list to store the visited cities
    visited = []
    # list to store the unvisited cities
    unvisited = cities.copy()
    # first add a random city to the visited list
    rand_num = random.randint(0, len(unvisited)-1)
    initial_city = unvisited.pop(rand_num)
    visited.append(initial_city)

    # find the nearest city to the first city, add it to the visited list and form a sub-tour
    nearest_city = min(unvisited, key=lambda city: distance(initial_city, city))
    visited.append(nearest_city)
    unvisited.remove(nearest_city)
    visited.append(initial_city)

    # insert city r into the sub-tour
    while unvisited:
        min_cost = math.inf
        for city in unvisited:
            for i in range(1, len(visited)):
                cost = distance(visited[i-1], city) + distance(city, visited[i]) - distance(visited[i-1], visited[i])
                if cost < min_cost:
                    min_cost = cost
                    min_city = city
                    min_index = i

        visited.insert(min_index, min_city)
        unvisited.remove(min_city)

    # convert the tour to edge list
    selected_edges = []
    for i in range(len(visited)-1):
        selected_edges.append((visited[i], visited[i+1]))

    return selected_edges


###################################################### Greedy Heuristic #####################################################
#   Step 1. Sort all the edges in non-decreasing order of their weights.                                                    #
#   Step 2. Pick the smallest edge. Check if it results in any vertices of degree greater than two. If not, then include    #
#           this edge. Otherwise, discard it.                                                                               #
#   Step 3. Repeat Step 2 until there are (V-1) edges in the spanning tree.                                                 #
#############################################################################################################################
def greedy_heuristic(cities):
    # list all the edges
    all_edges = []
    for i in range(len(cities)):
        for j in range(i+1, len(cities)):
            all_edges.append((cities[i], cities[j], distance(cities[i], cities[j])))

    # sort the edges based on the distance
    all_edges.sort(key=lambda edge: edge[2])

    # keep track of the degree of each city
    degree = [0] * (len(cities) + 1)

    # keep track of the neighbors of each city
    neighbors = {i: [] for i in range(len(cities) + 1)}

    # list to store the selected edges
    selected_edges = []

    def detect_path(start, target, visited=None):
        if visited is None:
            visited = set()
        visited.add(start)
        for neighbor in neighbors[start]:
            if neighbor == target and len(visited) < len(cities):
                return True
            if neighbor not in visited:
                if detect_path(neighbor, target, visited):
                    return True
        return False
    
    for edge in all_edges:
        city1_id, city2_id = edge[0][2], edge[1][2]
        if degree[city1_id] < 2 and degree[city2_id] < 2:
            if not detect_path(city1_id, city2_id):
                neighbors[city1_id].append(city2_id)
                neighbors[city2_id].append(city1_id)
                degree[city1_id] += 1
                degree[city2_id] += 1
                selected_edges.append((edge[0], edge[1]))

            if len(selected_edges) == len(cities):
                break

    return selected_edges


###################################################### 2-opt Heuristic ######################################################
#   Step 1. Start with a random tour.                                                                                       #
#   Step 2. Select two edges from the tour.                                                                                 #
#   Step 3. Remove the two edges from the tour.                                                                             #
#   Step 4. Add the two new edges to the tour.                                                                              #
#   Step 5. If the new tour is shorter than the previous tour, then accept the new tour.                                    #
#############################################################################################################################
def two_opt(tour, distance_matrix):
    def gain_from_two_opt(X1, X2, Y1, Y2):
        del_length = distance_matrix[X1][X2] + distance_matrix[Y1][Y2]
        add_length = distance_matrix[X1][Y1] + distance_matrix[X2][Y2]
        return del_length - add_length
    
    N = len(tour)
    optimal_tour = tour
    optimal_distance = tour_distance(tour, distance_matrix)
    local_optimal = False
    loop_iter_count = 0
    while not local_optimal:
        local_optimal = True
        loop_iter_count += 1
        for i in range(N - 2):
            X1 = tour[i]
            X2 = tour[i + 1]
            for j in range(i + 2, N - (i == 0)):
                Y1 = tour[j]
                Y2 = tour[(j + 1) % N]
                if gain_from_two_opt(X1, X2, Y1, Y2) >= 0.5:
                    tour[i+1:j+1] = tour[i+1:j+1][::-1]
                    new_distance = tour_distance(tour, distance_matrix)
                    local_optimal = False
                    if new_distance < optimal_distance:
                        optimal_tour = tour
                        optimal_distance = new_distance

        if loop_iter_count >= N:
            break

    return optimal_tour, optimal_distance


####################################################### Main Function #######################################################
# Function to read all the .tsp files in the directory and store the results
def main():
    # get the current directory
    current_directory = os.getcwd()
    # get the directory containing the TSP files
    directory = os.path.join(current_directory, 'TSP_assignment_task_benchmark_data')
    # get the list of files in the directory
    files = os.listdir(directory)

    # read each file and store the results
    for file in files:
        # check if the file is a .tsp file
        if file.endswith('.tsp'):
            # read the file
            print("File Name: ", file)
            cities = read_tsp_file(os.path.join(directory, file))
            print("Number of Cities: ", len(cities))
            selected_edges = greedy_heuristic(cities)
            tour = edges_to_tour(selected_edges)
            distance_matrix = [[distance(cities[i], cities[j]) for j in range(len(cities))] for i in range(len(cities))]
            print("Initial distance: ", tour_distance(tour, distance_matrix))
            optimized_tour, optimized_distance = two_opt(tour, distance_matrix)
            print("Optimized distance:", optimized_distance)
            

# call the main function
if __name__ == '__main__':
    main()