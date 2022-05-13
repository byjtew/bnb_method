#ifndef BNB_METHOD_SOLVER_H
#define BNB_METHOD_SOLVER_H

#include "problem.h"
#include "queue.h"

solution_t *solve_knapsack(problem_t *problem);

solution_t *solve_scheduling(problem_t *problem);

#endif //BNB_METHOD_SOLVER_H
