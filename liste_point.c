
#include "liste_point.h"


point remplisPoint(int col, int lig)
{
  // Reservation de la place memoire neccessaire pour une cellule :
  point p ;

  p.x = col;
  p.y = lig;

  return p;
}

liste_point l_vide (void)
{
  return NULL;
}

bool est_vide(liste_point L)
{
  return L == NULL;
}

point prem(liste_point L)
{
  return L->premier;
}

liste_point reste(liste_point L)
{
  return L->reste;
}

void ecrire_prem (point a, liste_point L)
{
  L->premier = a ;
}

void ecrire_reste (liste_point R, liste_point L)
{
    L->reste = R ;
}

liste_point cons(point a, liste_point L)
{
  liste_point M ;

  // Reservation de la place memoire neccessaire pour une cellule :
  M = malloc (sizeof (*M)) ;

  ecrire_prem(a,M);
  ecrire_reste(L,M);
  return M;
}

int presenceR(point a, liste_point L)
{
  if(est_vide(L))
    {
      printf("non present\n");
      return 0;
    }
  if( (prem(L).x == a.x) & (prem(L).y == a.y) )
    {
      printf("present\n");
      return 1;
    }
  return presenceR(a,reste(L));
}


liste_point supprimerR(point a, liste_point L)
{
  if(est_vide(L))
    {
      return l_vide();
    }

  if((a.x == prem(L).x) & (a.y == prem(L).y) )
    {
      return supprimerR(a,reste(L));
    }
  return cons(prem(L),supprimerR(a,reste(L))) ;
}

void afficher_point_liste(liste_point L)
{
  if(est_vide(L))
    {
      printf("l_vide\n");
      return;
    }
  printf("(%d, %d)\n",prem(L).x,prem(L).y);
  return afficher_point_liste(reste(L));
}

liste_point renverser_liste(liste_point L)
{
  liste_point R, M ;
  R = l_vide () ;
  M = L ;
  while (!est_vide(M))
    {
      R = cons (prem(M), R) ;
      M = reste (M) ;
    }
  return R ;

}

