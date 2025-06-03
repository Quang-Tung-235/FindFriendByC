#include "graph.h"
#include <string.h>

void graph_init(Graph* g) {
    g->student_count = 0;
}

int graph_find_student_index(const Graph* g, const char* id) {
    for (int i=0; i<g->student_count; i++) {
        if (strcmp(g->students[i].id, id) == 0) return i;
    }
    return -1;
}

Student* graph_get_student(Graph* g, const char* id) {
    int idx = graph_find_student_index(g, id);
    if (idx == -1) return NULL;
    return &g->students[idx];
}

int graph_add_student(Graph* g, const Student* s) {
    if (g->student_count >= MAX_STUDENTS) return 0;
    if (graph_find_student_index(g, s->id) != -1) return 0;
    g->students[g->student_count++] = *s;
    return 1;
}

int graph_add_friendship(Graph* g, const char* id1, const char* id2) {
    if (strcmp(id1, id2) == 0) return 0;
    Student* s1 = graph_get_student(g, id1);
    Student* s2 = graph_get_student(g, id2);
    if (!s1 || !s2) return 0;
    int res1 = student_add_friend(s1, id2);
    int res2 = student_add_friend(s2, id1);
    return res1 && res2;
}

int graph_remove_friendship(Graph* g, const char* id1, const char* id2) {
    Student* s1 = graph_get_student(g, id1);
    Student* s2 = graph_get_student(g, id2);
    if (!s1 || !s2) return 0;
    int res1 = student_remove_friend(s1, id2);
    int res2 = student_remove_friend(s2, id1);
    return res1 || res2;
}
