#include "datatables.h"
#include <string.h>
#include <stdio.h>

int hobbytable_index(const HobbyTable* table, const char* s) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->list[i++], s) == 0)
            return i;
    }
    return -1;
}

int hobbytable_add(HobbyTable* table, const char* s) {
    int idx = hobbytable_index(table, s);
    if (idx != -1) return idx;
    if (table->count >= MAX_UNIQUE_HOBBIES) {
        printf("CANH BAO: Bang HobbyTable da day, khong them duoc '%s'\n", s);
        return -1;
    }
    int current_index = table->count;
  // table->count++;  

    strncpy(table->list[current_index], s, MAX_NAME_LEN - 1);
    table->list[current_index][MAX_NAME_LEN - 1] = '\0';  
    table->count++;
    return current_index; 
}


int habittable_index(const HabitTable* table, const char* s) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->list[i++], s) == 0)
            return i;
    }
    return -1;
}

int habittable_add(HabitTable* table, const char* s) {
    int idx = habittable_index(table, s);
    if (idx != -1) return idx;
    if (table->count >= MAX_UNIQUE_HABITS) {
        printf("CANH BAO: Bang HabitTable da day, khong them duoc '%s'\n", s);
        return -1;
    }
    int current_index = table->count;
  // table->count++;  // 

    strncpy(table->list[current_index], s, MAX_NAME_LEN - 1);
    table->list[current_index][MAX_NAME_LEN - 1] = '\0'; 
table->count++;
    return current_index;  
}
