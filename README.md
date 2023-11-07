# navigationC

Or simply put, pathfind through an .osm in C using Dijkstra's algorithm

This is a project I wrote for my university Algorithms class. The program takes an .osm file (that is obtained through openStreetMaps) and parses it so that is represented by text files. Later on, it uses linked lists and the dijkstra algorithm to determine the distance between two points.

It uses an xml parser to convert the .osm file to .txt. It gives us 3 .txt files:
- cvorovi.txt: shows a node's id and it's x and y coordinates
- imena.txt: shows the name of a node
- putevi.txt: shows all the nodes connected to one way

The code was compiled on Linux terminal using:
- gcc graf_lista.c -I/usr/include/libxml2 -o a.out -lxml2 -lm
