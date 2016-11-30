#include "SDL.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//#include <SDL/SDL_mixer.h>
#include "liste_point.h"

#define SCREEN_WIDTH  768
#define SCREEN_HEIGHT 640
#define SPRITE_WIDTH 31
#define SPRITE_HEIGHT 31
#define SPRITE_SIZE 31
#define NY 20
#define NX 24
#define G1_WIDTH 31
#define G1_HEIGHT 31
#define COL 24
#define LIG 20
#define TIME_BTW_ANIMATIONS 40
//#define TIME_BTW_MOVEMENTS 10
int TIME_BTW_MOVEMENTS = 3 ;

#define taille 31

int gameover;
int moveRight, moveLeft, moveUp, moveDown; //gere le déplacement du pacman
int wantRight, wantLeft, wantUp, wantDown;
int currentTime, previousTime, fant_time; //gerer le temps entre les deplacements
int currentTimeAnim, previousTimeAnim; //gerer le temps entre les animations
int time_game;
int time_game_eat;

/* source and destination rectangles */
SDL_Rect rcSrc,rcWall,rcWall2, rcBloc, rcCoeur, rcSprite,rcG1,rcSG1, rcG2,rcG3, rcCandy, rcCandy2;
int i,j;

//musMix_Music *music, *start, *scream, *pilule, *die, *siren;
//musint Mix_OpenAudio(int frequency, Uint16 format, int channels, int chunksize);	

enum direction{
	right,
	left,
	up,
	down,
};

void deplacement(SDL_Rect *fant, int x, int y){
	fant->x += x;
	fant->y += y;
}

void deplacementBleu(int tab[NY][NX], SDL_Rect *fant, int *a, int *b){
	if(*a == 0){
		int temp;
		temp = rand()%4;
		switch(temp){
			case 0: //gauche 
				if(tab[fant->y/32][fant->x / 32 - 1] != 1 && tab[fant->y/32][fant->x / 32 -1] != 3)
					*a = 1;
				break;
			case 1: // droite 
				if(tab[fant->y/32][fant->x / 32 + 1] != 1 && tab[fant->y/32][fant->x / 32 +1] != 3)
					*a = 2;
				break;
			case 2: // haut
				if(tab[fant->y/32 - 1][fant->x/32] != 1 && tab[fant->y/32 - 1][fant->x/32] != 3)
					*a = 3;
				break;
			case 3: // bas
				if(tab[fant->y/32 + 1][fant->x/32] != 1 && tab[fant->y/32 + 1][fant->x/32] != 3)
					*a = 4;
				break;
		}
		if(*a == 0)
			return deplacementBleu(tab,fant,a,b);
	}
	
	if(*a == 1){
		deplacement(fant,-1,0);
		*b += 1;
	}
	if(*a == 2){
		deplacement(fant,-1,0);
		*b += 1;
	}
	if(*a == 3){
		deplacement(fant,0,1);
		*b += 1;
	}
	if(*a == 4){
		deplacement(fant,0,1);
		*b += 1;
	}
	
	if(*b == 32){
		*b = 0;
		*a = 0;
	}
	

	/*deplacement(fant,0,-2*32);
	deplacement(fant,4*32,0);
	deplacement(fant,0,4*32);
	deplacement(fant,6*-32,0);
	deplacement(fant,0,-4*32);
	deplacement(fant,2*32,0);*/
}


void deplacementFantomeBlanc(int tab[NY][NX],SDL_Rect *fant, int *a, int *b)
{
	if(*a == 0){
		
		if(moveLeft){
			if(tab[fant->y/32][fant->x / 32 + 1] != 1 && tab[fant->y/32][fant->x / 32 +1] != 3)
				*a = 1;
		}
		if(moveRight){
			if(tab[fant->y/32][fant->x/32 - 1] != 1 && tab[fant->y/32][fant->x/32 - 1] != 3)
				*a = 2;
		}
		if(moveUp){
			if(tab[fant->y/32 + 1][fant->x/32] != 1 && tab[fant->y/32 + 1][fant->x/32] != 3)
				*a = 3;
		}
		if(moveDown){
			if(tab[fant->y/32 - 1][fant->x/32] != 1 && tab[fant->y/32 - 1][fant->x/32] != 3)
				*a = 4;
		}
	}

	if(*a == 1){
		deplacement(fant,1,0);
		*b += 1;
	}
	if(*a == 2){
		deplacement(fant,-1,0);
		*b += 1;
	}
	if(*a == 3){
		deplacement(fant,0,1);
		*b += 1;
	}
	if(*a == 4){
		deplacement(fant,0,-1);
		*b += 1;
	}
	if(*b == 32){
		*a = 0;
		*b = 0;
	}
}

liste_point deplacementFantomeR(liste_point L, SDL_Rect *fant, int *a, int *b) {
	if (!est_vide(L)){
		if(*a == 0){
			if (prem(L).x == ((fant->x) / taille) + 1) { // droite
				*a = 1;
			}
			if (prem(L).x == ((fant->x) / taille) - 1) { // gauche
				*a = 2;
			}
			if (prem(L).y == ((fant->y) / taille) + 1) {	//bas
				*a = 3;
			}
			if (prem(L).y == ((fant->y) / taille) - 1) { // haut
				*a = 4;
			}
		}
		if(*a == 1){
			deplacement(fant,1,0);
			*b += 1;
		}
		if(*a == 2){
			deplacement(fant,-1,0);
			*b += 1;
		}
		if(*a == 3){
			deplacement(fant,0,1);
			*b += 1;
		}
		if(*a == 4){
			deplacement(fant,0,-1);
			*b += 1;
		}
		if(*b == 32){
			*a = 0;
			*b = 0;
			 L = reste(L);
		}
		/*if (prem(L).x == (fant->x / taille) + 1) { // droite
			//printf("DROITE\n");
			deplacement(fant,32,0);
			
			if (((fant->x)/31) == prem(L).x) {
				//m = fant.x/32;
				L = reste(L);
				return L;
			}
		}
		if (prem(L).x == (fant->x / taille) - 1) { // gauche
			//printf("GAUCHE\n");
			deplacement(fant,-32,0);
			if (((fant->x)/31) == prem(L).x) {
				L = reste(L);
				return L;
			}
			
		}
		if (prem(L).y == (fant->y / taille) + 1) {	//bas
			//printf("BAS\n");
			deplacement(fant,0,32);
			if (((fant->y)/31) == prem(L).y) {
				L = reste(L);
				return L;
			}
			
		}
		if (prem(L).y == (fant->y / taille) - 1) { // haut
			//printf("HAUT\n");
			deplacement(fant,0,-32);
			if (((fant->y)/31) == prem(L).y) {
				L = reste(L);
				return L;
			}
		}*/
	}
	return L;
}

liste_point pathfinding(int map[NY][NX],int dy, int dx, int fy, int fx)
{
	int i,j;

	liste_point LF;
	point pNode;
	LF = l_vide();

	int dist[NY][NX];
	int d;
	
	for (i=0; i<NY; i++)
		for (j=0; j<NX; j++)
        {
			dist[i][j] = 40000;
		}

	dist[dy][dx] = 0;
	
	for (d=0;d<NY*NX;d++)
	{
		for (i=0; i<NY; i++)
		{
			for (j=0; j<NX; j++)
			{
				if(dist[i][j] == d)
				{
				    if (i>0 && (map[i-1][j] != 1 && map[i-1][j] != 3) && dist[i-1][j]>d+1)
				    {
				        dist[i-1][j] = d+1;
				    }
				    if (j>0 && (map[i][j-1] != 1 && map[i][j-1] != 3)&& dist[i][j-1]>d+1)
				    {
				        dist[i][j-1] = d+1;
				    }
				    if (i<NY-1 && (map[i+1][j] != 1 && map[i+1][j] != 3) && dist[i+1][j]>d+1)
				    {
				        dist[i+1][j] = d+1;
				    }
				    if (j<NX -1 && (map[i][j+1] != 1 && map[i][j+1] != 3) && dist[i][j+1]>d+1)
				    {
				        dist[i][j+1] = d+1;
				    }
				}
            }
		}
	}	
	d = dist[fy][fx];

	if (d>=1000){
		return l_vide();
	}
	
    pNode = remplisPoint(fx,fy); // met l'arrivé dans la liste
    LF = cons(pNode,LF);
	
	for (;d>0;d--) {
		if (fy>0 && dist[fy-1][fx] == d-1){
			fy--;
			pNode = remplisPoint(fx,fy);
			LF = cons(pNode,LF);
			continue;
        	}
		if (fx>0 && dist[fy][fx-1] == d-1){
			fx--;
			pNode = remplisPoint(fx,fy);
			LF = cons(pNode,LF);
			continue; }
		if (fy<NY+1 && dist[fy+1][fx] == d-1) {
			fy++;
			pNode = remplisPoint(fx,fy);
			LF = cons(pNode,LF);
			continue; }
		if (fx<NX+1  && dist[fy][fx+1] == d-1) {
			fx++;
			pNode = remplisPoint(fx,fy);
			LF = cons(pNode,LF);
			continue; }
	}
	
	
	LF = reste(LF); // supprime le premier element, car on deja dessus

	return LF;
}
void SetMovementBooleans(enum direction dir)
{
	switch(dir){
	case left:
		if(moveUp){
                      	 moveUp = 0;
		}
		if(moveDown){
              		moveDown = 0;
		}
		if(moveRight){
                 	moveRight = 0;
		}
		moveLeft=1;		
	break;
	case right:
		if(moveUp){
			moveUp = 0;
		}
		if(moveDown){
			moveDown = 0;
		}
		if(moveLeft){
			moveLeft = 0;
		}
		moveRight = 1;
	break;
	case up:
		if(moveLeft){
                   	moveLeft = 0;
		}
		if(moveDown){
                      	moveDown = 0;
		}
		if(moveRight){
                     	moveRight = 0;
		}
		moveUp=1;
	break;
	case down:
		 if(moveUp){
                      	moveUp = 0;
		 }
		 if(moveLeft){
                    	moveLeft = 0;
		 }
		 if(moveRight){
                       	moveRight = 0;
		 }
		 moveDown=1;
	break;
	default:
	break;
	}
}

int bonne_case(int m, int n, int pos_Wall[NY][NX])
{
	return (pos_Wall[m][n] == 0 || pos_Wall[m][n] == 4 || pos_Wall[m][n] == 6);
}

void HandleAnimations();

void HandleEvent(SDL_Event event, int pos_Wall[NY][NX])
{
	int nL,mL,n1L,m1L,nR,mR,n1R,m1R,nU,mU,n1U,m1U,nD,mD,n1D,m1D;


	nU = ((rcSprite.x)/32);
	mU = ((rcSprite.y-1)/32); 
	n1U = ((rcSprite.x+31)/32);
	m1U = ((rcSprite.y-1)/32);

	nD = ((rcSprite.x)/32);
	mD = ((rcSprite.y+31+1)/32);
	n1D = ((rcSprite.x+31)/32);
	m1D = ((rcSprite.y+31+1)/32);

	nR = ((rcSprite.x+31+1)/32);
	mR = ((rcSprite.y)/32);
	n1R = ((rcSprite.x+31+1)/32);
	m1R = ((rcSprite.y+31)/32);

	nL = ((rcSprite.x-1)/32);
	mL = ((rcSprite.y)/32);
	n1L = ((rcSprite.x-1)/32);
	m1L = ((rcSprite.y+31)/32);
	switch (event.type) {
		default:
			break;
		/* close button clicked */
		case SDL_QUIT:
			gameover = 1;
			break;

		/* handle the keyboard */
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				default:
					break;
				case SDLK_ESCAPE:
				case SDLK_q:
					gameover = 1;
					break;
				case SDLK_LEFT:
					if(bonne_case(mL,nL,pos_Wall) && bonne_case(m1L,n1L,pos_Wall)){
						SetMovementBooleans(left);
					} else {
						wantLeft = 1;					
					}
					break;
				case SDLK_RIGHT:
					
					if(bonne_case(mR,nR,pos_Wall) && bonne_case(m1R,n1R,pos_Wall)){
						SetMovementBooleans(right);
					} else {
						wantRight = 1;
					}
				    	break;
				case SDLK_UP:
					
					if(bonne_case(m1U,n1U,pos_Wall) && bonne_case(mU,nU,pos_Wall)){
						SetMovementBooleans(up);
					} else {
						wantUp = 1;
					}
					break;
				case SDLK_DOWN:
					
					if(bonne_case(mD,nD,pos_Wall) && bonne_case(m1D,n1D,pos_Wall)){
						SetMovementBooleans(down);
					} else {
						wantDown = 1;
					}			
					break;
				//mus				
				/*case SDLK_m: 

					if(Mix_PausedMusic() == 1)
					{

						Mix_ResumeMusic(); 
						printf("p1");
					}
					else
					{
						Mix_PauseMusic(); 

						printf("p2"); 
					}

					break;*/
				
				}
			break;
		}
}

void HandleMovements(int pos_Wall[NY][NX])
{

    if(moveUp){
	int n = ((rcSprite.x)/32);
	int m = ((rcSprite.y-1)/32); 

	int n2 = ((rcSprite.x+31)/32);
	int m2 = ((rcSprite.y-1)/32);

	if ((pos_Wall[m][n] == 0 || pos_Wall[m][n] == 4 || pos_Wall[m][n] == 6) && (pos_Wall[m2][n2] == 0 || pos_Wall[m2][n2] == 4 || pos_Wall[m2][n2] == 6)){
        currentTime = SDL_GetTicks();
         if(currentTime - previousTime > TIME_BTW_MOVEMENTS){
            rcSprite.y -= 1;
            previousTime = currentTime;
         }
         HandleAnimations();
		}
	}
    if(moveDown){
	int n = ((rcSprite.x)/32);
	int m = ((rcSprite.y+31+1)/32);

	int n2 = ((rcSprite.x+31)/32);
	int m2 = ((rcSprite.y+31+1)/32);
	if ((pos_Wall[m][n] == 0 || pos_Wall[m][n] == 4|| pos_Wall[m2][n2] == 6) && (pos_Wall[m2][n2] == 0 || pos_Wall[m2][n2] == 4 || pos_Wall[m2][n2] == 6)){
        currentTime = SDL_GetTicks();
        if(currentTime - previousTime > TIME_BTW_MOVEMENTS){
            rcSprite.y += 1;
            previousTime = currentTime;
         }
        HandleAnimations();
    }
	}
    if(moveLeft){
	int n = ((rcSprite.x-1)/32);
	int m = ((rcSprite.y)/32);

	int n2 = ((rcSprite.x-1)/32);
	int m2 = ((rcSprite.y+31)/32);
	if ((pos_Wall[m][n] == 0 || pos_Wall[m][n] == 4 || pos_Wall[m2][n2] == 6) && (pos_Wall[m2][n2] == 0 || pos_Wall[m2][n2] == 4 || pos_Wall[m2][n2] == 6)){
        currentTime = SDL_GetTicks();
        if(currentTime - previousTime > TIME_BTW_MOVEMENTS){
            rcSprite.x -= 1;
            previousTime = currentTime;
         }
         HandleAnimations();
    }
	}
    if(moveRight){
	int n = ((rcSprite.x+31+1)/32);
	int m = ((rcSprite.y)/32);

	int n2 = ((rcSprite.x+31+1)/32);
	int m2 = ((rcSprite.y+31)/32);
	if ((pos_Wall[m][n] == 0 || pos_Wall[m][n] == 4 || pos_Wall[m2][n2] == 6) && (pos_Wall[m2][n2] == 0 || pos_Wall[m2][n2] == 4 || pos_Wall[m2][n2] == 6)){
        currentTime = SDL_GetTicks();
        if(currentTime - previousTime > TIME_BTW_MOVEMENTS){
            rcSprite.x += 1;
            previousTime = currentTime;
        }
        HandleAnimations();
    }
	}
}
void HandleAnimations()
{
    if(moveUp){
        rcSrc.y = 2 * SPRITE_HEIGHT;
        currentTimeAnim = SDL_GetTicks();
        if(currentTimeAnim - previousTimeAnim > TIME_BTW_ANIMATIONS){
            rcSrc.x = rcSrc.x + SPRITE_WIDTH;
            previousTimeAnim = currentTimeAnim;
        }

        if ( rcSrc.x >= 5 * SPRITE_WIDTH) {
            rcSrc.x = 0;
        }
    }
    if(moveDown){
        rcSrc.y = 3 * SPRITE_HEIGHT;
        currentTimeAnim = SDL_GetTicks();
        if(currentTimeAnim - previousTimeAnim > TIME_BTW_ANIMATIONS){
            rcSrc.x = rcSrc.x + SPRITE_WIDTH;
            previousTimeAnim = currentTimeAnim;
        }
        if ( rcSrc.x >= 5 * SPRITE_WIDTH) {
            rcSrc.x = 0;
        }
    }
	
    if(moveLeft){
        rcSrc.y = SPRITE_HEIGHT;
        currentTimeAnim = SDL_GetTicks();
        if(currentTimeAnim - previousTimeAnim > TIME_BTW_ANIMATIONS){
            rcSrc.x = rcSrc.x - SPRITE_WIDTH;
            previousTimeAnim = currentTimeAnim;
        }
        if (rcSrc.x < 0) {
            rcSrc.x = 5 * SPRITE_WIDTH;
        }
    }
  
    if(moveRight){
        rcSrc.y = 0;
        currentTimeAnim = SDL_GetTicks();
        if(currentTimeAnim - previousTimeAnim > TIME_BTW_ANIMATIONS){
            rcSrc.x = rcSrc.x + SPRITE_WIDTH;
            previousTimeAnim = currentTimeAnim;
        }
        if(rcSrc.x > 5 * SPRITE_WIDTH){
            rcSrc.x = 0;
        }
    }
}

int main()
{
	SDL_Surface *screen, *map, *temp, *wall, *wall2, *bloc, *coeur , *sprite, *g1, *g2, *g3, *candy, *candy2, *menu, *gover, *victory;
	SDL_Rect rcmap;
	int colorkey;
	int i,j;

	int deplaSG1,deplaCG1,deplaSG2,deplaCG2,deplaSG3,deplaCG3;
	liste_point liste_coord_rouge,liste_coord_bleu,liste_coord_blanc ;

	srand(time(NULL));

	/* initialize SDL */
	srand(time(NULL));
	SDL_Init(SDL_INIT_VIDEO);

	 /* initialize SDL */

	/*SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);  
	Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
	music = Mix_LoadMUS("sons/point.mp3");
	start = Mix_LoadMUS("sons/start.mp3");
	scream = Mix_LoadMUS("sons/scream.wav");
	pilule = Mix_LoadMUS("sons/pilule.wav");
	die = Mix_LoadMUS("sons/die.mp3");
	siren = Mix_LoadMUS("sons/siren.mp3");

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1){
		printf("%s", Mix_GetError());
	}*/

	

	//SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO /*SDL_INIT_TIMER)*/);


	/*test if the beginning of SDL is correct*/
	/*
	if (SDL_Init(SDL_INIT_VIDEO) == -1) // Démarrage de la SDL. Si erreur :
	{
	 fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // Écriture de l'erreur
	 exit(EXIT_FAILURE); // On quitte le programme
	}*/


	/* Icon */
	SDL_WM_SetIcon(SDL_LoadBMP("images/pac_ico.bmp"), NULL);
	
	/* set the title bar */
	SDL_WM_SetCaption("Pacman", "Pacman");

	/* create window */
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

	/* set keyboard repeat */
	SDL_EnableKeyRepeat(30,30);

 	/*load menu */ 
 	temp=SDL_LoadBMP("images/1.bmp");
	menu= SDL_DisplayFormat(temp);
 	SDL_FreeSurface(temp);

	/* load sprite */
	temp   = SDL_LoadBMP("images/pacman.bmp");
	sprite = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	/* setup sprite colorkey and turn on RLE*/
	colorkey = SDL_MapRGB(screen->format, 0,0,0);
	SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

	/* load Wall */
	temp   = SDL_LoadBMP("images/wall.bmp");
	wall = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* load Wall2 */
	temp   = SDL_LoadBMP("images/wall2.bmp");
	wall2 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* load bloc */
	temp   = SDL_LoadBMP("images/bloc.bmp");
	bloc = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	
	/* load coeur */
	temp   = SDL_LoadBMP("images/vie.bmp");
	coeur = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* setup coeur colorkey and turn on RLE*/
	colorkey = SDL_MapRGB(screen->format, 0,0,0);
	SDL_SetColorKey(coeur, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

	/* load candy */
	temp   = SDL_LoadBMP("images/bonbon.bmp");
	candy = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	
	/* load candy special */
	temp   = SDL_LoadBMP("images/bonbon2.bmp");
	candy2 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* load G1 */
	temp   = SDL_LoadBMP("images/g1_f.bmp");
	g1 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);


	/* setup g1 colorkey and turn on RLE*/
	colorkey = SDL_MapRGB(screen->format, 0,0,0);
	SDL_SetColorKey(g1, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);


	/* load g2 */
	temp   = SDL_LoadBMP("images/g2_f.bmp");
	g2 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* setup g2 colorkey and turn on RLE*/
	colorkey = SDL_MapRGB(screen->format, 0,0,0);
	SDL_SetColorKey(g2, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);


	/* load g3 */
	temp   = SDL_LoadBMP("images/g3_f.bmp");
	g3 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* setup g3 colorkey and turn on RLE*/
	colorkey = SDL_MapRGB(screen->format, 0,0,0);
	SDL_SetColorKey(g3, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

	/* load map */
	temp  = SDL_LoadBMP("images/mapb.bmp");
	map = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* load victoiry */	
	temp   = SDL_LoadBMP("images/victoire.bmp");
	victory = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

        /*initialise le temps*/
        currentTime = 0;
        previousTime = 0;
        currentTimeAnim = 0;
        previousTimeAnim = 0;
	fant_time = 0;

	/* set sprite position */
	rcSprite.x = SCREEN_WIDTH/2;
	rcSprite.y = SCREEN_HEIGHT/2 +32;

	/* set Ghost position */

	
	rcG1.x = SCREEN_WIDTH/2 -32;
	rcG1.y = SCREEN_HEIGHT/2-32;

	
	rcG2.x = SCREEN_WIDTH/2;
	rcG2.y = SCREEN_HEIGHT/2-32;


	rcG3.x = SCREEN_WIDTH/2 +32;
	rcG3.y = SCREEN_HEIGHT/2-32;
	

	/* set animation frame */
	rcSrc.x = 0;
	rcSrc.y = 0;
	rcSrc.w = SPRITE_WIDTH;
	rcSrc.h = SPRITE_HEIGHT;

	/* set animation frame */
	rcSG1.x = 0;
	rcSG1.y = 0;
	rcSG1.w = G1_WIDTH;
	rcSG1.h = G1_HEIGHT;

	rcmap.x = 0;
	rcmap.y = 0;

	gameover = 0;
	
	liste_coord_rouge = l_vide();
	liste_coord_bleu = l_vide();
	liste_coord_blanc = l_vide();

	int pos_Wall[NY][NX]= {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,6,0,4,0,4,0,4,0,4,0,4,6,0,4,0,4,0,4,0,4,0,6,1},
	{1,0,1,1,0,1,1,1,1,1,1,4,1,4,1,1,1,1,1,0,1,1,0,1},
	{1,4,1,1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,4,1},
	{1,0,0,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,0,1},
	{1,4,1,1,0,1,1,1,1,1,1,4,1,4,1,1,1,1,1,0,1,1,4,1},
	{1,0,1,1,4,0,4,0,4,0,0,0,1,0,1,4,0,4,0,4,1,1,0,1},
	{1,4,0,1,0,1,3,1,1,4,0,4,0,4,0,4,1,3,1,0,1,0,4,1},
	{1,0,0,1,4,1,4,0,1,0,1,1,5,1,1,0,1,4,0,0,1,0,1,1},
	{0,4,0,4,0,4,0,4,0,4,1,7,8,9,1,4,0,4,0,4,0,4,0,4},
	{1,1,0,1,4,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,0,0,1},
	{1,4,0,1,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,1,4,0,1},
	{1,0,1,1,4,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,4,1},
	{1,4,1,1,0,1,4,0,4,0,4,0,4,0,4,0,4,0,1,4,1,1,0,1},
	{1,0,1,1,4,1,0,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,4,1},
	{1,4,0,4,0,4,0,4,0,4,0,4,1,4,0,4,0,4,0,4,0,4,0,1},
	{1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,4,1},
	{1,4,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,0,1},
	{1,0,6,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,6,1},
	{1,1,10,10,10,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

	int cpt=0;
	int pac_y;
	int pac_x;

	int fant_rouge_x;
	int fant_rouge_y;

	int fant_bleu_x;
	int fant_bleu_y;


	int fant_blanc_x;
	int fant_blanc_y;

	int eat;
	eat = 0;

	int home; /*_rouge, home_bleu, home_blanc;
	home_rouge =0;
	home_bleu =0;
	home_blanc =0;*/

	home =0;
	int life = 4;

	int compte_life = 0;

	deplaSG1 = 0;
	deplaCG1 = 0;
	deplaSG2 = 0;
	deplaCG2 = 0;
	deplaSG3 = 0;
	deplaCG3 = 0;

	/*Mix_PlayMusic(start, 1);
	SDL_BlitSurface(menu,NULL,screen,NULL);
  	SDL_Flip(screen);
  	SDL_Delay(5000);
	*/
	int g_rouge_back_home, g_bleu_back_home, g_blanc_back_home;

	g_rouge_back_home = 0;
	g_bleu_back_home = 0;
	g_blanc_back_home = 0;

	//deplacementBleu(&rcG2);


	 /*message pump */
	while (!gameover)	// ------------------- BOUCLE ICI -------------------- //
{
	fant_rouge_x = (rcG1.x+16)/32;
	fant_rouge_y = (rcG1.y+16)/32;

	fant_bleu_x = (rcG2.x+16)/32;
	fant_bleu_y = (rcG2.y+16)/32;

	fant_blanc_x = (rcG3.x+16)/32;
	fant_blanc_y = (rcG3.y+16)/32;
	
	pac_x = ((rcSprite.x+16)/32);
	pac_y = ((rcSprite.y+16)/32);

		SDL_Event event;
		/* look for an event */
		if (SDL_PollEvent(&event)) {
			HandleEvent(event,pos_Wall);
		}

		time_game = SDL_GetTicks();

		if((time_game == 5 || time_game % 5 == 0) && (eat == 0) && (home == 0)){

			/*liste_coord_blanc = pathfinding(pos_Wall, fant_blanc_y, fant_blanc_x,  pac_y, pac_x );
			liste_coord_blanc = deplacementFantomeR(liste_coord_blanc , &rcG3, &deplaSG3,&deplaCG3);*/
			liste_coord_rouge = pathfinding(pos_Wall, fant_rouge_y, fant_rouge_x, pac_y, pac_x);
			liste_coord_rouge = deplacementFantomeR(liste_coord_rouge, &rcG1, &deplaSG1,&deplaCG1);	
				

 			deplacementFantomeBlanc(pos_Wall,&rcG3, &deplaSG3,&deplaCG3);


			deplacementBleu(pos_Wall,&rcG2,&deplaSG2,&deplaCG2);

			/*deplacement(&rcG2,0,32);
			deplacement(&rcG2,32,0);
			deplacement(&rcG2,0,-32);
			deplacement(&rcG2,-32,0);*/
		}
		if((time_game == 5 || time_game % 5 == 0) && (eat == 1)){
			liste_coord_rouge = l_vide();
			liste_coord_rouge = pathfinding(pos_Wall, fant_rouge_y, fant_rouge_x, 1, 1);
			liste_coord_rouge = deplacementFantomeR(liste_coord_rouge, &rcG1, &deplaSG1,&deplaCG1);		
		}

		if((time_game == 5 || time_game % 5 == 0) && (eat == 1)){
			liste_coord_blanc = l_vide();	
			liste_coord_blanc = pathfinding(pos_Wall, fant_blanc_y, fant_blanc_x, 1, 10);
			liste_coord_blanc = deplacementFantomeR(liste_coord_blanc, &rcG3, &deplaSG3,&deplaCG3);		
		}

		if((time_game == 5 || time_game % 5 == 0) && (g_rouge_back_home == 1)){
			liste_coord_rouge = l_vide();
			liste_coord_rouge = pathfinding(pos_Wall, fant_rouge_y, fant_rouge_x, 9, 11);
			liste_coord_rouge = deplacementFantomeR(liste_coord_rouge, &rcG1, &deplaSG1,&deplaCG1);		
		}

		if((time_game == 5 || time_game % 5 == 0) && (g_blanc_back_home == 1)){
			liste_coord_blanc = l_vide(); 
			liste_coord_blanc = pathfinding(pos_Wall, fant_blanc_y, fant_blanc_x, 9, 13);
			liste_coord_blanc = deplacementFantomeR(liste_coord_blanc, &rcG3, &deplaSG3,&deplaCG3);		
		}

       		 HandleMovements(pos_Wall);
		/* collide with edges of screen */
		if (rcSprite.x <= 0)
			rcSprite.x = SCREEN_WIDTH - SPRITE_WIDTH -2;
		if (rcSprite.x >= SCREEN_WIDTH - SPRITE_WIDTH -1)
			rcSprite.x = 1;

		/*if (rcSprite.y <= 0)
			rcSprite.y = 0;
		if (rcSprite.y >= SCREEN_HEIGHT - SPRITE_HEIGHT
			rcSprite.y = SCREEN_HEIGHT - SPRITE_HEIGHT;*/


		/* draw the map */
		SDL_BlitSurface(map,NULL,screen,&rcmap);


		
	for(i=0; i<NY ; i++){
		for(j=0;j<NX;j++){
			if ( pos_Wall[i][j] == 1 ){
				rcBloc.x = j * 32;
				rcBloc.y = i * 32;
				SDL_BlitSurface(bloc, NULL, screen, &rcBloc);
			}
			if ( pos_Wall[i][j] == 2){
				rcWall.x = j * 32;
				rcWall.y = i * 32;
				SDL_BlitSurface(wall, NULL, screen, &rcWall);
			}
			if ( pos_Wall[i][j] == 3){
				rcWall2.x = j * 32;
				rcWall2.y = i * 32;
				SDL_BlitSurface(wall2, NULL, screen, &rcWall2);
			}

			if ( pos_Wall[i][j] == 4 ){
				rcCandy.x = j * 32+8;
				rcCandy.y = i * 32+8;
				SDL_BlitSurface(candy, NULL, screen, &rcCandy);
			}
			if ( pos_Wall[i][j] == 6 ){
				rcCandy2.x = j * 32+8;
				rcCandy2.y = i * 32+8;
				SDL_BlitSurface(candy2, NULL, screen, &rcCandy2);
			}
			if ( pos_Wall[i][j] == 10 ){
				rcCoeur.x = j * 32 + 8;
				rcCoeur.y = i * 32 + 8;

				rcBloc.x = j * 32;
				rcBloc.y = i * 32;

				SDL_BlitSurface(bloc, NULL, screen, &rcBloc);
				SDL_BlitSurface(coeur, NULL, screen, &rcCoeur);
			}
			/* draw the GHOST 1 */
			/*if ( pos_Wall[i][j] == 7 ){
				rcG1.x = j * 32;
				rcG1.y = i * 32;
			SDL_BlitSurface(g1, NULL, screen, &rcG1);
			}*/
			/* draw the GHOST 2 */
			/*if ( pos_Wall[i][j] == 8 ){
				rcG2.x = j * 32;
				rcG2.y = i * 32;
				SDL_BlitSurface(g2, NULL, screen, &rcG2);
			}*/
			/* draw the GHOST 3 */
			/*if ( pos_Wall[i][j] == 9 ){
				rcG3.x = j * 32;
				rcG3.y = i * 32;
				SDL_BlitSurface(g3, NULL, screen, &rcG3);
			}*/			
		}
	}


	if ( pos_Wall[pac_y][pac_x] == 4){
		cpt ++;
		printf("cpt = %d\n",cpt);
		pos_Wall[pac_y][pac_x]=0;

		//musMix_PlayMusic(music, 1);
		//if (cpt == 5 ) {
		/* load screamer */
	       // temp = SDL_LoadBMP("images/scream.bmp");
		//  gover = SDL_DisplayFormat(temp);
		//  SDL_FreeSurface(temp);


		 // Mix_PlayMusic(scream, 1); 
		//SDL_BlitSurface(gover,NULL,screen,NULL);
	     	// SDL_Flip(screen);
	      	//SDL_Delay(2500);
		//}				
	}

	if (pos_Wall[pac_y][pac_x] == 6) {
	
		eat = 1;
		time_game_eat = time_game;
		

		cpt += 10;
		printf("cpt = %d\n",cpt);
		//musMix_PlayMusic(siren, 2);
		printf("Siren OK\n");
		
		/* ------------ durée de 5 ms pour les 2 fantomes ------------*/
		temp = SDL_LoadBMP("images/g1_eat.bmp");
		g1 = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		
		liste_coord_rouge = l_vide();
		liste_coord_rouge = pathfinding(pos_Wall, fant_rouge_y, fant_rouge_x, 1, 1 );
		liste_coord_rouge = deplacementFantomeR(liste_coord_rouge, &rcG1, &deplaSG1,&deplaCG1);			

		colorkey = SDL_MapRGB(screen->format, 0,0,0);
		SDL_SetColorKey(g1, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

		temp = SDL_LoadBMP("images/g2_eat.bmp");
		g2 = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		colorkey = SDL_MapRGB(screen->format, 0,0,0);
		SDL_SetColorKey(g2, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

		temp = SDL_LoadBMP("images/g3_eat.bmp");
		g3 = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		colorkey = SDL_MapRGB(screen->format, 0,0,0);
		SDL_SetColorKey(g3, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
		
		pos_Wall[pac_y][pac_x] = 0;
	}


	if (time_game - time_game_eat >= 6000 && time_game_eat != 0 && g_rouge_back_home == 0){	// OK

		eat = 0;

		temp   = SDL_LoadBMP("images/g1_f.bmp");
		g1 = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		colorkey = SDL_MapRGB(screen->format, 0,0,0);
		SDL_SetColorKey(g1, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
		
	}

	if (time_game - time_game_eat >= 6000 && time_game_eat != 0 && g_bleu_back_home == 0){	// OK

		eat = 0;

		temp   = SDL_LoadBMP("images/g2_f.bmp");
		g2 = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		colorkey = SDL_MapRGB(screen->format, 0,0,0);
		SDL_SetColorKey(g2, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
		
	}

	if (time_game - time_game_eat >= 6000 && time_game_eat != 0 && g_blanc_back_home == 0){	// OK

		eat = 0;
		temp   = SDL_LoadBMP("images/g3_f.bmp");
		g3 = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		colorkey = SDL_MapRGB(screen->format, 0,0,0);
		SDL_SetColorKey(g3, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
	}

	if (cpt == 146) {
		printf("VICTORY ");
		SDL_BlitSurface(victory,NULL,screen,NULL);
  		SDL_Flip(screen);
  		SDL_Delay(5000);
		cpt ++;
	}
	
	/********************** fantome rouge ************************/
	if ((fant_rouge_y == pac_y && fant_rouge_x == pac_x) && (eat == 1 ) && (g_rouge_back_home == 0)){


		g_rouge_back_home = 1;
	
		/* load eye */
		temp = SDL_LoadBMP("images/eye_rouge.bmp");
		g1 = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		colorkey = SDL_MapRGB(screen->format, 0,0,0);
		SDL_SetColorKey(g1, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
	}

	if ((fant_bleu_y == pac_y && fant_bleu_x == pac_x) && (eat == 1 ) && (g_bleu_back_home == 0)){

		g_bleu_back_home = 1;
	
		/* load eye */
		temp = SDL_LoadBMP("images/eye_bleu.bmp");
		g2 = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		colorkey = SDL_MapRGB(screen->format, 0,0,0);
		SDL_SetColorKey(g2, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
	}

	if ((fant_blanc_y == pac_y && fant_blanc_x == pac_x) && (eat == 1 ) && (g_blanc_back_home == 0)){

		g_blanc_back_home = 1;
	
		/* load eye */
		temp = SDL_LoadBMP("images/eye_blanc.bmp");
		g3 = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		colorkey = SDL_MapRGB(screen->format, 0,0,0);
		SDL_SetColorKey(g3, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
	}

/*
	if ((fant_rouge_y != pac_y && fant_rouge_x != pac_x) && (eat == 1 ) && (home == 0 )){

		liste_coord_rouge = pathfinding(pos_Wall, fant_rouge_y, fant_rouge_x, 1, 1);
		liste_coord_rouge = deplacementFantomeR(liste_coord_rouge, &rcG1, &deplaSG1,&deplaCG1);
	}

	if ((fant_bleu_y != pac_y && fant_bleu_x != pac_x) && (eat == 1 ) && (home == 0 )){

		liste_coord_bleu = pathfinding(pos_Wall, fant_bleu_y, fant_bleu_x, 1, 1);
		liste_coord_bleu = deplacementFantomeR(liste_coord_bleu, &rcG2, &deplaSG2,&deplaCG2);
	}

	if ((fant_blanc_y != pac_y && fant_blanc_x != pac_x) && (eat == 1 ) && (home == 0 )){

		liste_coord_blanc = pathfinding(pos_Wall, fant_blanc_y, fant_blanc_x, 1, 1);
		liste_coord_blanc = deplacementFantomeR(liste_coord_blanc, &rcG3, &deplaSG3,&deplaCG3);
	}
	



	if ((home == 1) && ((fant_rouge_y == pac_y) && (fant_rouge_x == pac_x))){
		liste_coord_rouge = pathfinding(pos_Wall, fant_rouge_y, fant_rouge_x, 9, 11);
		liste_coord_rouge = deplacementFantomeR(liste_coord_rouge, &rcG1, &deplaSG1,&deplaCG1);
	}

	if ((home == 2) && ((fant_bleu_y == pac_y) && (fant_bleu_x == pac_x))){
		liste_coord_bleu = pathfinding(pos_Wall, fant_bleu_y, fant_bleu_x, 9, 11);
		liste_coord_bleu = deplacementFantomeR(liste_coord_bleu, &rcG2, &deplaSG2,&deplaCG2);
	}

	if ((home == 3) && ((fant_blanc_y == pac_y) && (fant_blanc_x == pac_x))){
		liste_coord_blanc = pathfinding(pos_Wall, fant_blanc_y, fant_blanc_x, 9, 11);
		liste_coord_blanc = deplacementFantomeR(liste_coord_blanc, &rcG3, &deplaSG3,&deplaCG3);
	}


	if (home == 1){
		liste_coord_rouge = pathfinding(pos_Wall, fant_rouge_y, fant_rouge_x, 9, 11);
		liste_coord_rouge = deplacementFantomeR(liste_coord_rouge, &rcG1, &deplaSG1,&deplaCG1);
	}
	if (home == 2){
		liste_coord_bleu = pathfinding(pos_Wall, fant_bleu_y, fant_bleu_x, 9, 12);
		liste_coord_bleu = deplacementFantomeR(liste_coord_bleu, &rcG2, &deplaSG2,&deplaCG2);
	}
	if (home == 3){
		liste_coord_blanc = pathfinding(pos_Wall, fant_blanc_y, fant_blanc_x, 9, 13);
		liste_coord_blanc = deplacementFantomeR(liste_coord_blanc, &rcG3, &deplaSG3,&deplaCG3);
	}
*/
	if (fant_rouge_y == 9 && fant_rouge_x == 11 && g_rouge_back_home ==1 ){

		temp   = SDL_LoadBMP("images/g1_f.bmp");
		g1 = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		colorkey = SDL_MapRGB(screen->format, 0,0,0);
		SDL_SetColorKey(g1, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

		g_rouge_back_home =0;
	}
	if (fant_bleu_y == 9 && fant_bleu_x == 12 && g_bleu_back_home ==1 ){

		temp   = SDL_LoadBMP("images/g2_f.bmp");
		g2 = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		colorkey = SDL_MapRGB(screen->format, 0,0,0);
		SDL_SetColorKey(g2, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

		g_bleu_back_home =0;
	}
	if (fant_blanc_y == 9 && fant_blanc_x == 13 && g_blanc_back_home ==1 ){

		temp   = SDL_LoadBMP("images/g3_f.bmp");
		g3 = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		colorkey = SDL_MapRGB(screen->format, 0,0,0);
		SDL_SetColorKey(g3, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

		g_blanc_back_home =0;
	}

/**********/

/*	if (fant_bleu_y == 9 && fant_bleu_x == 12){
		home = 0;

		temp   = SDL_LoadBMP("images/g2_f.bmp");
		g2 = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		colorkey = SDL_MapRGB(screen->format, 0,0,0);
		SDL_SetColorKey(g2, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
	}

	if (fant_blanc_y == 9 && fant_blanc_x == 13) {
		home = 0;

		temp   = SDL_LoadBMP("images/g3_f.bmp");
		g3 = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		colorkey = SDL_MapRGB(screen->format, 0,0,0);
		SDL_SetColorKey(g3, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
	}

*/
	

	if( (((fant_rouge_y == pac_y) && (fant_rouge_x == pac_x)) && (eat == 0))|| (((fant_bleu_y == pac_y) && (fant_bleu_x == pac_x)) && (eat == 0)) || (((fant_blanc_y == pac_y) && (fant_blanc_x == pac_x)) && (eat == 0))){
		printf("GAMEOVER");
		//gameover = 1;
		//aficher game over;
		//musMix_PlayMusic(die, 1);

		life --;
		compte_life = 1;

	  	SDL_Delay(1500);

		moveUp = 0;
		moveDown = 0;
		moveLeft = 0;
		moveRight = 0;

		//liste_coord = l_vide();


		rcSprite.x = SCREEN_WIDTH/2;
		rcSprite.y = SCREEN_HEIGHT/2 +32;

		/*rcG1.x = SCREEN_WIDTH/2 -32;
		rcG1.y = SCREEN_HEIGHT/2-32;	


		rcG2.x = SCREEN_WIDTH/2;
		rcG2.y = SCREEN_HEIGHT/2 -31;

		rcG3.x = SCREEN_WIDTH/2+32;
		rcG3.y = SCREEN_HEIGHT/2 -32;	*/
	}

	if ((life == 3 )&& (compte_life == 1)){
		pos_Wall[19][4] = 1;
		printf("3 vies\n");
		compte_life = 0;
	}

	if ((life == 2 )&&( compte_life == 1)){
		pos_Wall[19][3] = 1;
		printf("2 vies\n");
		compte_life = 0;

	}

	if ((life == 1) && (compte_life == 1)){
		pos_Wall[19][2] = 1;
		printf("1 vies\n");
		compte_life = 0;
	}

	if (life == 0){

		temp   = SDL_LoadBMP("images/gameover.bmp");
		gover = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);

		SDL_BlitSurface(gover,NULL,screen,NULL);
  		SDL_Flip(screen);
  		SDL_Delay(3000);

		liste_coord_rouge = l_vide();
		liste_coord_bleu = l_vide();
		liste_coord_blanc = l_vide();

		temp = SDL_LoadBMP("images/scream.bmp");
		gover = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		//musMix_PlayMusic(scream, 1); 
		SDL_BlitSurface(gover,NULL,screen,NULL);
		SDL_Flip(screen);
		SDL_Delay(2500);

		gameover = 1;
	}



		/* draw the sprite */		
		SDL_BlitSurface(g2, NULL, screen, &rcG2);
		SDL_BlitSurface(g3, NULL, screen, &rcG3);
		SDL_BlitSurface(sprite, &rcSrc, screen, &rcSprite);
		SDL_BlitSurface(g1, NULL, screen, &rcG1);
		
		/* update the screen */
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	
}
	/* On libère le matériel */

	//Mix_CloseAudio();

	/* clean up */
	SDL_FreeSurface(sprite);
	SDL_FreeSurface(wall);
	SDL_FreeSurface(wall2);
	SDL_FreeSurface(bloc);
	SDL_FreeSurface(coeur);
	SDL_FreeSurface(candy);
	SDL_FreeSurface(g1);
	SDL_FreeSurface(g2);
	SDL_FreeSurface(g3);
	SDL_FreeSurface(map);
	SDL_FreeSurface(menu);
	//SDL_FreeSurface(gover);
	SDL_FreeSurface(victory);
	//NO SDL_FreeSurface(scream);

   	//mus
	/*Mix_FreeMusic(music);
	Mix_FreeMusic(start);
	Mix_FreeMusic(scream);
	Mix_FreeMusic(pilule);
	Mix_FreeMusic(die);
	Mix_FreeMusic(siren);*/
	SDL_Quit();
	return 0;
}


	

