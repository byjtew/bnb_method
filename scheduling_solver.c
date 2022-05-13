#include "solver.h"

float scheduling_compute_upper_bound(node_t *n, problem_t *pb) {
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
				upperBound += (float) rest_weight / (float) pb->weights[idx];
				return upperBound;
			}
		}
	}

	free(is_picked);

	return upperBound;
}

int scheduling_compute_lower_bound(const node_t *n, const problem_t *pb) {
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


solution_t *solve_scheduling(problem_t *problem) {
	assert(problem->type == SCHEDULING_PROBLEM);

	solution_t *solution = solution_create(problem);

	int lower_bound = 0;

	sorted_queue_t *tree = squeue_create(sizeof(node_t), compare_upper_bound, node_t_copy, node_t_destroy);

	node_t *optimal_node = node_t_create(problem);
	node_t *root = node_t_create(problem);
	root->next_item_index = 0;
	root->upper_bound = scheduling_compute_upper_bound(root, problem);
	squeue_enqueue(tree, root);
	int expanded_nodes = 0;

	while (!squeue_is_empty(tree)) {
		node_t *node = (node_t *) squeue_dequeue(tree);
		if (node->upper_bound < (float) lower_bound) continue;

		if (node->next_item_index == problem->n_items) {
			int lb = scheduling_compute_lower_bound(node, problem);
			if (lb != INFEASIBLE && lb > lower_bound) {
				lower_bound = lb;
				node_t_destroy(optimal_node);
				optimal_node = node_t_copy(node);
			} else if (lb == INFEASIBLE) {
			}
		} else {
			int next_item_index = node->next_item_index;
			for(int m=0; m<problem->n_slots; m++) {
				node_t *child = node_t_copy(node);

			}
			node_t *selected = node_t_copy(node);
			selected->items_states[next_item_index] = ITEM_SELECTED;
			selected->next_item_index++;
			selected->upper_bound = scheduling_compute_upper_bound(selected, problem);
			squeue_enqueue(tree, selected);
			node_t_destroy(selected);

			node_t *discarded = node_t_copy(node);
			discarded->items_states[next_item_index] = ITEM_NOT_SELECTED;
			discarded->next_item_index++;
			discarded->upper_bound = scheduling_compute_upper_bound(discarded, problem);
			squeue_enqueue(tree, discarded);
			node_t_destroy(discarded);

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

