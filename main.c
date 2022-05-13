#include <stdio.h>
#include "queue.h"
#include "problem.h"
#include "solver.h"


int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: %s <input_file>\n", argv[0]);
		return 1;
	}

	problem_t *problem = problem_create_from_file(argv[1]);

	if (problem == NULL) {
		printf("Error: could not read problem from file\n");
		return 1;
	}

	problem_print(problem);


	knapsack_solution_t *knapsackSolution = solve_knapsack(problem);


	// Memory de-allocation(s)
	problem_destroy(problem);
	knapsack_solution_destroy(knapsackSolution);

	return 0;
}
