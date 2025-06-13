#include "dataloader.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LEN 512
#define MAX_TOKENS 50

// Hàm tách chuỗi đơn giản bằng strtok
static void simple_split(char* input, char delimiter, char tokens[][MAX_NAME_LEN], int* count) {
    *count = 0;
    char delim[2] = {delimiter, ' '};
    char* token = strtok(input, delim);
    while (token != NULL && *count < MAX_TOKENS) {
        strncpy(tokens[(*count)++], token, MAX_NAME_LEN - 1);
        tokens[*count - 1][MAX_NAME_LEN - 1] = ' ';
        token = strtok(NULL, delim);
    }
}

int load_hobby_table(const char* filename, HobbyTable* table) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("LOI: Khong mo duoc file %s\n", filename);
        return 0;
    }
    char line[MAX_LINE_LEN];
    table->count = 0;
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = 0;
        if (line[0] == ' ') continue;
        hobbytable_add(table, line);
    }
    fclose(f);
    return 1;
}

int load_habit_table(const char* filename, HabitTable* table) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("LOI: Khong mo duoc file %s\n", filename);
        return 0;
    }
    char line[MAX_LINE_LEN];
    table->count = 0;
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = 0;
        if (line[0] == ' ') continue;
        habittable_add(table, line);
    }
    fclose(f);
    return 1;
}

int load_students(const char* filename, Graph* graph, HobbyTable* hobby_table, HabitTable* habit_table) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("LOI: Khong mo duoc file %s\n", filename);
        return 0;
    }
    char line1[MAX_LINE_LEN];
    char line2[MAX_LINE_LEN];
    char line3[MAX_LINE_LEN];
    char line4[MAX_LINE_LEN];
    char line5[MAX_LINE_LEN];

    graph_init(graph);

    while (fgets(line1, sizeof(line1), f)) {
        if (!fgets(line2, sizeof(line2), f)) break;
        if (!fgets(line3, sizeof(line3), f)) break;
        if (!fgets(line4, sizeof(line4), f)) break;
        if (!fgets(line5, sizeof(line5), f)) break;

        // Xóa ký tự xuống dòng
        line1[strcspn(line1, "\r\n")] = 0;
        line2[strcspn(line2, "\r\n")] = 0;
        line3[strcspn(line3, "\r\n")] = 0;
        line4[strcspn(line4, "\r\n")] = 0;
        line5[strcspn(line5, "\r\n")] = 0;

        Student s = {0};
        strncpy(s.id, line1, MAX_ID_LEN - 1); 
        strncpy(s.name, line2, MAX_NAME_LEN - 1);

        // 1. Thêm bạn bè
        char* token = strtok(line3, " \t");
        while (token != NULL) {
            student_add_friend(&s, token);
            token = strtok(NULL, " \t");
        }

        // 2. Thêm sở thích từ hobby_table
        token = strtok(line4, " \t");
        while (token != NULL) {
            int idx = atoi(token);
            if (idx >= 0 && idx < hobby_table->count) {
                student_add_hobby(&s, hobby_table->list[idx-1]);
            }
            token = strtok(NULL, " \t");
        }

        // 3. Thêm thói quen từ habit_table
        token = strtok(line5, " \t");
        while (token != NULL) {
            int idx = atoi(token);
            if (idx >= 0 && idx < habit_table->count) {
                student_add_habit(&s, habit_table->list[idx-1]);
            }
            token = strtok(NULL, " \t");
        }

        graph_add_student(graph, &s);
    }

    fclose(f);
    return 1;
}