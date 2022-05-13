#include "stack.h"

#include <stdlib.h>
#include <string.h>

stack_t *stack_create() {
	stack_t *stack = (stack_t *) malloc(sizeof(stack_t));
	stack->capacity = STACK_PAGE_SIZE;
	stack->elements = (int *) calloc(stack->capacity, sizeof(int));
	stack->top = 0;
	return stack;
}

int stack_empty(const stack_t *stack) {
	return stack->top == 0;
}

stack_t *stack_clone(const stack_t *stack) {
	stack_t *clone = stack_create();
	clone->top = stack->top;
	clone->capacity = stack->capacity;
	memcpy(clone->elements, stack->elements, stack->top * sizeof(int));
	return clone;
}

int stack_pop(stack_t *stack) {
	if (stack->top == 0) return -1;
	int element = stack->elements[stack->top - 1];
	stack->top--;
	return element;
}

void stack_push(stack_t *stack, int element) {
	if (stack->top >= stack->capacity) {
		stack->elements = (int *) realloc(stack->elements, sizeof(int) * stack->capacity + STACK_PAGE_SIZE);
		stack->capacity += STACK_PAGE_SIZE;
	}
	stack->elements[stack->top++] = element;
}

void stack_destroy(stack_t *stack) {
	free(stack->elements);
	free(stack);
}
