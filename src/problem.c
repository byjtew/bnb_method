/** Auteur: Benjamin Lozes
 *  Classe: M1 CHPS
 */


#include "problem.h"

int safe_int_fscanf(FILE *fp, int *ret) {
	char str_int[10];
	int scanf_ret = fscanf(fp, "%s", str_int);
	*ret = (int) strtol(str_int, NULL, 10);
	if (*ret < 0) {
		fprintf(stderr, "Error: negative number in input file\n");
		exit(1);
	}
	return scanf_ret;
}

problem_t *problem_create(int type, int constraint, int n_items, int n_slots, int *weights, int *values) {
	problem_t *problem = malloc(sizeof(problem_t));
	problem->type = type;
	problem->constraint = constraint;
	problem->n_items = n_items;
	problem->n_slots = n_slots;

	// Copy the weights and values
	problem->weights = malloc(sizeof(int) * constraint);
	problem->values = malloc(sizeof(int) * constraint);
	memcpy(problem->weights, weights, n_items * sizeof(int));
	memcpy(problem->values, values, n_items * sizeof(int));

	return problem;
}

problem_t *problem_create_from_file(const char *filename) {
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "Could not open file %s\n", filename);
		exit(1);
	}

	// While the first character is a #, skip the line
	int c;
	while ((c = fgetc(file)) == '#')
		while (c != '\n') c = fgetc(file);
	fseek(file, -1, SEEK_CUR);

	// Read the problem type
	int problem_type, nb_slots = 0;
	char type[32];
	fscanf(file, "%s", type);
	if (strcmp(type, "n-knapsack") == 0) {
		problem_type = N_KNAPSACK_PROBLEM;
		safe_int_fscanf(file, &nb_slots);
		if (nb_slots <= 0) {
			fprintf(stderr, "Invalid number of slots: %d, must greater than 0\n", nb_slots);
			exit(1);
		}
	} else if (strcmp(type, "knapsack") == 0) {
		problem_type = KNAPSACK_PROBLEM;
		printf("Reading knapsack problem\n");
	} else {
		fprintf(stderr, "Unknown problem type %s\n", type);
		exit(1);
	}

	// Read the constraint
	int constraint;
	safe_int_fscanf(file, &constraint);

	// Read each line of the file
	int *weights = malloc(sizeof(int) * 1);
	int *values = malloc(sizeof(int) * 1);
	int count = 0;
	while (1) {
		int ret = safe_int_fscanf(file, &weights[count]);
		if (ret == EOF) goto end_read;

		ret = safe_int_fscanf(file, &values[count]);
		if (ret == EOF) goto end_read;

		count++;
		weights = realloc(weights, sizeof(int) * (count + 1));
		values = realloc(values, sizeof(int) * (count + 1));
	}
	end_read:;

	fclose(file);

	problem_t *pb = problem_create(problem_type, constraint, count, nb_slots, weights, values);

	free(weights);
	free(values);

	return pb;
}

char *problem_type_to_string(int type) {
	switch (type) {
		case N_KNAPSACK_PROBLEM:
			return "n-knapsack";
		case KNAPSACK_PROBLEM:
			return "knapsack";
		default:
			return "unknown";
	}
}

void problem_print(problem_t *problem) {
	printf("* Problem type: %s\n", problem_type_to_string(problem->type));
	printf("* Constraint: %d\n", problem->constraint);
	if (problem->type == N_KNAPSACK_PROBLEM)
		printf("* Number of slots: %d\n", problem->n_slots);
	printf("* Number of items: %d\n", problem->n_items);
	for (int i = 0; i < problem->n_items; i++) {
		printf("\tItem %d: {weight %.2d,  value %.2d}\n", i, problem->weights[i], problem->values[i]);
	}
	printf("\n");
}

void problem_destroy(problem_t *problem) {
	if (problem == NULL) return;
	free(problem->weights);
	free(problem->values);
	free(problem);
}