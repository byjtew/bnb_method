#include "problem.h"

knapsack_solution_t *knapsack_solution_create(problem_t *problem) {
	knapsack_solution_t *solution = malloc(sizeof(knapsack_solution_t));
	solution->status = INFEASIBLE; // default
	solution->item_statuses = calloc(problem->n_items, sizeof(int));
	solution->problem = problem;
	return solution;
}

char *solution_status_to_string(int status) {
	switch (status) {
		case INFEASIBLE:
			return "INFEASIBLE";
		case OPTIMAL:
			return "OPTIMAL";
		default:
			return "UNKNOWN";
	}
}

void knapsack_solution_print(knapsack_solution_t *solution) {
	printf("-- Solution status: %s\n", solution_status_to_string(solution->status));
	printf("-- Item statuses:\n");
	for (int i = 0; i < solution->problem->n_items; i++) {
		printf("\tItem %d: %s\n", i, solution->item_statuses[i] ? "keep" : "discard");
	}
	printf("\n");
}

void knapsack_solution_set_status(knapsack_solution_t *solution, int status) {
	solution->status = status;
}

void knapsack_solution_destroy(knapsack_solution_t *solution) {
	if (solution == NULL) return;
	free(solution->item_statuses);
	free(solution);
}