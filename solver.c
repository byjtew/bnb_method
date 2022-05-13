#include <assert.h>
#include "solver.h"

#define ITEM_UNDEFINED 0
#define ITEM_SELECTED 1
#define ITEM_NOT_SELECTED 2

#pragma region Node class

typedef struct {
		float upper_bound;
		int next_item_index;
		int *items_states;
		int n_items;
} node_t;

node_t *node_t_create(const problem_t *problem) {
	node_t *node = (node_t *) malloc(sizeof(node_t));
	node->upper_bound = 0;
	node->next_item_index = -1;
	node->n_items = problem->n_items;
	node->items_states = (int *) calloc(node->n_items, sizeof(int)); // All items are assigned to ITEM_UNDEFINED
	return node;
}

void *node_t_copy(const void *elem) {
	node_t *node = (node_t *) elem;
	node_t *copy = (node_t *) malloc(sizeof(node_t));
	copy->upper_bound = node->upper_bound;
	copy->next_item_index = node->next_item_index;
	copy->n_items = node->n_items;
	copy->items_states = (int *) calloc(copy->n_items, sizeof(int));
	memcpy(copy->items_states, node->items_states, copy->n_items * sizeof(int));
	return copy;
}

void node_t_destroy(void *node) {
	if (node == NULL) return;
	node_t *n = (node_t *) node;
	free(n->items_states);
	free(n);
}

int compare_upper_bound(const void *elm_a, const void *elm_b) {
	if (elm_a == NULL || elm_b == NULL) return 0;
	node_t *a = (node_t *) elm_a;
	node_t *b = (node_t *) elm_b;
	return a->upper_bound < b->upper_bound;
}

#pragma endregion

char *state_to_string(int state) {
	switch (state) {
		case ITEM_UNDEFINED:
			return "UNDEFINED";
		case ITEM_SELECTED:
			return "SELECTED";
		case ITEM_NOT_SELECTED:
			return "NOT_SELECTED";
		default:
			return "UNKNOWN";
	}
}

int pick_max_ratio_item(const problem_t *problem, int *is_picked) {
	int max_ratio_idx = -1;
	float max_ratio = 0;
	for (int i = 0; i < problem->n_items; i++) {
		if (!is_picked[i]) {
			float ratio = ((float) problem->values[i]) / ((float) problem->weights[i]);
			if (ratio > max_ratio) {
				max_ratio = ratio;
				max_ratio_idx = i;
			}
		}
	}
	return max_ratio_idx;
}

float compute_upper_bound(node_t *n, problem_t *pb) {
	float upperBound = 0;
	int rest_weight = pb->constraint;

	int *is_picked = calloc(n->n_items, sizeof(int));

	for (int i = 0; i < n->n_items; i++) {
		int idx = pick_max_ratio_item(pb, is_picked);
		if (idx >= 0 && n->items_states[idx] != ITEM_NOT_SELECTED) {
			if (pb->weights[idx] <= rest_weight) {
				upperBound += (float) pb->values[idx];
				rest_weight -= pb->weights[idx];
				continue;
			} else {
				float ratio = ((float) pb->values[idx]) / ((float) pb->weights[idx]);
				upperBound += (float) rest_weight * ratio;
				return upperBound;
			}
		}
	}

	free(is_picked);

	return upperBound;
}

int compute_lower_bound(const node_t *n, const problem_t *pb) {
	int current_constraint = pb->constraint;
	int total_value = 0;

	for (int i = 0; i < pb->n_items; i++) {
		if (n->items_states[i] != ITEM_SELECTED) continue;

		if (current_constraint < pb->weights[i]) return INFEASIBLE;
		current_constraint -= pb->weights[i];
		total_value += pb->values[i];
	}

	return total_value;
}

knapsack_solution_t *solve_knapsack(problem_t *problem) {
	assert(problem->type == KNAPSACK_PROBLEM);

	knapsack_solution_t *solution = knapsack_solution_create(problem);

	int lower_bound = 0;

	sorted_queue_t *tree = squeue_create(sizeof(node_t), compare_upper_bound, node_t_copy, node_t_destroy);

	node_t *optimal_node = node_t_create(problem);
	node_t *root = node_t_create(problem);
	root->next_item_index = 0;
	root->upper_bound = compute_upper_bound(root, problem);
	squeue_enqueue(tree, root);
	int expanded_nodes = 0;

	while (!squeue_is_empty(tree)) {
		node_t *node = (node_t *) squeue_dequeue(tree);
		if (node->upper_bound < (float) lower_bound) {
			printf("\tXX Discarding node %d with upper bound %f < %d\n", expanded_nodes, node->upper_bound, lower_bound);
			continue;
		}
		printf("\t-- Expanding node %d with upper bound %f\n", expanded_nodes, node->upper_bound);

		if (node->next_item_index == problem->n_items) {
			int lb = compute_lower_bound(node, problem);
			if (lb != INFEASIBLE && lb > lower_bound) {
				lower_bound = lb;
				node_t_destroy(optimal_node);
				optimal_node = node_t_copy(node);
				printf("\t-- New lower bound: %d\n", lower_bound);
			} else if (lb == INFEASIBLE) {
				printf("\t-- Node infeasible, lb = %d\n", lb);
			}
		} else {
			int next_item_index = node->next_item_index;
			node_t *selected = node_t_copy(node);
			selected->items_states[next_item_index] = ITEM_SELECTED;
			selected->next_item_index++;
			selected->upper_bound = compute_upper_bound(selected, problem);
			squeue_enqueue(tree, selected);

			node_t *discarded = node_t_copy(node);
			discarded->items_states[next_item_index] = ITEM_NOT_SELECTED;
			discarded->next_item_index++;
			discarded->upper_bound = compute_upper_bound(discarded, problem);
			squeue_enqueue(tree, discarded);

			expanded_nodes += 2;
		}
	}

	printf("Expanded nodes: %d\n", expanded_nodes);

	// Print the optimal solution
	printf("\n** Optimal solution: %d\n", lower_bound);
	for (int i = 0; i < optimal_node->n_items; i++)
		printf("%s ", state_to_string(optimal_node->items_states[i]));


	node_t_destroy(root);
	squeue_destroy(tree);
	return solution;
}