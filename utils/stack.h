#ifndef BND_METHOD_STACK_H
#define BND_METHOD_STACK_H


#define STACK_PAGE_SIZE 128

typedef struct {
		int *elements;
		int top;
		int capacity;
} stack_t;

stack_t *stack_create();

int stack_empty(const stack_t *stack);

stack_t *stack_clone(const stack_t *stack);

int stack_pop(stack_t *stack);

void stack_push(stack_t *stack, int element);

void stack_destroy(stack_t *stack);

#endif //BND_METHOD_STACK_H
