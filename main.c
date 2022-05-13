#include <stdio.h>
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

	solution_t *solution = (problem->type == KNAPSACK_PROBLEM) ? solve_knapsack(problem) : solve_scheduling(problem);

	// Memory de-allocation(s)
	problem_destroy(problem);
	solution_destroy(solution);

	return 0;
}
