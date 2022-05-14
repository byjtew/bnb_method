/** Auteur: Benjamin Lozes
 *  Classe: M1 CHPS
 */


#ifndef BND_METHOD_QUEUE_H
#define BND_METHOD_QUEUE_H


#define QUEUE_PAGE_SIZE 128

typedef struct {
		void **elements;
		int first, last;
		int capacity;
		long sizeof_element;

		void *(*copy_func)(const void *);

		void (*free_func)(void *);

		int (*compare_func)(const void *, const void *);
} sorted_queue_t;

sorted_queue_t *
squeue_create(long sizeof_element, int(*compare_func)(const void *, const void *), void *(*copy_func)(const void *),
              void (*free_func)(void *));

int squeue_is_empty(sorted_queue_t *queue);

void *squeue_dequeue(sorted_queue_t *queue);

void squeue_enqueue(sorted_queue_t *queue, void *element);

void squeue_destroy(sorted_queue_t *queue);

#endif //BND_METHOD_QUEUE_H
