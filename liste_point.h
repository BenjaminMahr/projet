#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"

#ifndef LISTE_POINT_H_
#define LISTE_POINT_H_

typedef struct point point;
struct point
{
    int x;
    int y;
};

typedef struct liste_point *liste_point;
struct liste_point
{
    point premier;
    liste_point reste;
} ;

liste_point l_vide () ;
bool est_vide (liste_point L) ;
point prem(liste_point L);
liste_point reste (liste_point L) ;
void ecrire_prem (point, liste_point L) ;
void ecrire_reste (liste_point R, liste_point L) ;
liste_point cons (point a, liste_point L) ;
int presenceR(point a, liste_point L);
liste_point supprimerR(point a, liste_point L);
point remplisPoint(int col,int lig);
void afficher_point_liste(liste_point L);
liste_point renverser_liste(liste_point L);


#endif /* LISTE_POINT_H_ */

