#ifndef BNB_METHOD_PROBLEM_H
#define BNB_METHOD_PROBLEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Problems **/

#define KNAPSACK_PROBLEM 0
#define SCHEDULING_PROBLEM 1

typedef struct {
		int type;
		int constraint; // Problem constraint
		int n_items;    // Number of items
		int *weights;   // Weights/Duration of every item
		int *values;    // Values of the items (1 for every item in a scheduling problem)
} problem_t;

problem_t *problem_create_from_file(const char *filename);

void problem_print(problem_t *problem);

void problem_destroy(problem_t *problem);

/** Solutions **/

#define INFEASIBLE (-1)
#define OPTIMAL 0

typedef struct {
		int status;
		int *item_statuses;
		problem_t *problem;
} solution_t;

solution_t *solution_create(problem_t *problem);

void solution_print(solution_t *solution);

void solution_set_status(solution_t *solution, int status);

void solution_destroy(solution_t *solution);

#endif //BNB_METHOD_PROBLEM_H
