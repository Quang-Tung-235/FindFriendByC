#ifndef DATATABLES_H
#define DATATABLES_H
#define MAX_UNIQUE_HOBBIES 50
#define MAX_UNIQUE_HABITS 50
#define MAX_NAME_LEN 50

typedef struct {
    char list[MAX_UNIQUE_HOBBIES][MAX_NAME_LEN];
    int count;
} HobbyTable;

typedef struct {
    char list[MAX_UNIQUE_HABITS][MAX_NAME_LEN];
    int count;
} HabitTable;

int hobbytable_index(const HobbyTable* table, const char* s);
int hobbytable_add(HobbyTable* table, const char* s);

int habittable_index(const HabitTable* table, const char* s);
int habittable_add(HabitTable* table, const char* s);
#endif