#include "fun_podaci.h"
#include "limits.h"

#define INF 9999

// Graph

Vertex* new_vertex( int key, BIG id, double weight ) {
    Vertex *new_node = ( Vertex* )malloc( sizeof(Vertex) );

    new_node->weight = weight;
    new_node->id = id;
    new_node->key = key;

    new_node->next = NULL;

    return new_node;
}

Graph* new_graph( int V ) {
    Graph *graph = ( Graph * )malloc(sizeof( Graph ) );

    graph->V = V;
    graph->arr = ( Edge* )malloc( V * sizeof( Edge ) );

    for( int i = 0; i < V; ++i )
        graph->arr[i].head = NULL;

    return graph;
}

double haversine(double lat1, double lon1, double lat2, double lon2) {
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;

    lat1 = (lat1) * M_PI / 180.0;
    lat2 = (lat2) * M_PI / 180.0;

    double a = pow(sin(dLat / 2), 2) +pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
    double rad = 6371;
    double c = 2 * asin(sqrt(a));

    return 1000 * rad * c;
}

void new_edge( Graph *graph, int first, int last, ID *arr ) {
    double weight = haversine( (arr + first)->lat, (arr + first)->lon, (arr + last)->lat, (arr + last)->lon );

    Vertex *new_node = new_vertex( last, (arr + last)->id, weight );
    new_node->next = graph->arr[first].head;
    graph->arr[first].head = new_node;

    new_node = new_vertex( first, (arr+first)->id, weight );
    new_node->next = graph->arr[last].head;
    graph->arr[last].head = new_node;
}

// ID

ID *fillID( int SIZE, char *file ) {
    FILE *nodes;
    nodes = opener( file, "r" );

    int con, indikator = 1, key = 0;
    BIG id;
    double lon, lat;

    ID *arr = NULL;
    arr = ( ID * )realloc( arr , sizeof( ID ) * ( SIZE + 1 ) );
    if( arr == NULL ) exit( EXIT_FAILURE );

    while( indikator ) {
       con = fscanf(nodes,"%ld %lf %lf", &id , &lat, &lon );

       indikator = (con!=EOF);

       if( indikator ) {
            ( arr + key )->key = key;
            ( arr + key )->id = id;
            ( arr + key )->lat = lat;
            ( arr + key )->lon = lon;

            key++;
		};
    }
    fclose(nodes);

    return arr;
}

// Graph <> ID

int findID( ID *arr, BIG id ) {
    int id_key, i = 0;

    while( (arr + i )->id != id )
        i++;

    return ( arr + i )->key;
}

void fillGraph( Graph *graph, FILE *roads, ID *arr ) {
    int id_key, temp_key, con, pass = 0, indikator = 1;
    char name[40], test[40];
    BIG id, temp_id, way;

    while( indikator ) {
        con = fscanf(roads,"%s %ld", name , &id );
        indikator = (con!=EOF);

        if( indikator ) {
            if( !strcmp(name,"id") ) {
                if(!pass) {
                    temp_id = id;
                }
                else if( pass ) {
                    temp_key = findID(arr, temp_id); id_key = findID(arr, id);

                    new_edge(graph, temp_key, id_key, arr );
                    ( arr + id_key )->way = way;
                    ( arr + temp_key )->way = way;

                    temp_id = id;
                }
                pass++;
            }
            if(!strcmp(name, "way")) {
                way = id;

                pass = 0;
            }
		}
    }
}

// Fin

void print_way( STREET *arr, BIG way ) {
    int i = 0;

    while( (arr + i )->way != way )
        i++;

    printf("%s", ( arr + i )->name);
}

int findWayID( Graph *graph, BIG way, ID *arr ) {
    BIG id;

    for( int i = 0; i < graph->V; i++ ) {
        if( (arr+i)->way == way ) {
            return (arr+i)->key;
        }
    }
}

int find_min_dist( int* dist, int* pos, int size )
{
	int max = INT_MAX;
	int min;

	for( int i = 0 ; i < size ; i++ )
		if( !( *( pos + i ) ) && *( dist + i ) <= max )
		{
			min = i;
		    max = *( dist + i );
		}

	return min;
}

void print_path( Graph *graph , ID *arr, STREET *ways, int *parent, int cur, BIG last )
{
	/* Prints every node on the way
	if( *( parent + cur ) == -1 )
		return;

	print_path( graph , arr, parent, *( parent + cur ), last );
	printf( "%ld\n", (arr + cur)->id );
    */

    while( *( parent + cur ) != -1 ) {
        if( (arr + cur)->way != last) {
            print_way( ways, ( arr + cur )->way );
            printf("\n");
        }
        last = (arr+cur)->way;
        cur = *( parent + cur );
    }
}

void dijkstra( Graph *graph, ID *arr, STREET *ways, BIG source , BIG dest )
{
	int *pos = NULL, *dist = NULL, *parent = NULL;

	int end = findWayID( graph, dest, arr );
	int start = findWayID( graph, source, arr );

	// Initialize
	pos = ( int * ) calloc( graph->V , sizeof( int ) );
	if( pos == NULL ) exit( EXIT_FAILURE );

	dist = ( int * ) malloc( sizeof( int ) * graph->V );
	if( dist == NULL ) exit( EXIT_FAILURE );

	parent = ( int * ) malloc( sizeof( int ) * graph->V );
	if( parent == NULL ) exit( EXIT_FAILURE );
	//

	for( int i = 0 ; i < graph->V ; i++ )
	{
		*( dist + i ) = INF;
		*( parent + i ) = -1;
	}

	*( dist + start ) = 0;

	for( int i = 0 ; i < graph->V - 1 ; i++ )
	{
		int cur = find_min_dist( dist, pos, graph->V );

		*( pos + cur ) = 1;

		Vertex *node = graph->arr[cur].head;

		while( node != NULL )
		{
			int temp = node->key;

			if( !( *( pos + temp ) ) && *( dist + cur ) + node->weight < *( dist + temp ) )
			{
				*( dist + temp ) = *( dist + cur ) + node->weight;
				*( parent + temp ) = cur;
			}
			node = node->next;
		}
	}

    print_way( ways, source ); printf(" -> "); print_way( ways, dest ); 
	printf("\nDistanca: %d[m]\n", *( dist + end ) );
    printf("----------------------------------------------\n\n");

	print_path( graph , arr, ways, parent, end, -1 );

	free( pos );
	free( parent );
	free( dist );
}


void print( Graph *graph, ID *arr ) {
    printf("Printed graph:\n");
    for( int i = 0; i < graph->V; i++ ) {
        Vertex *temp = graph->arr[i].head;
        printf("%d", i );
        while( temp ) {
            printf(" -> %ld (%ld)", temp->id, (arr + temp->key )->way  );
            printf(" <%lf>",  temp->weight);
            temp = temp->next;
        }
        printf("\n");
    }
}
