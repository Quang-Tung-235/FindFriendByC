#include <stdio.h>
#include "graph.h"
#include "datatables.h"
#include "dataloader.h"
#include "cli.h"

int main() {
    Graph graph;
    HobbyTable hobby_table = {0};
    HabitTable habit_table = {0};

    graph_init(&graph);
    if (!load_hobby_table("data/hobbyc.txt", &hobby_table)) {
        printf("LOI: Khong the nap bang so thich.\n");
        return 1;
    }
    if (!load_habit_table("data/habitc.txt", &habit_table)) {
        printf("LOI: Khong the nap bang thoi quen.\n");
        return 1;
    }

    if (!load_students("data/student_info.txt", &graph, &hobby_table, &habit_table)) {
        printf("LOI: Khong the nap du lieu sinh vien.\n");
        return 1;
    }
    printf("\n===DANH SACH SO THICH===\n");
    for (int i=1; i<hobby_table.count; i++) {
        printf("%d . %s\n", i, hobby_table.list[i-1]);
    }
    printf("\n===DANH SACH THOI QUEN===\n");
    for (int i=1; i<habit_table.count; i++) {
        printf("%d . %s\n", i, habit_table.list[i-1]);
    }


    cli_run(&graph, &hobby_table, &habit_table);

    return 0;
}
