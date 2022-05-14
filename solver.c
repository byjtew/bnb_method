#include <stdarg.h>
#include "solver.h"

node_t *node_t_create(const problem_t *problem) {
	node_t *node = (node_t *) malloc(sizeof(node_t));
	node->upper_bound = 0;
	node->next_item_index = -1;
	node->n_items = problem->n_items;
	node->items_states = (int *) malloc(node->n_items * sizeof(int));
	memset(node->items_states, ITEM_UNDEFINED, node->n_items * sizeof(int));
	if (problem->type == N_KNAPSACK_PROBLEM) {
		node->items_attributions = (int *) malloc(node->n_items * sizeof(int));
		memset(node->items_attributions, ITEM_NOT_ATTRIBUTED, node->n_items * sizeof(int));
	} else {
		node->items_attributions = NULL;
	}
	// All items are assigned to ITEM_UNDEFINED
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
	if (node->items_attributions != NULL) {
		copy->items_attributions = (int *) calloc(copy->n_items, sizeof(int));
		memcpy(copy->items_attributions, node->items_attributions, copy->n_items * sizeof(int));
	}
	return copy;
}

void node_t_destroy(void *node) {
	if (node == NULL) return;
	node_t *n = (node_t *) node;
	free(n->items_states);
	if (n->items_attributions != NULL) free(n->items_attributions);
	free(n);
}

int compare_upper_bound(const void *elm_a, const void *elm_b) {
	if (elm_a == NULL || elm_b == NULL) return 0;
	node_t *a = (node_t *) elm_a;
	node_t *b = (node_t *) elm_b;
	return a->upper_bound < b->upper_bound;
}

void verbose_printf(const char *format, ...) {
#ifdef VERBOSE
#if VERBOSE == 1
	// Cyan print
	printf("\033[36m");
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	// Reset print
	printf("\033[0m");
#endif
#endif
}


void info_printf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}


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

int pick_max_ratio_item(const problem_t *problem, const int *is_picked) {
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


