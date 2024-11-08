import os


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
            

# call the main function
if __name__ == '__main__':
    main()