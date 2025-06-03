#ifndef GRAPH_H
#define GRAPH_H
#include "student.h"

#define MAX_STUDENTS 100

typedef struct {
    Student students[MAX_STUDENTS];
    int student_count;
} Graph;

void graph_init(Graph* g);
int graph_add_student(Graph* g, const Student* s);
int graph_find_student_index(const Graph* g, const char* id);
Student* graph_get_student(Graph* g, const char* id);
int graph_add_friendship(Graph* g, const char* id1, const char* id2);
int graph_remove_friendship(Graph* g, const char* id1, const char* id2);
#endif