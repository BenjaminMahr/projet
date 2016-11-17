#include "SDL.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
//#include <SDL/SDL_mixer.h>

#define SCREEN_WIDTH  720	
#define SCREEN_HEIGHT 797
#define SPRITE_WIDTH 32
#define SPRITE_HEIGHT 32

#define G1_WIDTH 32
#define G1_HEIGHT 34

/*
#define H 797
#define J 720
*/

/* rgb chez nous et bgr sur sdl
il faut que vert soient inferieur 2f genre 50*/

int gameover;// previousTime, currentTime entier qui stocke le temps
int move; //gere le déplacement du pacman


float dirX, dirY,x,y;

/* source and destination rectangles */
int SDL_FillRect(SDL_Surface *dst, SDL_Rect *dstrect, Uint32 color);
SDL_Rect rcSrc, rcSprite,rcG1, rcSG1, rcG2,rcG3,rcSG2,rcSG3;// rcCandy, rcCandy2, rcCandy3, rcCandy4, 


		//rcSG1 -> ghost param image

int a , continuer ;
int Convertir(float nb)
{
	nb += 0.5;
	int a = (int) nb;
	return a;
}
SDL_Surface *map;

Uint32 getpixel(SDL_Surface *map, int x, int y) {
    if (x<0 || y<0 || x>=map->w || y>=map->h) return 0;
    Uint8 *p = (Uint8 *)map->pixels + y*map->pitch + x*map->format->BytesPerPixel;
    return p[0] | p[1] << 8 | p[2] << 16; // TODO if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
}

void putpixel(int x, int y, Uint32 pixel) {
    int bpp_ = map->format->BytesPerPixel;
    if (x<0 || y<0 || x>=map->w || y>=map->h) return;
    Uint8 *p = (Uint8 *)map->pixels + y*map->pitch + x*bpp_;
    for (int i=0; i<bpp_; i++) {
        p[i] = ((Uint8*)&pixel)[i]; // TODO if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
    }
}


unsigned char green(Uint32 color) {
	return (color & (255*256))/256;
} 

unsigned char blue(Uint32 color) {
	return (color & 255);
} 

unsigned char red(Uint32 color) {
	return (color & (255*256*256))/(256*256);
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
void pleinecran();
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
					move=1;
					rcSrc.y = SPRITE_HEIGHT ;
					rcSrc.x = rcSrc.x - SPRITE_WIDTH;
					if (rcSrc.x < 0) {
						rcSrc.x = 5 * SPRITE_WIDTH;
					}
				   /*
					while ((getpixel(map, rcSprite.x-1, rcSprite.y)) != getpixel(map, 5, 5)) {
						rcSprite.x -= 5;
			
					}
						*/
					
					
					
					
					break;
				case SDLK_RIGHT:
					move=1;
					rcSrc.y = 0;
					//currentTime = SDL_GetTicks();
					//if(currentTime - previousTime > 50 ){ //temps en ms entre chq animation				
						rcSrc.x = rcSrc.x + SPRITE_WIDTH;
						if(rcSrc.x > 5 * SPRITE_WIDTH){
						rcSrc.x = 0;
						}
					//previousTime = currentTime;
					//}

					
					
					/*	
					
					
					if (!getpixel(map, rcSprite.x+33, rcSprite.y)) {
					 	rcSprite.x += 5;
					}*/
					
		        
					break;
				case SDLK_UP:
					move=1;
					rcSrc.y = 2 * SPRITE_HEIGHT;
					rcSrc.x = rcSrc.x + SPRITE_WIDTH;
					if ( rcSrc.x > 5 * SPRITE_WIDTH) {
						rcSrc.x = 0;
					}

					/*
					while ((getpixel(map, rcSprite.x-1, rcSprite.y)) != getpixel(map, 5, 5)) {
						rcSprite.y -= 5;
			
					}
						
					if (!getpixel(map, rcSprite.x-1, rcSprite.y-1)) {
					 	rcSprite.y -= 5;
					}*/


					break;
				case SDLK_DOWN:
					move=1;
					rcSrc.y = 3 * SPRITE_HEIGHT;
					rcSrc.x = rcSrc.x + SPRITE_WIDTH;
					if ( rcSrc.x >= 5 * SPRITE_WIDTH) {
						rcSrc.x = 0;
					}

					/*
					while ((getpixel(map, rcSprite.x-1, rcSprite.y)) != getpixel(map, 5, 5)) {
						rcSprite.y += 5;
			
					}
						
					if (!getpixel(map, rcSprite.x, rcSprite.y+33)) {
					 	rcSprite.y += 5;
					}
					*/
					break;
				
				case SDLK_w:	// LEFT
					move=1;
					rcSG1.y = 0;
					rcSG1.x = rcSG1.x - 32;
					if (rcSG1.x <= 0) {
						rcSG1.x = 32;
					}	
					/*if (!getpixel(map, rcG1.x-1, rcG1.y)) {
					 	rcG1.x -= 5;
					}*/
					

									
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
				
				 	rcG1.y += 5;
					break;

				case SDLK_x:	//DOWN
					move=1;
					rcSG1.y = 4 * G1_HEIGHT;
					rcSG1.x = G1_WIDTH;
					if ( rcSG1.x >= G1_WIDTH) {
						rcSG1.x = 0;
					}	
					 rcG1.y -= 5;	
					
					break;

				case SDLK_b:	
					pleinecran;					
					break;
				
			}
			break;
			

		
	}
}
/*
void pleinecran(){
SDL_WINDOW_FULLSCREEN_DESKTOP(map);
}*/

int main(int argc, char* argv[])
{
	SDL_Surface *screen, *temp, *sprite, *g1, *g2, *g3; //*candy, *candy2, *candy3, *candy4, 
	SDL_Rect rcmap;
	int colorkey;
	int i,j;

	/* print candy */

	/*
	for (i=0; i<5; i++) {
		for (j=0;j<5;j++) {
		pac_array[i][j] = candy;
		}	
	}
	*/
	
	/*initialize move of pacman */
	move=0;
	
	/* initialize SDL */
	SDL_Init(SDL_INIT_VIDEO);

	/* set the title bar */
	SDL_WM_SetCaption("Pacman", "Pacman");

	/* create window */
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

	/* set keyboard repeat */
	SDL_EnableKeyRepeat(30,30);

	/* load sprite */
	temp   = SDL_LoadBMP("pacman.bmp");
	sprite = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	/* setup sprite colorkey and turn on RLE*/ 
	colorkey = SDL_MapRGB(screen->format, 0,0,0);	
	SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
	
	/* load candy */
	/*temp   = SDL_LoadBMP("bonbon.bmp");
	candy = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
*/
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
	/*colorkey = SDL_MapRGB(screen->format, 0,0,0);	
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
	temp  = SDL_LoadBMP("map2.bmp");
	map = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* set sprite position */
	rcSprite.x = SCREEN_WIDTH/2 - 16; 
	rcSprite.y = SCREEN_HEIGHT/2 + 32; 

	/* set candy positon */
	/*rcCandy.x = SCREEN_WIDTH/2 - 80;
	rcCandy.y = SCREEN_HEIGHT/2 + 40;
*/
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
	rcG1.x = SCREEN_WIDTH/2 -64; 
	rcG1.y = SCREEN_HEIGHT/2 -40; 

	/* set G2 positon */		//b
	rcG2.x = SCREEN_WIDTH/2-16;
	rcG2.y = SCREEN_HEIGHT/2 -40;

	/* set  positon */		//w
	rcG3.x = SCREEN_WIDTH/2+32;
	rcG3.y = SCREEN_HEIGHT/2 -40;

	
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

	/* set animation frame */
	rcSG2.x = 0; 
	rcSG2.y = 0;
	rcSG2.w = G1_WIDTH;
	rcSG2.h = G1_HEIGHT;

	/* set animation frame */
	rcSG3.x = 0; 
	rcSG3.y = 0;
	rcSG3.w = G1_WIDTH;
	rcSG3.h = G1_HEIGHT;

	rcmap.x = 0;
	rcmap.y = 0;

	gameover = 0;
	/*
	float pos_candy[H][J];
	for (i=0;i<H;i++){
		for(j=0;j<J;j++){
			pos_candy[i][j]=0;
		}
	}
pos_candy[rcCandy.x][rcCandy.y]=1;
	pos_candy[rcCandy2.x][rcCandy2.y]=1;
	pos_candy[rcCandy3.x][rcCandy3.y]=1;
	pos_candy[rcCandy4.x][rcCandy4.y]=1;
*/

	putpixel (5, 5, (255));

	SDL_EnableKeyRepeat(3,3);
	//previousTime = currentTime = 0;
	/* message pump */
	while (!gameover)
	{


		SDL_Event event;

		if (move) {
			//SDL_FillRect(candy2, NULL , SDL_MapRGB(candy2->format,0,0,0));
		
		/*
		if(event.key.keysym.sym == SDLK_RIGHT) {
    			while (!getpixel(map, rcSprite.x-1, rcSprite.y) ) {
				rcSprite.x += 1;
			}
		}
		if(event.key.keysym.sym == SDLK_UP) {
    			while (!getpixel(map, rcSprite.x-1, rcSprite.y) ) {
				rcSprite.y += 5;
			}
		}
			*/
		if(event.key.keysym.sym == SDLK_RIGHT ){
			rcSG1.y = 4 * G1_HEIGHT;
			rcSG1.x = G1_WIDTH;
			if ( rcSG1.x >= G1_WIDTH) {
			rcSG1.x = 0;
			}		
			rcG1.y -= 5;
			rcG2.x += 5;	
			rcG3.x -= 5;
		
			continuer = 1;
			while (continuer){
				if ((!getpixel(map, rcSprite.x-1, rcSprite.y)) ) {			 	
						rcSprite.x += 5;	
						
					}
					else
					{
						continuer=0;	
					}
					}
		}
		
		if(event.key.keysym.sym == SDLK_LEFT ){
			
			
			continuer = 1;
			while (continuer){
				rcG1.y += 5;
				rcG2.x -= 5;
				rcG3.x += 5;
				if ((!getpixel(map, rcSprite.x-1, rcSprite.y)) ) {			 	
						rcSprite.x -= 5;	
						
					}
					else
					{
						continuer=0;	
					}
					}
		}
		if(event.key.keysym.sym == SDLK_UP ){
			rcSG2.y = 4 * G1_HEIGHT;
			rcSG2.x = G1_WIDTH;
			if ( rcSG2.x >= G1_WIDTH) {
			rcSG2.x = 0;
			}	
			rcG1.x -= 5;
			rcG2.y -= 5;
			rcG3.x += 5;

			continuer = 1;
			while (continuer){
				if ((!getpixel(map, rcSprite.x-1, rcSprite.y)) ) {			 	
						rcSprite.y += 5;	
						
					}
					else
					{
						continuer=0;	
					}
					}
		
		}
		if(event.key.keysym.sym == SDLK_DOWN ){
			rcSG3.y = 4 * G1_HEIGHT;
			rcSG3.x = G1_WIDTH;
			if ( rcSG3.x >= G1_WIDTH) {
			rcSG3.x = 0;
			}	
			rcG1.x += 5;
			rcG2.y += 5;
			rcG3.y -= 5;

			continuer = 1;
			while (continuer){
				if ((!getpixel(map, rcSprite.x-1, rcSprite.y)) ) {			 	
						rcSprite.y -= 5;	
						
					}
					else
					{
						continuer=0;	
					}
					}
		}
		
		
		
		if ( (rcG1.x == rcSprite.x) && (rcG1.y == rcSprite.y) ) {
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
	/*SDL_FreeSurface(candy);
	SDL_FreeSurface(candy2);
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
