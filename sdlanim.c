#include "SDL.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//#include <SDL/SDL_mixer.h>

#define SCREEN_WIDTH  768
#define SCREEN_HEIGHT 640
#define SPRITE_WIDTH 32
#define SPRITE_HEIGHT 32
#define SPRITE_SIZE 32
#define NY 20
#define NX 24
#define G1_WIDTH 32
#define G1_HEIGHT 34

#define TIME_BTW_ANIMATIONS 40
#define TIME_BTW_MOVEMENTS 5

int gameover;// previousTime, currentTime entier qui stocke le temps
int moveRight, moveLeft, moveUp, moveDown; //gere le déplacement du pacman
int move;
int currentTime, previousTime; //gerer le temps entre les deplacements
int currentTimeAnim, previousTimeAnim; //gerer le temps entre les animations
float dirX, dirY,x,y;

/* source and destination rectangles */
int SDL_FillRect(SDL_Surface *dst, SDL_Rect *dstrect, Uint32 color);
SDL_Rect rcSrc,rcWall,rcWall2, rcBloc, rcSprite,rcG1, rcSG1, rcG2,rcG3,rcSG2, rcSG3,rcCandy;//, rcCandy2, rcCandy3, rcCandy4,


		//rcSG1 -> ghost param image

int a;
int Convertir(float nb)
{
	nb += 0.5;
	int a = (int) nb;
	return a;
}
SDL_Surface *map;

unsigned char green(Uint32 color) {
	return (color & (255*256))/256;
}

unsigned char blue(Uint32 color) {
	return (color & 255);
}

unsigned char red(Uint32 color) {
	return (color & (255*256*256))/(256*256);
}



Uint32 getpixel(SDL_Surface *map, int x, int y) {
    if (x<0 || y<0 || x>=map->w || y>=map->h) return 0;
    Uint8 *p = (Uint8 *)map->pixels + y*map->pitch + x*map->format->BytesPerPixel;
    return p[0] | p[1] << 8 | p[2] << 16; // TODO if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
}

void putpixel(int x, int y, Uint32 pixel) {
    int bpp_ = map->format->BytesPerPixel;
    if (x<0 || y<0 || x>=map->w || y>=map->h) return;
    Uint8 *p = (Uint8 *)map->pixels + y*map->pitch + x*bpp_;
    int i;
    for (i=0; i<bpp_; i++) {
        p[i] = ((Uint8*)&pixel)[i]; // TODO if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
    }
}

/*
void chercherchemin(int N, Matrice mat, int ldep,int cdep,int larriv,int carriv, bool trouve, Liste chem)
{
	trouve = false;
	mat[ldep][cdep]= false;
	if( ldep == larriv && cdep==c){
		trouve = true;
		chem = cons((ldep,cdep),l_vide());
	}
	if(cdep != N-1 ) {
		if ( mat[ldep][cdep+1]){
			chercherchemin(N,mat,ldep,cdep+1,larriv,carriv,trouve,chem);
		}
	}
	if(ldep != N-1 && !(trouve) ) {
		if (mat[ldep+1][cdep]) {
			chercherchemin(N,mat,ldep+1,cdep,larriv,carriv,trouve,chem);
		}
	}
	if(ldep != N-1 && !(trouve) ) {	//gauche
		if (mat[ldep+1][cdep]) {
			chercherchemin(N,mat,ldep+1,cdep,larriv,carriv,trouve,chem);
		}
	}
	if(ldep != N-1 && !(trouve) ) {	//dessus
		if (mat[ldep+1][cdep]) {
			chercherchemin(N,mat,ldep+1,cdep,larriv,carriv,trouve,chem);
		}
	}

	if(trouve){
		chemin=cons((ldep,cdep),chem);
	}


}

*/

void HandleAnimations();



void HandleEvent(SDL_Surface *map, SDL_Event event)
{
	switch (event.type) {
		/* close button clicked */
		case SDL_QUIT:
			gameover = 1;
			break;

		/* handle the keyboard */
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
				case SDLK_q:
					gameover = 1;
					break;
				case SDLK_LEFT:
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
					move = 1;
					break;
				case SDLK_RIGHT:
				    if(moveUp){
                     			   moveUp = 0;
				    }
				    if(moveDown){
                     			   moveDown = 0;
				    }
				    if(moveLeft){
                     			   moveLeft = 0;
				    }
					moveRight=1;
					move = 1;
					break;
				case SDLK_UP:
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
					move = 1;
					break;
				case SDLK_DOWN:
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
					move = 1;

					break;

				case SDLK_w:	// LEFT
					move=1;
					rcSG1.y = 0;
					rcSG1.x = rcSG1.x - 32;
					if (rcSG1.x <= 0) {
						rcSG1.x = 32;
					}
					if (!getpixel(map, rcG1.x-1, rcG1.y)) {
					 	rcG1.x -= 5;
					}


					break;
				case SDLK_c:	//RIGHT
					move=1;
					rcSG1.y = G1_HEIGHT;
					rcSG1.x =  G1_WIDTH;
					if(rcSG1.x >= G1_WIDTH){
						rcSG1.x = 0;
					}
					 rcG1.x += 5;
					break;
				case SDLK_s:	//UP
					move=1;
					rcSG1.y = 3 * G1_HEIGHT;
					rcSG1.x =  G1_WIDTH;
					if ( rcSG1.x >= G1_WIDTH) {
						rcSG1.x = 0;
					}
				 	rcG1.y -= 5;
					break;
				case SDLK_x:	//DOWN
					move=1;
					rcSG1.y = 4 * G1_HEIGHT;
					rcSG1.x = G1_WIDTH;
					if ( rcSG1.x >= G1_WIDTH) {
						rcSG1.x = 0;
					}
					 rcG1.y += 5;

					break;
				
			}
			break;



	}
}


void HandleMovements()
{
    if(moveUp){
        currentTime = SDL_GetTicks();
         if(currentTime - previousTime > TIME_BTW_MOVEMENTS){
            rcSprite.y -= 1;
	   if ( rcSprite.y ==  0 )
	      {
        
		moveUp = 0;
		moveDown = 1;
	      }

	    	rcSG2.y = 4 * G1_HEIGHT;
			rcSG2.x = G1_WIDTH;
			if ( rcSG2.x >= G1_WIDTH) {
			rcSG2.x = 0;
			}	
			rcG1.x -= 5;
			rcG2.y -= 5;
			rcG3.x += 5;

            previousTime = currentTime;
         }
         HandleAnimations();
    }

    if(moveDown){
       
	 
	 currentTime = SDL_GetTicks();
	 if(currentTime - previousTime > TIME_BTW_MOVEMENTS){
	   rcSprite.y += 1;
	    
	   if ( rcSprite.y ==  600 )
	      {
        
		moveDown = 0;
		moveUp = 1;
	      }

	   
	    	rcSG3.y = 4 * G1_HEIGHT;
		rcSG3.x = G1_WIDTH;
		if ( rcSG3.x >= G1_WIDTH) {
		  rcSG3.x = 0;
		}	
		rcG1.x += 5;
		rcG2.y += 5;
		rcG3.y -= 5;
            previousTime = currentTime;
         }
        HandleAnimations();
	
    }
    if(moveLeft){
        currentTime = SDL_GetTicks();
        if(currentTime - previousTime > TIME_BTW_MOVEMENTS){
            rcSprite.x -= 1;

	       if ( rcSprite.x ==  0 )
	      {
		moveLeft = 0;
		moveRight = 1;
	      }

	    rcG1.y += 5;
	    rcG2.x -= 5;
	    rcG3.x += 5;
            previousTime = currentTime;
         }
         HandleAnimations();
    }
    if(moveRight){
        currentTime = SDL_GetTicks();
        if(currentTime - previousTime > TIME_BTW_MOVEMENTS){
            rcSprite.x += 1;

	     if ( rcSprite.x ==  720 )
	      {


		moveRight = 0;
		moveLeft = 1;
	      }

	    rcSG1.y = 4 * G1_HEIGHT;
	    rcSG1.x = G1_WIDTH;
	    if ( rcSG1.x >= G1_WIDTH) {
	      rcSG1.x = 0;
	    }		
	    rcG1.y -= 5;
	    rcG2.x += 5;	
	    rcG3.x -= 5;
            previousTime = currentTime;
        }
        HandleAnimations();
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

int main(int argc, char* argv[])
{
	SDL_Surface *screen, *temp, *wall, *wall2, *bloc, *sprite, *g1, *g2, *g3, *candy;// *candy2, *candy3, *candy4,
	SDL_Rect rcmap;
	int colorkey;
	int i,j;

	/*rcBloc.h = 32;
	rcBloc.w = 32;
	rcBloc.x = 0;
	rcBloc.y = 0;
	*/

	/*initialize move of pacman */
	move=0;

	/* initialize SDL */
	SDL_Init(SDL_INIT_VIDEO);

	/* set the title bar */
	SDL_WM_SetCaption("Pacman", "Pacman");

	/* create window */
	screen = SDL_SetVideoMode(SCREEN_WIDTH, 640, 0, 0);

	/* set keyboard repeat */
	SDL_EnableKeyRepeat(30,30);

	/* load sprite */
	temp   = SDL_LoadBMP("pacman.bmp");
	sprite = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	/* setup sprite colorkey and turn on RLE*/
	colorkey = SDL_MapRGB(screen->format, 0,0,0);
	SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

	/* load Wall */
	temp   = SDL_LoadBMP("wall.bmp");
	wall = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* load Wall2 */
	temp   = SDL_LoadBMP("wall2.bmp");
	wall2 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* load bloc */
	temp   = SDL_LoadBMP("bloc.bmp");
	bloc = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);


	/* load candy */
	temp   = SDL_LoadBMP("bonbon.bmp");
	candy = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* setup candy colorkey and turn on RLE*/
	/*colorkey = SDL_MapRGB(screen->format, 0,0,0);
	SDL_SetColorKey(candy, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
*/
	/* load candy 2*/
	/*temp   = SDL_LoadBMP("bonbon.bmp");
	candy2 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
*/
	/* setup candy2 colorkey and turn on RLE*/
	/*colorkey = SDL_MapRGB(screen->format, 0,0,0);
	SDL_SetColorKey(candy2, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
	*/
	/* load candy 3 */
	/*temp   = SDL_LoadBMP("bonbon.bmp");
	candy3 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
*/
	/* setup candy3 colorkey and turn on RLE*/
	/*colorkey = SDL_MapRGB(screen->format, 0,0,0);
	SDL_SetColorKey(candy3, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
*/
	/* load candy 4 */
	/*temp   = SDL_LoadBMP("bonbon.bmp");
	candy4 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
*/
	/* setup candy4 colorkey and turn on RLE*/
	/*colorkey = SDL_MapRGB(screen->format, 0,0,0);c
	SDL_SetColorKey(candy4, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
*/
	/* load G1 */
	temp   = SDL_LoadBMP("g1.bmp");
	g1 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* setup g1 colorkey and turn on RLE*/
	colorkey = SDL_MapRGB(screen->format, 0,0,0);
	SDL_SetColorKey(g1, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);


	/* load g2 */
	temp   = SDL_LoadBMP("g2.bmp");
	g2 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* setup g2 colorkey and turn on RLE*/
	colorkey = SDL_MapRGB(screen->format, 0,0,0);
	SDL_SetColorKey(g2, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);


	/* load g3 */
	temp   = SDL_LoadBMP("g3.bmp");
	g3 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* setup g3 colorkey and turn on RLE*/
	colorkey = SDL_MapRGB(screen->format, 0,0,0);
	SDL_SetColorKey(g3, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

	/* load map */
	temp  = SDL_LoadBMP("mapb.bmp");
	map = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

    /*initialise le temps*/
    currentTime = 0;
    previousTime = 0;
    currentTimeAnim = 0;
    previousTimeAnim = 0;

	/* set sprite position */
	rcSprite.x = SCREEN_WIDTH/2;
	rcSprite.y = SCREEN_HEIGHT/2 +32;


	/* set candy 2 positon */
	/*rcCandy2.x = SCREEN_WIDTH/2 - 40;
	rcCandy2.y = SCREEN_HEIGHT/2 + 40;
*/
	/* set candy 3 positon */
	/*rcCandy3.x = SCREEN_WIDTH/2 + 40;
	rcCandy3.y = SCREEN_HEIGHT/2 + 40;
*/
	/* set candy 4 positon */
	/*rcCandy4.x = SCREEN_WIDTH/2 + 80;
	rcCandy4.y = SCREEN_HEIGHT/2 + 40;
	*/
	/* set G1 position */		//r
	rcG1.x = SCREEN_WIDTH/2 -32;
	rcG1.y = SCREEN_HEIGHT/2-32;

	/* set G2 positon */		//b
	rcG2.x = SCREEN_WIDTH/2;
	rcG2.y = SCREEN_HEIGHT/2 -31;

	/* set  positon */		//w
	rcG3.x = SCREEN_WIDTH/2+32;
	rcG3.y = SCREEN_HEIGHT/2 -32;


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


	int pos_Wall[NY][NX]= {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,4,0,4,0,4,0,4,0,4,0,4,1,0,4,0,4,0,4,0,4,0,4,1},
	{1,0,1,1,0,1,1,1,1,1,1,4,1,4,1,1,1,1,1,0,1,1,0,1},
	{1,4,1,1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,4,1},
	{1,0,0,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,0,1},
	{1,4,1,1,0,1,1,1,1,1,1,4,1,4,1,1,1,1,1,0,1,1,4,1},
	{1,0,1,1,4,0,4,0,4,0,1,0,1,0,1,4,0,4,0,4,1,1,0,1},
	{1,4,0,1,0,1,3,1,0,4,0,4,0,4,0,4,1,3,1,0,1,0,4,1},
	{1,1,0,1,4,1,4,1,4,0,1,1,0,1,1,0,1,4,1,0,1,0,1,1},
	{0,4,0,4,0,4,0,4,0,4,1,0,0,0,1,4,0,4,0,4,0,4,0,4},
	{1,1,0,1,4,1,1,1,0,0,1,1,1,1,1,0,1,1,1,0,1,0,1,1},
	{1,4,0,1,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,1,4,0,1},
	{1,0,1,1,4,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,4,1},
	{1,4,1,1,0,1,4,0,4,0,4,0,4,0,4,0,4,0,1,4,1,1,0,1},
	{1,0,1,1,4,1,0,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,4,1},
	{1,4,0,4,0,4,0,4,0,4,0,4,1,4,0,4,0,4,0,4,0,4,0,1},
	{1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,4,1},
	{1,4,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,0,1},
	{1,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

	//putpixel( 5 ,5, SDL_MapRGB(map->format,0,128,0));
	//putpixel( 5 ,5, (255*256*256)+255*256);

	//previousTime = currentTime = 0;
	/* message pump */
	while (!gameover)
	{
		SDL_Event event;

		if (move) {
			printf("le pacman bouge \n");
			//SDL_FillRect(candy2, NULL , SDL_MapRGB(candy2->format,0,0,0));
        		
		
		
		if ( (rcG1.x == rcSprite.x) && (rcG1.y == rcSprite.y) || (rcG2.x == rcSprite.x) && (rcG2.y == rcSprite.y)  || (rcG3.x == rcSprite.x) && (rcG3.y == rcSprite.y)  ) {

				printf ("gameover");
					
	 	/* Display screen game over 5 seconds */ 
	    	 /* SDL_BlitSurface(map,NULL,screen,NULL);
	    	 	 SDL_Flip(screen);
	    	  	SDL_Delay(5000);*/
				
		}
			move = 0;
		}

		/* look for an event */
		if (SDL_PollEvent(&event)) {
			HandleEvent(map, event);
		}

        HandleMovements();
		/* collide with edges of screen */
		if (rcSprite.x <= 0)
			rcSprite.x = SCREEN_WIDTH - SPRITE_WIDTH -1;
		if (rcSprite.x >= SCREEN_WIDTH - SPRITE_WIDTH)
			rcSprite.x = 1;

		if (rcSprite.y <= 0)
			rcSprite.y = 0;
		if (rcSprite.y >= SCREEN_HEIGHT - SPRITE_HEIGHT)
			rcSprite.y = SCREEN_HEIGHT - SPRITE_HEIGHT;


	/* draw the map */
	SDL_BlitSurface(map,NULL,screen,&rcmap);

	int m = (rcSprite.x)/32;
        int n = (rcSprite.y)/32;
	//printf("m= %d n= %d ", m,n);

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
				//printf("OK");
				rcCandy.x = j * 32;
				rcCandy.y = i * 32;
				SDL_BlitSurface(candy, NULL, screen, &rcCandy);
			}
			/*if ( pos_Wall[m][n] != 0 ){
				//printf("PAS BON");
				//rcSprite.y = 0;

			}*/
		}
	}


	/*for (i=0; i< NY;i++){
		for (j=0;j<NX;j++){
			printf("%d",pos_Wall[i][j]);
		}
	}
	*/

		//SDL_BlitSurface(bloc, NULL, screen, &rcBloc);


		/* draw the sprite */
		SDL_BlitSurface(sprite, &rcSrc, screen, &rcSprite);


		/* draw the candy */
		//SDL_BlitSurface(candy, NULL, screen, &rcCandy);

		/* draw the candy 2 */
		//SDL_BlitSurface(candy2, NULL, screen, &rcCandy2);

		/* draw the candy 3 */
		//SDL_BlitSurface(candy3, NULL, screen, &rcCandy3);

		/* draw the candy 4 */
		//SDL_BlitSurface(candy4, NULL, screen, &rcCandy4);

		/* draw the GHOST 1 */
		SDL_BlitSurface(g1, NULL, screen, &rcG1);

		/* draw the GHOST 2 */
		SDL_BlitSurface(g2, NULL, screen, &rcG2);

		/* draw the GHOST 3 */
		SDL_BlitSurface(g3, NULL, screen, &rcG3);

		/* update the screen */
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	}

	/* clean up */
	SDL_FreeSurface(sprite);
	SDL_FreeSurface(wall);
	SDL_FreeSurface(wall2);
	SDL_FreeSurface(bloc);
	SDL_FreeSurface(candy);
	/*SDL_FreeSurface(candy2);
	SDL_FreeSurface(candy3);
	SDL_FreeSurface(candy4);*/
	SDL_FreeSurface(g1);
	SDL_FreeSurface(g2);
	SDL_FreeSurface(g3);
	SDL_FreeSurface(map);
	SDL_Quit();

	return 0;
}


/*****************/
/**Regle du Jeu***/
/*****************/
/*
J'hésite entre faire 2 choses lorsque le pacman meurt :
-soit il clignote et il devient invincible pendant une courte période
-soit il repart à sa place comme au début






(les fantômes)
En état "normal" il sont en couleurs et se déplacent de manière à se rapprocher du pacman.
Si le pacman rencontre un fantôme en état "normal", la partie est perdue.
Il y a une super-pastille à chaque coin de l'écran. Si le pacman en mange une, les fantômes passent à l'état "effrayé" pendant une durée déterminée.
Dans cet état les fantômes sont bleus et ralentis.
Ils se déplacent de manière à s'éloigner du pacman.
Si le pacman rencontre un fantôme en état "effrayé", le fantôme est mangé.
Ce fantôme retourne alors au centre (on ne voit que ses yeux durant ce retour) et redevient à l'état "normal".
La partie est gagnée lorsque le pacman a mangé toutes les pastilles du jeu.

Chaque fantôme a un comportement qui lui est propre :

    Blinky attaque directement Pac Man. Il suit Pac-Man comme son ombre.
    Pinky a tendance à se mettre en embuscade. Elle vise l'endroit où va se trouver Pac-Man.
    Inky est capricieux. De temps en temps, il part dans la direction opposée à Pac-Man.
*/
