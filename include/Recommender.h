#ifndef RECOMMENDER_H
#define RECOMMENDER_H

#include "graph.h"

#define MAX_POTENTIAL_CANDIDATES 100
#define MAX_LEN 50

typedef struct {
    char id[MAX_LEN];
    double score;
} Candidate;

void recommend_friends(Graph* g, const char* user_id, int k, double weight_mutual, double weight_jaccard);

#endif
