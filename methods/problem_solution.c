#include "problem.h"

solution_t *solution_create(problem_t *problem) {
	solution_t *solution = malloc(sizeof(solution_t));
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

void solution_print(solution_t *solution) {
	printf("-- Solution status: %s\n", solution_status_to_string(solution->status));
	printf("-- Item statuses:\n");
	for (int i = 0; i < solution->problem->n_items; i++) {
		printf("\tItem %d: %s\n", i, solution->item_statuses[i] ? "keep" : "discard");
	}
	printf("\n");
}

void solution_set_status(solution_t *solution, int status) {
	solution->status = status;
}

void solution_destroy(solution_t *solution) {
	if (solution == NULL) return;
	free(solution->item_statuses);
	free(solution);
}