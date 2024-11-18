# TSP Solver and Analysis Project

This project is designed to solve the Traveling Salesman Problem (TSP), analyze the results, and generate a LaTeX report summarizing the findings.

## Project Structure

- **`TSP_assignment_task_benchmark_data/`**: Directory containing TSP test cases in the form of `.tsp` files.
- **`cse_318_TSP_assignment_task.pdf`**: Folder containing LaTeX templates and scripts to generate the final report.
- **`2005021.py`**: Python script to solve TSP problems and generate a CSV file with the results.
- **`output_table.csv`**: Generated sample output. 
- **`2005021_utility.py`**: Python script to analyze the generated CSV file and produce graphs.
- **`2005021_report/`**: Folder containing LaTeX templates and scripts to generate the final report.
- **`2005021_report.pdf`**: The final report.

## Usage

### Step 1: Solve the TSP
Place your `.tsp` test case files in the `TSP_assignment_task_benchmark_data/` directory. Run the following command to solve the TSP problems:

```bash
python 2005021.py
```
This will generate a results.csv file in the same directory, containing the solutions to the TSP test cases.

### Step 2: Analyze Results
Use the 2005021_utility.py script to analyze the results and generate visualizations:

```bash
python 2005021_utility.py
```
This will generate graphs and analysis based on the results.csv file.