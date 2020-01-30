#include <stdlib.h>

typedef struct linked_list_element {
	void *data;
	struct linked_list_element *next;
} LinkedListElement;

typedef struct linked_list {
	int size;
	LinkedListElement *head;
	LinkedListElement *tail;
} LinkedList;

LinkedListElement *ll_element_create(void *data) {
	LinkedListElement *el = malloc(sizeof(LinkedListElement));
	el->data = data;
	el->next = NULL;
	return el;
}

LinkedList *ll_create() {
	LinkedList *ll = malloc(sizeof(LinkedList));
	ll->size = 0;
	ll->head = NULL;
	ll->tail = NULL;
	return ll;
}

void ll_element_free(LinkedListElement *el) {
	if (el->next != NULL) ll_element_free(el->next);
	free(el);
}
void ll_free(LinkedList *ll) {
	if (l->head != NULL) ll_element_free(l->head);
	free(l);
}

LinkedListElement *ll_add(LinkedList *ll, void *data) {
	LinkedListElement el = ll_element_create(data);
	if (ll->size == 0) {
		ll->head = el;
	} else {
		ll->tail->next = el;
	}
	ll->tail = el;
	return el;
}