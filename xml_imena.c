#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

long print_element_names_imena( FILE *ulaz , xmlNode *a_node , long *i , long *brojac ) {
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

		print_element_names_imena(ulaz,cur_node->children,i,brojac);
	}
	
	return( *i );
}

void print_element_names_putevi( FILE *ulaz , xmlNode *a_node) {
	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {

			if (!strcmp(cur_node->name, "way")) {
				xmlChar *id = xmlGetProp(cur_node, "id");
				if (id != NULL) {
					fprintf(ulaz,"way %s\n", id);

					xmlNode *childNode = cur_node->children;
					for (; childNode; childNode = childNode->next) {
						if (!strcmp("tag", childNode->name)) {
							xmlChar *key = xmlGetProp(childNode, "k");
							xmlChar *value = xmlGetProp(childNode, "v");

							if (key != NULL && value != NULL && ( !strcmp( key , "name" ) || !strcmp( key , "oneway" ) || !strcmp( key , "operator" ) ) ) {
								fprintf(ulaz,"%s %s\n", key, value);

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

		print_element_names_putevi(ulaz,cur_node->children);
	}
}
// gcc xml_imena.c -I/usr/include/libxml2 -o a.out -lxml2
int main() {
	char filename[] = "proba.osm"; //.osm
    long i = 0 , brojac = 0;
    
    FILE *ulaz_imena = fopen( "pevac_cvorovi.txt" , "w" ) , *ulaz_putevi = fopen( "pevac_putevi.txt" , "w" ); //one dve txt datoteke

	xmlDoc *doc1 = xmlReadFile(filename, "UTF-8", 0);
	if (doc1 == NULL) {
		fprintf(stderr, "Failed to parse %s\n", filename);
		return -1;
	}
	
	xmlNode *root_imena = xmlDocGetRootElement(doc1);

	i = print_element_names_imena(ulaz_imena , root_imena , &i , &brojac );
	
	xmlFreeDoc(doc1);
	xmlCleanupParser();
    
    fclose( ulaz_imena );
    
	xmlDoc *doc2 = xmlReadFile(filename, "UTF-8", 0);
	if (doc2 == NULL) {
		fprintf(stderr, "Failed to parse %s\n", filename);
		return -1;
	}

	xmlNode *root_putevi = xmlDocGetRootElement(doc2);
    
    fprintf( ulaz_putevi , "%ld\n" , i );

	print_element_names_putevi(ulaz_putevi , root_putevi);

	xmlFreeDoc(doc2);
	xmlCleanupParser();

    fclose( ulaz_putevi );

	return 0;
}
