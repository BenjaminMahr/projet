
#include <stdio.h>
#include <stdlib.h>
#include "liste_point.h"

const int width  = 16;
const int height = 16;


int main() {
	int i,j;
	liste_point	LF;
	point_cout pNode;
	LF = l_vide();
	char map[height][height];
	int dist[height][height];
	int dx = 1, dy = 1, fx = 1, fy = 10;
	int d;

	for (i=0; i<height; i++) {
		for (j=0; j<width; j++) {
			map[i][j] = 1;
			dist[i][j] = 40000;
		}
	}
	dist[dx][dy] = 0;

//	for (j=0; j<width-5; j++) {
//		map[5][j] = '#';
//	}

	for (d=0;d<width*height;d++)
	{
		for (i=0; i<height; i++)
		{
			for (j=0; j<width; j++)
			{
				if (dist[i][j]!=d)
				{
					continue;
				}
				if (i>0 && map[i-1][j  ]== 1 && dist[i-1][j  ]>d+1)
				{
					dist[i-1][j  ] = d+1;
				}
				if (j>0 && map[i  ][j-1]== 1 && dist[i  ][j-1]>d+1)
				{
					dist[i  ][j-1] = d+1;
				}
				if (i<height-1 && map[i+1][j  ]== 1 && dist[i+1][j  ]>d+1)
				{
					dist[i+1][j  ] = d+1;
				}
				if (j<width -1 && map[i  ][j+1]== 1 && dist[i  ][j+1]>d+1)
				{
					dist[i  ][j+1] = d+1;
				}
			}
		}
	}

	d = dist[fy][fx];
	if (d>=1000) {
		return 0;
	}
 	printf("dist: %d\n", dist[fy][fx]);


	for (;d>0;d--) {
		if (fy>0 && dist[fy-1][fx  ]==d-1) {
			fy--;
			pNode = remplisPoint(fx,fy,0);
			LF = cons(pNode,LF);
			continue;}
		if (fx>0 && dist[fy  ][fx-1]==d-1) {
			fx--;
			pNode = remplisPoint(fx,fy,0);
			LF = cons(pNode,LF);
			continue; }
		if (fy<height+1 && dist[fy+1][fx  ]==d-1) {
			fy++;
			pNode = remplisPoint(fx,fy,0);
			LF = cons(pNode,LF);
			continue; }
		if (fx<width+1  && dist[fy  ][fx+1]==d-1) {
			fx++;
			pNode = remplisPoint(fx,fy,0);
			LF = cons(pNode,LF);
			continue; }
		printf("%d %d\n", fx, fy);
	}

//
//	for (i=0; i<height; i++) {
//		for (j=0; j<width; j++) {
//			printf("%c", map[i][j]);
//		}
//		printf("\n");
//	}

	afficher_point_liste(LF);
	return 0;
}
