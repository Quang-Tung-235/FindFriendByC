#ifndef STUDENT_H
#define STUDENT_H
#define MAX_ID_LEN 20
#define MAX_NAME_LEN 50
#define MAX_HOBBIES 10
#define MAX_HABITS 10
#define MAX_FRIENDS 20

typedef struct {
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    char hobbies[MAX_HOBBIES][MAX_NAME_LEN];
    int hobbies_count;
    char habits[MAX_HABITS][MAX_NAME_LEN];
    int habits_count;
    char friends[MAX_FRIENDS][MAX_ID_LEN];
    int friends_count;
} Student;

// Thêm sở thích, thói quen, bạn bè
int student_add_hobby(Student* s, const char* hobby);
int student_add_habit(Student* s, const char* habit);
int student_add_friend(Student* s, const char* friend_id);
int student_remove_friend(Student* s, const char* friend_id);
int student_is_friend(const Student* s, const char* friend_id);
#endif