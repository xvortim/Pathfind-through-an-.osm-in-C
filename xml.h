#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

char *move_str( char *string ) {
    char *pom = NULL;
    int num = 0;
    int con = 1;
    
    while( con ) {
        con = ( *( string + num) != '\0' ) ? 1 : 0;
        
        pom = ( char * )realloc( pom, sizeof( char ) * ( num + 1 ) );
        if( pom == NULL ) exit( EXIT_FAILURE );
        
        *( pom + num ) = *( string + num );
        
        num++;
    }
    
    return pom;
}

long print_element_names_cvorovi( FILE *ulaz , xmlNode *a_node , long *i , long *brojac ) {
	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {

			if (!strcmp(cur_node->name, "node")) {
				xmlChar *id = xmlGetProp(cur_node, "id");
                xmlChar *lat = xmlGetProp(cur_node, "lat");
                xmlChar *lon = xmlGetProp(cur_node, "lon");
                xmlChar *vers = xmlGetProp(cur_node, "version");
                
				if (id != NULL) {
					fprintf(ulaz,"%s %s %s\n",id,lat,lon);
                    
                    (*i)++;

					free(id);
				}

			}
		}
		
		xmlChar *k = xmlGetProp(cur_node , "k" );
                
        if( k != NULL ) (*brojac)++;

		print_element_names_cvorovi(ulaz,cur_node->children,i,brojac);
	}
	
	return( *i );
}

void print_element_names_putevi( FILE *ulaz , xmlNode *a_node, int *val ) {
	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {

			if (!strcmp(cur_node->name, "way")) {
				xmlChar *id = xmlGetProp(cur_node, "id");
				if (id != NULL) {
					fprintf(ulaz,"way %s\n", id);
                    (*val)++;

					xmlNode *childNode = cur_node->children;
					for (; childNode; childNode = childNode->next) {
						if (!strcmp("tag", childNode->name)) {
							xmlChar *key = xmlGetProp(childNode, "k");
							xmlChar *value = xmlGetProp(childNode, "v");

							if (key != NULL && value != NULL && ( !strcmp( key , "name" ) || !strcmp( key , "oneway" ) || !strcmp( key , "operator" ) ) ) {
								// fprintf(ulaz,"%s %s\n", key, value);

								free(key);
								free(value);
							}
						}
						if (!strcmp("nd", childNode->name)) {
							xmlChar *node_id = xmlGetProp(childNode, "ref");

							if (node_id != NULL) {
								fprintf(ulaz,"id %s\n", node_id);

								free(node_id);

							}
						}
					}

					free(id);
				}

			}
		}

		print_element_names_putevi(ulaz,cur_node->children, val );
	}
}

void print_element_names_imena( FILE *ulaz , xmlNode *a_node, STREET *arr, int cur ) {
	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {

			if (!strcmp(cur_node->name, "way")) {
				xmlChar *id = xmlGetProp(cur_node, "id");
				if (id != NULL) {
					fprintf(ulaz,"%s:\n", id);

					xmlNode *childNode = cur_node->children;
					for (; childNode; childNode = childNode->next) {
						if (!strcmp("tag", childNode->name)) {
							xmlChar *key = xmlGetProp(childNode, "k");
							xmlChar *value = xmlGetProp(childNode, "v");

							if (key != NULL && value != NULL && ( !strcmp( key , "name" ) ) ) {
								fprintf(ulaz,"%s\n", value);
                                ( arr + cur )->name = move_str(value);
							} 
                            
                            free(key);
                            free(value);
						}
					}
					
					( arr + cur )->key = cur;
                    ( arr + cur++ )->way = atoi(id);
                    
					free(id);
				}

			}
		}

		print_element_names_imena(ulaz,cur_node->children,arr,cur);
	}
}

// gcc graf_lista.c -I/usr/include/libxml2 -o a.out -lxml2 -lm
int files_road() {
	char filename[] = "map.osm"; //.osm
    long i = 0 , brojac = 0;
    int way_count = 0;
    
    FILE *ulaz_cvorovi = fopen( "cvorovi.txt" , "w" ) , *ulaz_putevi = fopen( "putevi.txt" , "w" );
    
    // cvorovi

	xmlDoc *doc1 = xmlReadFile(filename, "UTF-8", 0);
	if (doc1 == NULL) {
		fprintf(stderr, "Failed to parse %s\n", filename);
		return -1;
	}
	
	xmlNode *root_cvorovi = xmlDocGetRootElement(doc1);

	i = print_element_names_cvorovi(ulaz_cvorovi , root_cvorovi , &i , &brojac );
	
	xmlFreeDoc(doc1);
	xmlCleanupParser();
    
    fclose( ulaz_cvorovi );
    
    // putevi
    
	xmlDoc *doc2 = xmlReadFile(filename, "UTF-8", 0);
	if (doc2 == NULL) {
		fprintf(stderr, "Failed to parse %s\n", filename);
		return -1;
	}

	xmlNode *root_putevi = xmlDocGetRootElement(doc2);
    
    fprintf( ulaz_putevi , "%ld\n" , i );
	print_element_names_putevi(ulaz_putevi , root_putevi, &way_count);

	xmlFreeDoc(doc2);
	xmlCleanupParser();

    fclose( ulaz_putevi );
    
    return way_count;
}

STREET *files_names(int way_count) {
    char filename[] = "map.osm"; //.osm
    FILE *ulaz_imena = fopen( "imena.txt" , "w" );
    //printf("-->%d\n\n", way_count);
    
    //
    STREET *arr = NULL;
    arr = ( STREET * )realloc( arr , sizeof( STREET ) * (way_count + 1) );
    if( arr == NULL ) exit( EXIT_FAILURE );
    //
    
    xmlDoc *doc1 = xmlReadFile(filename, "UTF-8", 0);
	if (doc1 == NULL) {
		fprintf(stderr, "Failed to parse %s\n", filename);
	}
	
	xmlNode *root_imena = xmlDocGetRootElement(doc1);
    int key = 0;
    print_element_names_imena(ulaz_imena , root_imena, arr, key);
	
	xmlFreeDoc(doc1);
	xmlCleanupParser();
    
    fclose( ulaz_imena );
    
    return arr;
}
