#pragma once
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>



typedef unsigned  weight_t;

typedef struct graph_t
{
    weight_t** weight_table;
    size_t vertex_num;
}
graph_t;



graph_t*
GraphConstructor (const size_t vertex_num);

graph_t
GraphDestructor (graph_t const graph);

graph_t
ReadGraph (void);

weight_t*
InitD (const size_t vertex_num,
       const size_t root_index);
