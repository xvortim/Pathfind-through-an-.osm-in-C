#include <stdio.h>
#include "fun_graf.h"
#include "xml.h"

/*
 Kod radi na principu way, jer je tesko da se cirilica provlaci kroz program
 */

#define START 26000421 // start street way
#define END 195144104 // end street way

int main() {
    int way_count = files_road();
        STREET *ways = NULL;
        ways = files_names(way_count);
    
    FILE *roads;
        ID *ids = NULL;
        int SIZE;

    roads = opener( "putevi.txt", "r"); // first line has amount of nodes
    fscanf( roads , "%d" , &SIZE);

    Graph *graph = new_graph( SIZE );
    ids = fillID( SIZE, "cvorovi.txt" );
    //
    fillGraph( graph, roads, ids );
    //
    BIG start_way = START;
    BIG end_way = END;

    dijkstra( graph, ids, ways, start_way, end_way );

    free(ids);
    free(ways);
    free(graph);
    fclose(roads);

    return 0;
}
