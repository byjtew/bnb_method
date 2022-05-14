/** Auteur: Benjamin Lozes
 *  Classe: M1 CHPS
 */


#include "solver.h"

float n_knapsack_compute_upper_bound(node_t *n, problem_t *pb) {
	float *upper_bounds = (float *) calloc(pb->n_slots, sizeof(float));
	int *rest_weights = malloc(pb->n_slots * sizeof(int));
	memset(rest_weights, pb->constraint, pb->n_slots * sizeof(int));

	int *is_picked = calloc(n->n_items, sizeof(int));

	for (int i = 0; i < n->n_items; i++) {
		int idx = pick_max_ratio_item(pb, is_picked);
		if (idx >= 0 && n->items_states[idx] != ITEM_NOT_SELECTED) {
			int slot_idx = n->items_attributions[idx];
			if (pb->weights[idx] <= rest_weights[slot_idx]) {
				upper_bounds[slot_idx] += (float) pb->values[idx];
				rest_weights[slot_idx] -= pb->weights[idx];
			} else {
				upper_bounds[slot_idx] += (float) rest_weights[slot_idx] / (float) pb->weights[idx];
				rest_weights[slot_idx] = 0;
			}
		}
	}

	float max_upper_bound = 0;
	for (int i = 0; i < pb->n_slots; i++)
		if (upper_bounds[i] > max_upper_bound)
			max_upper_bound = upper_bounds[i];

	free(is_picked);
	free(rest_weights);
	free(upper_bounds);

	return max_upper_bound;
}

void print_node_n_knapsack(const node_t *n, const problem_t *pb, void(*print_func)(const char *, ...)) {
	int *total_values = calloc(pb->n_slots, sizeof(int));
	int *total_weights = calloc(pb->n_slots, sizeof(int));
	int *jobs_per_slot = calloc(pb->n_slots, sizeof(int));

	for (int i = 0; i < n->n_items; i++) {
		if (n->items_states[i] == ITEM_NOT_SELECTED) continue;
		int slot_idx = n->items_attributions[i];
		total_values[slot_idx] += pb->values[i];
		total_weights[slot_idx] += pb->weights[i];
		jobs_per_slot[slot_idx]++;
	}
	int max_jobs_per_slot = 0;
	for (int s = 0; s < pb->n_slots; s++)
		if (jobs_per_slot[s] > max_jobs_per_slot)
			max_jobs_per_slot = jobs_per_slot[s];


	print_func("\n");
	for (int s = 0; s < pb->n_slots; s++) {
		print_func("Slot %.02d: | ", s);
		int jobs_for_this_slot = 0;
		for (int i = 0; i < n->n_items; i++) {
			int slot_idx = n->items_attributions[i];
			if (slot_idx == s && n->items_states[i] != ITEM_NOT_SELECTED) {
				print_func("%.02d | ", i);
				jobs_for_this_slot++;
			}
		}
		for (int i = 0; i < max_jobs_per_slot - jobs_for_this_slot; i++)
			print_func("-- | ");

		print_func(" --> {weight: %d  value: %d}\n", total_weights[s], total_values[s]);
	}

	free(total_values);
	free(total_weights);
	free(jobs_per_slot);
}

int n_knapsack_compute_lower_bound(const node_t *n, const problem_t *pb) {
	int *current_constraints = calloc(pb->n_slots, sizeof(int)); // Current constraints, per slot
	for (int i = 0; i < pb->n_slots; i++) current_constraints[i] = pb->constraint;
	int *values = calloc(pb->n_slots, sizeof(int)); // Values of the items, per slot
	int total_value = 0;

	for (int i = 0; i < n->n_items; i++) {
		if (n->items_states[i] != ITEM_SELECTED) continue;
		int slot_idx = n->items_attributions[i];
		if (current_constraints[slot_idx] < pb->weights[i]) return INFEASIBLE;
		current_constraints[slot_idx] -= pb->weights[i];
		values[slot_idx] += pb->values[i];

	}

	for (int i = 0; i < pb->n_slots; i++)
		total_value += values[i];

	if (total_value == pb->n_items) {
		for (int i = 0; i < pb->n_slots; i++) {
			verbose_printf("Slot %d: | ", i);
			for (int j = 0; j < pb->n_items; j++)
				if (n->items_states[j] == ITEM_SELECTED && n->items_attributions[j] == i) verbose_printf("%.02d | ", j);
				else verbose_printf("-- | ");
			verbose_printf(" --> {value: %d  constraint: %d}\n", values[i], current_constraints[i]);
		}
		verbose_printf("-> total_value: %d\n\n", total_value);
	}

	free(current_constraints);
	free(values);

	return total_value;
}


solution_t *solve_n_knapsack(problem_t *problem) {
	verbose_printf("Solving n-knapsack problem...\n");
	assert(problem->type == N_KNAPSACK_PROBLEM);

	solution_t *solution = solution_create(problem);

	int lower_bound = 0;

	sorted_queue_t *tree = squeue_create(sizeof(node_t), compare_upper_bound, node_t_copy, node_t_destroy);

	node_t *optimal_node = node_t_create(problem);
	node_t *root = node_t_create(problem);
	root->next_item_index = 0;
	root->upper_bound = n_knapsack_compute_upper_bound(root, problem);
	squeue_enqueue(tree, root);
	node_t_destroy(root);
	int expanded_nodes = 0;

	while (!squeue_is_empty(tree)) {
		node_t *node = (node_t *) squeue_dequeue(tree);
		if (node->upper_bound < (float) lower_bound) continue;

		if (node->next_item_index == problem->n_items) {
			int lb = n_knapsack_compute_lower_bound(node, problem);
			// Print this node in cyan
			verbose_printf("lower_bound: %d\n", lb);
			print_node_n_knapsack(node, problem, verbose_printf);

			if (lb != INFEASIBLE && lb > lower_bound) {
				lower_bound = lb;
				node_t_destroy(optimal_node);
				optimal_node = node_t_copy(node);
			}
		} else {
			int next_item_index = node->next_item_index;
			for (int m = 0; m < problem->n_slots; m++) {
				node_t *selected = node_t_copy(node);
				selected->items_states[next_item_index] = ITEM_SELECTED;
				selected->items_attributions[next_item_index] = m;
				selected->next_item_index++;
				selected->upper_bound = n_knapsack_compute_upper_bound(selected, problem);
				squeue_enqueue(tree, selected);
				node_t_destroy(selected);
				expanded_nodes++;
			}
			node_t *discarded = node_t_copy(node);
			discarded->items_states[next_item_index] = ITEM_NOT_SELECTED;
			discarded->items_attributions[next_item_index] = ITEM_NOT_ATTRIBUTED;
			discarded->next_item_index++;
			discarded->upper_bound = n_knapsack_compute_upper_bound(discarded, problem);
			squeue_enqueue(tree, discarded);
			node_t_destroy(discarded);
			expanded_nodes++;
		}
	}

	if (optimal_node->upper_bound == INFEASIBLE) {
		info_printf("No solution found.\n");
		solution->status = INFEASIBLE;
		for (int i = 0; i < problem->n_items; i++)
			solution->item_statuses[i] = ITEM_NOT_SELECTED;
	} else {


		info_printf("** Expanded nodes: %d\n", expanded_nodes);
		// Print the optimal solution
		info_printf("** Optimal solution: %d\n", lower_bound);
		info_printf("** Attributions: ");
		for (int i = 0; i < optimal_node->n_items; i++)
			info_printf("%d ", optimal_node->items_attributions[i]);
		info_printf("\n");

		print_node_n_knapsack(optimal_node, problem, info_printf);

		solution->status = OPTIMAL;
		for(int i = 0; i < optimal_node->n_items; i++)
			solution->item_statuses[i] = optimal_node->items_attributions[i];
	}

	node_t_destroy(optimal_node);
	squeue_destroy(tree);

	return solution;
}

