#include "graph.h"



weight_t*
FibHeapDijkstra (const graph_t* const graph,
                 const size_t root_index)
{
    assert (graph);

    weight_t* d = InitD (graph->vertex_num, root_index);
    assert (d);

    bool* a = (bool*) calloc (graph->vertex_num, sizeof (bool));
    assert (a);


}
