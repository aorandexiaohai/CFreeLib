#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cdouble_list.h"
int cmp_int(generic_data_t d1, generic_data_t d2) { return ((int)(size_t)d1 - (int)(size_t)d2); }
int cmp_str(generic_data_t d1, generic_data_t d2) { return strcmp((char *)d1, (char *)d2); }
int main() {
    {
        printf("test 1\n");
        double_list_t *l = create_double_list(NULL);
        push_back_double_list(l, (void *)(size_t)(1));
        push_back_double_list(l, (void *)(size_t)(2));
        push_back_double_list(l, (void *)(size_t)(3));
        assert(l->size == 3);
        void *data = NULL;
        while ((data = pop_front_double_list(l))) {
            printf("%d ", (int)((size_t)data));
        }
        assert(l->size == 0);
        printf("\n****************************************\n");
        free_double_list(l);
    }
    {
        printf("test 2\n");
        double_list_t *l = create_double_list(NULL);
        double_list_node_t *n1 = push_back_double_list(l, (void *)"1a");
        double_list_node_t *n2 = push_back_double_list(l, (void *)"2bb");
        double_list_node_t *n3 = push_back_double_list(l, (void *)"3ccc");
        assert(l->size == 3);
        remove_double_list(l, n1);
        assert(l->size == 2);
        remove_double_list(l, n2);
        assert(l->size == 1);
        remove_double_list(l, n3);
        assert(l->size == 0);

        printf("\n****************************************\n");
        free_double_list(l);
    }

    {
        printf("test 3\n");
        double_list_t *l = create_double_list(NULL);
        push_back_double_list(l, (void *)"1a");
        push_back_double_list(l, (void *)"2bb");
        push_back_double_list(l, (void *)"3ccc");

        printf("\n****************************************\n");
        free_double_list(l);
    }

    {
        printf("test 4\n");
        double_list_t *l = create_double_list(free);
        push_back_double_list(l, malloc(1));
        push_back_double_list(l, malloc(2));
        push_back_double_list(l, malloc(3));

        printf("\n****************************************\n");
        free_double_list(l);
    }

    {
        printf("test 5\n");
        double_list_t *l = create_double_list(free);
        double_list_node_t *n1 = push_back_double_list(l, malloc(1));
        double_list_node_t *n2 = push_back_double_list(l, malloc(2));
        double_list_node_t *n3 = push_back_double_list(l, malloc(3));
        assert(l->size == 3);
        remove_double_list(l, n1);
        assert(l->size == 2);
        remove_double_list(l, n2);
        assert(l->size == 1);
        remove_double_list(l, n3);
        assert(l->size == 0);

        printf("\n****************************************\n");
        free_double_list(l);
    }

    {
        printf("test 6\n");
        double_list_t *l = create_double_list1(NULL, cmp_int);
        insert_double_list(l, (void *)(size_t)(3));
        insert_double_list(l, (void *)(size_t)(-1));
        insert_double_list(l, (void *)(size_t)(2));
        insert_double_list(l, (void *)(size_t)(1));
        assert(l->size == 4);
        void *data = NULL;
        while ((data = pop_front_double_list(l))) {
            printf("%d ", (int)((size_t)data));
        }
        assert(l->size == 0);
        printf("\n****************************************\n");
        free_double_list(l);
    }

    {
        printf("test 7\n");
        double_list_t *l = create_double_list1(NULL, cmp_int);
        insert_double_list(l, (void *)(size_t)(3));
        insert_double_list(l, (void *)(size_t)(-1));
        insert_double_list(l, (void *)(size_t)(2));
        insert_double_list(l, (void *)(size_t)(1));
        assert(find_double_list1(l, (void *)(size_t)(3))->data == (void *)(size_t)(3));
        assert(find_double_list1(l, (void *)(size_t)(-1))->data == (void *)(size_t)(-1));
        assert(find_double_list1(l, (void *)(size_t)(2))->data == (void *)(size_t)(2));
        assert(find_double_list1(l, (void *)(size_t)(1))->data == (void *)(size_t)(1));
        assert(l->size == 4);
        void *data = NULL;
        while ((data = pop_front_double_list(l))) {
            printf("%d ", (int)((size_t)data));
        }
        assert(l->size == 0);
        printf("\n****************************************\n");
        free_double_list(l);
    }

    {
        printf("test 8\n");
        double_list_t *l = create_double_list1(NULL, cmp_str);
        double_list_node_t *n1 = push_back_double_list(l, (void *)"bb");
        double_list_node_t *n2 = push_back_double_list(l, (void *)"cc");
        double_list_node_t *n3 = push_back_double_list(l, (void *)"acc");
        assert(l->size == 3);
        remove_double_list(l, n1);
        assert(l->size == 2);
        remove_double_list(l, n2);
        assert(l->size == 1);
        remove_double_list(l, n3);
        assert(l->size == 0);

        printf("\n****************************************\n");
        free_double_list(l);
    }

    return 0;
}