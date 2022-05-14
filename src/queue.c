/** Auteur: Benjamin Lozes
 *  Classe: M1 CHPS
 */


#include "queue.h"

#include <stdlib.h>
#include <stdio.h>

sorted_queue_t *squeue_create(long sizeof_element, int(*compare_func)(const void *, const void *), void *(*copy_func)(const void *),
                              void (*free_func)(void *)) {
	sorted_queue_t *queue = (sorted_queue_t *) malloc(sizeof(sorted_queue_t));
	queue->sizeof_element = sizeof_element;
	queue->compare_func = compare_func;
	queue->copy_func = copy_func;
	queue->free_func = free_func;
	queue->capacity = QUEUE_PAGE_SIZE;
	queue->elements = (void **) calloc(queue->capacity, sizeof_element);
	queue->first = 0;
	queue->last = 0;
	return queue;
}

int squeue_is_empty(sorted_queue_t *queue) {
	return queue->first == queue->last;
}


void *squeue_dequeue(sorted_queue_t *queue) {
	if (queue->first >= queue->last) {
		fprintf(stderr, "squeue_dequeue: queue is empty\n");
		exit(1);
	}
	void *element = queue->elements[queue->first++];
	if(squeue_is_empty(queue)) {
		queue->first = 0;
		queue->last = 0;
	}
	return element;
}

void squeue_enqueue(sorted_queue_t *queue, void *element) {
	if (queue->last >= queue->capacity) {
		queue->capacity += QUEUE_PAGE_SIZE;
		queue->elements = (void **) realloc(queue->elements, queue->sizeof_element * queue->capacity);
	}

	// Sorted insert (optimized)
	int i = queue->last;
	while (i > queue->first && queue->compare_func(element, queue->elements[i - 1]) < 0) {
		queue->elements[i] = queue->elements[i - 1];
		i--;
	}
	queue->elements[i] = queue->copy_func(element);
	queue->last++;

	// Sorted insert (the brutal way)
	/*queue->elements[queue->last++] = queue->copy_func(element);
	qsort(queue->elements + queue->first, queue->last - queue->first, queue->compare_func);*/
}


void squeue_destroy(sorted_queue_t *queue) {
	for (int i = queue->first; i < queue->last; i++)
		queue->free_func(queue->elements[i]);
	free(queue->elements);
	free(queue);
}