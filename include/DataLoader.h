#ifndef DATALOADER_H
#define DATALOADER_H
#include "graph.h"
#include "datatables.h"

int load_hobby_table(const char* filename, HobbyTable* table);
int load_habit_table(const char* filename, HabitTable* table);
int load_students(const char* filename, Graph* graph, HobbyTable* hobby_table, HabitTable* habit_table);
int load_friendships(const char* filename, Graph* graph);
#endif