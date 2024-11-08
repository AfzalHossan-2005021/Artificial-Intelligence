import os
import math
import random


# Function to calculate the distance between two cities
def distance(city1, city2):
    return math.sqrt((city1[0] - city2[0])**2 + (city1[1] - city2[1])**2)


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
            selected_edges = nearest_neighbour(cities)
            print("Selected Edges: ", selected_edges)
            

# call the main function
if __name__ == '__main__':
    main()