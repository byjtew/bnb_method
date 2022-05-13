#ifndef BNB_METHOD_SOLVER_H
#define BNB_METHOD_SOLVER_H

#include "problem.h"
#include "queue.h"


#include <assert.h>

#pragma region Node class

#define ITEM_UNDEFINED -1
#define ITEM_SELECTED -2
#define ITEM_NOT_SELECTED -3

typedef struct {
		float upper_bound;
		int next_item_index;
		int *items_states;
		int n_items;
} node_t;

node_t *node_t_create(const problem_t *problem);
void *node_t_copy(const void *elem);
void node_t_destroy(void *node);

#pragma endregion

#pragma region Utils

int compare_upper_bound(const void *elm_a, const void *elm_b);
char *state_to_string(int state);
int pick_max_ratio_item(const problem_t *problem, const int *is_picked);

#pragma endregion

solution_t *solve_knapsack(problem_t *problem);

solution_t *solve_scheduling(problem_t *problem);

#endif //BNB_METHOD_SOLVER_H
