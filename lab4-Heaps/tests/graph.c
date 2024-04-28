#include "graph.h"



graph_t*
GraphConstructor (const size_t vertex_num)
{
    graph_t* const graph =
        (graph_t*) malloc (sizeof (graph_t));
    if (graph == NULL) return NULL;

    graph->weight_table =
        (weight_t**) malloc (vertex_num * vertex_num * sizeof (weight_t));
    if (graph->weight_table == NULL)
        return GraphDestructor (graph);

    graph->vertex_num = vertex_num;

    return graph;
}

graph_t*
GraphDestructor (graph_t* const graph)
{
    if (graph == NULL) return NULL;

    free (graph->weight_table);
    free (graph);
    return NULL;
}

graph_t*
ReadGraph (void)
{
    size_t vertex_num = 0;
    if (scanf ("%zu", &vertex_num) != 1) return NULL;

    graph_t* graph = GraphConstructor (vertex_num);
    if (graph == NULL) return NULL;

    weight_t weight = 0;

    for (size_t i = 0; i < vertex_num; ++i)
    {
        for (size_t j = 0; j < i; ++j)
        {
            if (scanf ("%zu", &weight) != 1)
                return GraphDestructor (graph);

            graph->weight_table[i * vertex_num + j] = weight;
            graph->weight_table[j * vertex_num + i] = weight;
        }

        graph->weight_table[i * vertex_num + i] = 0;
    }

    return graph;
}

weight_t*
InitD (const size_t vertex_num,
       const size_t root_index)
{
    assert (root_index < vertex_num);

    weight_t* const d =
        (weight_t*) malloc(vertex_num * sizeof(weight_t));
    assert (d);

    for (size_t i = 0; i < vertex_num; ++i)
        d[i] = UINT_MAX;

    d[root_index] = 0;

    return d;
}
