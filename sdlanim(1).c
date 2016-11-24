//groupe 1.4
//27/05/2016
#include "SDL.h"
#include <math.h>
#include <time.h>
#include<stdio.h>
#include<stdbool.h>
//#include <SDL/SDL_mixer>

#define SCREEN_WIDTH  720 // horizontal size of the frame
#define SCREEN_HEIGHT 540	// vertical size of the frame
#define SPRITE_SIZE   157  
#define PI 3.14159265359
#define BOARD_LEFT 200 // x-coord of the left border of the game board
#define BOARD_RIGHT 520 // x-coord of the right border of the game board
#define BOARD_TOP 31 	// y-coord of the top border of the game board
#define BOARD_BOTTOM 422 // y-coord of the bottom border of the game board (will be useful in the next level)
#define LAUNCHER_WIDTH 187 // width of the launcher surface
#define LAUNCHER_HEIGHT 157 // height of the launcher surface
#define LAUNCHER_CENTER 94 // y-coord of the launcher center
#define LAUNCHER_DIV 48
#define BUB_SIZE 40// horizontal and vertical size of the bubble surface
#define VELOCITY 1  //speed
#define BUB_NX 8 	// max. number bubbles in horizontal direction
#define BUB_NY 11 	// max. number of bubbles in vertical direction
#define NUM_COLORS 8	// number of bubbles's colors
#define SUP_LAUNCH_HEIGHT 116
#define SUP_L_HEIGHT 60  
#define GEAR_WIDTH 126 // width of the gears surface
#define GEAR_HEIGHT 116 // height of the gears surface
#define WHEEL_WIDTH 47   // width of the wheel surface
#define WHEEL_HEIGHT 33  // height of the wheel surface
#define vitesseforme 60  // speed of the animation in bubble

int gameover;

/* source and destination rectangles */
SDL_Rect rcSrc, rcSprite, rcBub, rcSrc_bub,rcfond,rcSrcwheel,rcSrcgear,rcGear,rcWheel;
float dirX, dirY, theta, delta, pos_arrow, x,y;
int move;
int bub_array[BUB_NY][BUB_NX];
int bub_array_centers[BUB_NY][BUB_NX][2];
int bub_connected_component[BUB_NY][BUB_NX];
unsigned int i;
int current_col, collide;
int compteur = 0 ;
//Mix_Music *music, *scream;

//int Mix_OpenAudio(int frequency, Uint16 format, int channels, int chunksize);	

void print_array(int array[BUB_NY][BUB_NX]);
	
void HandleEvent(SDL_Event event);			



int main(int argc, char* argv[])
{
  SDL_Surface *screen, *temp, *sprite, *grass, *bub[NUM_COLORS], *tmp, *gear, *wheel , *gover,*menus , *victory,*fond;	
	
  //SDL_Surface *screamer;
  // int compteur;
  //cpt = 0;

  SDL_Rect rcGrass;
  float dist,dx ,dy;
  int colorkey,i,j,col,l;

  srand(time(NULL));
  current_col = rand()%NUM_COLORS+1;


  /* initialize pos_arrow and move */
  pos_arrow = 0;  		
  move = 0;
	
  /* initialize SDL */
  SDL_Init(SDL_INIT_VIDEO);
  /* SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
     Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);  
     Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
     music = Mix_LoadMUS("dbz.wav");
     scream = Mix_LoadMUS("scream.wav");
     if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1){
     printf("%s", Mix_GetError());
     }
  */

  /* set the title bar */
  SDL_WM_SetCaption("Bubble shot", "Bubble shot");  // title of window

  /* create window */
  screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

  /* set keyboard repeat */
  SDL_EnableKeyRepeat(30, 30);  // speed of the arrow

  
  /* table initialized with bubbles on the first three lines */
  for (i = 0; i<3; i++){
    for (j = 0; j<BUB_NX; j++){
      bub_array[i][j] = current_col;
      current_col = rand()%8+1;
     
    }
  }


 

  /*load gear*/
  temp = SDL_LoadBMP("frame/frame_gears.bmp");
  gear = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  /*load gear*/
  temp = SDL_LoadBMP("frame/frame_wheel.bmp");
  wheel = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  /*load victory */
 
  temp=SDL_LoadBMP("image/victoire.bmp");
  victory= SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  /*load game over */
 
  temp=SDL_LoadBMP("image/gameover.bmp");
  gover= SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  /*load menu */
 
  temp=SDL_LoadBMP("image/ecran.bmp");
  menus= SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  /* load sprite */
  temp   = SDL_LoadBMP("frame/frame_launcher.bmp");
  sprite = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

	
  /* load bub */   
  tmp = SDL_LoadBMP("image/bub_blue.bmp");
  bub[0] = SDL_DisplayFormat(tmp);
  SDL_FreeSurface(tmp);
	
  tmp = SDL_LoadBMP("image/bub_black.bmp");
  bub[1] = SDL_DisplayFormat(tmp);
  SDL_FreeSurface(tmp);
		
  tmp  = SDL_LoadBMP("image/bub_green.bmp");
  bub[2] = SDL_DisplayFormat(tmp);
  SDL_FreeSurface(tmp);
		
  tmp = SDL_LoadBMP("image/bub_orange.bmp");
  bub[3] = SDL_DisplayFormat(tmp);
  SDL_FreeSurface(tmp);
	
  tmp = SDL_LoadBMP("image/bub_purple.bmp");
  bub[4] = SDL_DisplayFormat(tmp);
  SDL_FreeSurface(tmp);
		
  tmp = SDL_LoadBMP("image/bub_red.bmp");
  bub[5] = SDL_DisplayFormat(tmp);
  SDL_FreeSurface(tmp);
		
  tmp = SDL_LoadBMP("image/bub_white.bmp");
  bub[6] = SDL_DisplayFormat(tmp);
  SDL_FreeSurface(tmp);
	
  tmp = SDL_LoadBMP("image/bub_yellow.bmp");
  bub[7] = SDL_DisplayFormat(tmp);
  SDL_FreeSurface(tmp);
	
  /******/

  
  colorkey = SDL_MapRGB(screen->format, 255, 0, 255);
  SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

  colorkey = SDL_MapRGB(screen -> format,255,0,255);
  SDL_SetColorKey(wheel    , SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);


  colorkey = SDL_MapRGB(screen -> format,255,0,255);
  SDL_SetColorKey(gear    , SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
  
  colorkey = SDL_MapRGB(screen -> format,255,0,255);
  SDL_SetColorKey(gover , SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

  colorkey = SDL_MapRGB(screen -> format,255,0,255);
  SDL_SetColorKey(menus, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

  colorkey = SDL_MapRGB(screen -> format,255,0,255);
  SDL_SetColorKey(victory, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

 
  SDL_BlitSurface(menus,NULL,screen,NULL);
  SDL_Flip(screen);
  SDL_Delay(8000);
  

  /* setup sprite colorkey and turn on RLE */
  for(i = 0; i < NUM_COLORS; i++){
    SDL_SetColorKey(bub[i], SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey); 
  }


  /* load grass */
  temp  = SDL_LoadBMP("frame/frame_1p.bmp");
  grass = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  SDL_SetColorKey(grass, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);



  /*load fond */
 
  temp=SDL_LoadBMP("image/fond.bmp");
  fond= SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  SDL_SetColorKey(fond, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
  /*******/



  /* set sprite position */
  rcSprite.x = 540/2-SPRITE_SIZE;
  rcSprite.y = 720-157/2 ;
	
  /* set  fond */
  rcfond.x = 0;
  rcfond.y = 0;
	

  /* set animation frame */
  rcSrc.x = 0;
  rcSrc.y = 0;
  rcSrc.w = 187;
  rcSrc.h = 157;

  /* set animation frame */  
  rcSrc_bub.x = 0;
  rcSrc_bub.y = 0;
  rcSrc_bub.w = 40;
  rcSrc_bub.h = 40;


  /* set the gear position */
  rcGear.x = SCREEN_WIDTH/2 - 55;
  rcGear.y = SCREEN_HEIGHT - 125;

  /* set animation gear */
  rcSrcgear.x = 0;
  rcSrcgear.y = 0;
  rcSrcgear.w = 126;
  rcSrcgear.h = 116;

  /* set wheel position */
  rcWheel.x = SCREEN_WIDTH/2 + 70;
  rcWheel.y = SCREEN_HEIGHT - 58;

  /* set animation wheel */
  rcSrcwheel.x = 0;
  rcSrcwheel.y = 188;
  rcSrcwheel.w = 32;
  rcSrcwheel.h = 47;


  gameover = 0;

  /* set bub position */
  x = rcBub.x = SCREEN_WIDTH/2 - BUB_SIZE/2;   
  y = rcBub.y = SCREEN_HEIGHT-LAUNCHER_HEIGHT+LAUNCHER_CENTER-BUB_SIZE/2;
		

  /*  initializing the coordinates of the centers of the table boxes with odd lines and even lines*/
  for(l = 1; l < BUB_NY ; l = l+2){ 
    for(col = 0; col < BUB_NX ; col++){
      bub_array_centers[l][col][0] = BOARD_LEFT+BUB_SIZE*col + BUB_SIZE ;
      bub_array_centers[l][col][1] = BOARD_TOP+35*l + BUB_SIZE/2;
    }
  }
  for(l = 0; l < BUB_NY ; l = l+2){ 
    for(col = 0; col < BUB_NX ; col++){
      bub_array_centers[l][col][0] = BOARD_LEFT+BUB_SIZE*col + BUB_SIZE/2 ;
      bub_array_centers[l][col][1] = BOARD_TOP+35*l + BUB_SIZE/2;
    }
  }

  while (!gameover)
    {
      /* 	Mix_PlayMusic(music, 1);     // POUR LANCER LA MUSIQUE !!! */
      /*   	while (Mix_PlayingMusic() == 1)  */
      /* {  */
      /*     /\* Tant que la musique n'est pas terminée, on fait quelque chose  */
      /*        car sinon le programme s'éteint et le son aussi *\/ */
      /*     SDL_Delay(10);  */
      /* } */

      
      /* loop realizing the animation in the bubbles */
      compteur++;
      if(compteur%vitesseforme == 0 ){
	compteur = 0;
	rcSrc_bub.y = ( rcSrc_bub.y + BUB_SIZE ) % 960;
      }

      SDL_Event event;

		
      if(move){

	x += dirX;
	y += dirY;
	rcBub.x = x;
	rcBub.y = y;
			
	if (rcBub.x < BOARD_LEFT){	// coordinates of the bubbles : border left
	  x = rcBub.x = 2 * BOARD_LEFT - rcBub.x;	
	  dirX = -dirX;			
								
	}

	if(rcBub.x > BOARD_RIGHT-BUB_SIZE){	// coordinates of the bubbles :border right
			
	  x = rcBub.x = 2 * BOARD_RIGHT - rcBub.x - 2*BUB_SIZE;
	  dirX = -dirX;
				
	}
  
	if(rcBub.y < BOARD_TOP) // coordinates of the bubbles : border top
	  {
	    y = rcBub.y = BOARD_TOP;
	    for (i = 0; i < BUB_NX; i++) {

	      float pos_x = BOARD_LEFT + i*BUB_SIZE;
	      if (fabs(rcBub.x - pos_x) <= BUB_SIZE/2)
		{
		  x = rcBub.x= BOARD_LEFT+i*BUB_SIZE; 
		  bub_array[0][i]= current_col;
		}
	    }
			    
	    move = 0;
	    x = rcBub.x = SCREEN_WIDTH/2 - BUB_SIZE/2;   
	    y = rcBub.y = SCREEN_HEIGHT-LAUNCHER_HEIGHT+LAUNCHER_CENTER-BUB_SIZE/2; 
	    current_col = rand()%NUM_COLORS+1;


	  }
	else{
	  	
	  collide = 0;
	  for ( i = 0 ; i < BUB_NY ; i++){
	    for( j = 0 ; j < BUB_NX ; j++){
	      if(bub_array[i][j]>0){
		dx = rcBub.x + BUB_SIZE/2 - bub_array_centers[i][j][0];
		dy = rcBub.y + BUB_SIZE/2 - bub_array_centers[i][j][1];
		dist = sqrt(dx*dx + dy*dy);
		if ( dist < 0.87 * BUB_SIZE){
		  collide = 1;
		 
		}
	      }
	    }
	    
	  }

	  /* loop of the collision */

	  if (collide){
	    int added = 1; // variable that will serve us to go into a loop if a condition is met
	    int count = 1;  // variable that will count if at least 3 balls of the same color are touching

	    /* initialized array for connected components */
	    for (i = 0; i<BUB_NY; i++){
	      for (j = 0; j<BUB_NX; j++){
		bub_connected_component[i][j]= 0;
	      }
	    }
	    
	    /* loop which sticks the ball thrown at him glued to the plate or another bubble */

	    for(l = 0 ; l<BUB_NY;l++){
	      for (col = 0; col < BUB_NX - l%2 ; col++){
		if (bub_array[l][col] == 0){
		  dx = rcBub.x + BUB_SIZE/2 - bub_array_centers[l][col][0];
		  dy = rcBub.y +BUB_SIZE/2 - bub_array_centers[l][col][1];
		  dist = sqrt((dx*dx) + (dy*dy)); 
		
	
		  if (dist <= BUB_SIZE/2){
		    bub_array[l][col]=current_col;
		    bub_connected_component[l][col]=1;
		  }
		}
	      }
	    }
	    /*  loop through the array of connected components and counts the number of the same colored balls linked  */
	    while (added) {
	      added = 0;
	      for(l = 0 ; l<BUB_NY;l++){
		for (col = 0; col < BUB_NX - l%2 ; col++){
		  if (current_col==bub_array[l][col] && bub_connected_component[l][col]==0) {
		    for (i=0;i<BUB_NY;i++){
		      for(j=0;j<(BUB_NX - (i%2));j++){
			if (bub_connected_component[i][j]==1) {
			  dx=bub_array_centers[i][j][0] - bub_array_centers[l][col][0];
			  dy=bub_array_centers[i][j][1] - bub_array_centers[l][col][1];
			  dist=sqrt((dx*dx)+(dy*dy));
			  if (dist<=sqrt(2)*BUB_SIZE){
			    bub_connected_component[l][col]=1;
			    added = 1;
			    count++;
			  }
			} 
		      }
		    }
		  }
		}
	      }
	    }

	    /* if 3 or more balls of the same color are touching : bubbles burst */
	    if (count >= 3) {
	      for (i=0;i<BUB_NY;i++){
		for(j=0;j<BUB_NX;j++){
		  if(bub_connected_component[i][j]==1)
		    {
		      bub_array[i][j]=0;
		    }
		}
	      }
	    
	      /* initialized array for connected components */
	      for (i = 0; i<BUB_NY; i++){
		for (j = 0; j<BUB_NX; j++){
		  bub_connected_component[i][j]= 0;
		}
	      }
	      /* related components for making a bubble placed against the top of the table*/
	      for (col = 0; col < BUB_NX  ; col++){
		if (bub_array[0][col]>0){
		  bub_connected_component[0][col]=1;
		}
	      }

	      /* loop through the array of connected components to make 1 all bubbles associated with that of the first line */
	      added =1 ;
	      
	      while (added) {
		added = 0;
		for(l = 0 ; l<BUB_NY;l++){
		  for (col = 0; col < BUB_NX - l%2 ; col++){
		    if (bub_connected_component[l][col]==0 && bub_array[l][col] > 0) {
		      for (i=0;i<BUB_NY;i++){
			for(j=0;j<(BUB_NX - (i%2));j++){
			  if (bub_connected_component[i][j]==1) {
			    dx=bub_array_centers[i][j][0] - bub_array_centers[l][col][0];
			    dy=bub_array_centers[i][j][1] - bub_array_centers[l][col][1];
			    dist=sqrt((dx*dx)+(dy*dy));
			    
			    if (dist<=1.1*BUB_SIZE){
			      bub_connected_component[l][col]=1;
			      added = 1;
			    }
			  }
			}
		      }
		    }
		  }
		}
	      }

	      /* loop which breaks all the bubbles which are not hooked to the top tray related balls */
	      for (i=0;i<BUB_NY;i++){
		for(j=0;j<BUB_NX;j++){
		  if(bub_connected_component[i][j]==0)
		    {
		      bub_array[i][j]=0;
		    }
		}
	      } 

	      
	    }
	  
	  
	    /*  collision detection and so launched the ball beyond the 11 */
	    if(collide && (rcBub.y>(9*BUB_SIZE+BOARD_TOP))) {
	      printf("\n YOU LOOSE :( \n GAME OVER !!! \n NEW GAME - GOOD LUCK \n\n");  // GAME OVER , les bulles ont atteint les 11 lignes

	      // cpt ++;
	      //if (cpt > 3 ) {

	      /*load screamer */ 
	      /*  temp =SDL_LoadBMP("scream.bmp");
		  gover= SDL_DisplayFormat(temp);
		  SDL_FreeSurface(temp);


		  Mix_PlayMusic(scream, 1); 
		  }
	      */

	      for (i = 0; i<BUB_NY; i++) {
		for ( j = 0; j<BUB_NX - i%2; j++) {
		  bub_array[i][j]=0;
		}
	      }
	      

	      /* Display screen game over 5 seconds */ 
	      SDL_BlitSurface(gover,NULL,screen,NULL);
	      SDL_Flip(screen);
	      SDL_Delay(5000);

	      /* View menu after endgame */
	      SDL_BlitSurface(menus,NULL,screen,NULL);
	      SDL_Flip(screen);
	      SDL_Delay(8000);
  
	     
	     
	     

	      /*  table initialized with bubbles on the first three lines */
	      for (i = 0; i<3; i++){
		for (j = 0; j<BUB_NX; j++){
		  bub_array[i][j] = current_col;
		  current_col = rand()%8+1;
     
		}
	      }
   
	    }
	    
	    if (collide )
	      {
		int victorie;
		victorie = 1 ;
		for(j=0;j<BUB_NX;j++){
		  if(bub_array[0][j]!=0)
		    {
		      victorie=0;
		    }
		}
		if (victorie)
		  {
		    printf("\n VICTORY !!! \n \t :) \n  NEW GAME \n\n");  // Victoire , aucune bulle ne se trouve sur la premiere ligne du tableau

		    /*  Display screen victory 5 seconds  */ 
		    SDL_BlitSurface(victory,NULL,screen,NULL);
		    SDL_Flip(screen);
		    SDL_Delay(5000);

		    /*  View menu after endgame */
		    SDL_BlitSurface(menus,NULL,screen,NULL);
		    SDL_Flip(screen);
		    SDL_Delay(8000);
  
	     
	     
	     

		    /* table initialized with bubbles on the first three lines */
		    for (i = 0; i<3; i++){
		      for (j = 0; j<BUB_NX; j++){
			bub_array[i][j] = current_col;
			current_col = rand()%8+1;
     
		    


		      }
		    }
		  }
	      }
	    
	    /* Then again new ball */
		 
	    move=0;
	    x = rcBub.x = SCREEN_WIDTH/2 - BUB_SIZE/2;   
	    y = rcBub.y = SCREEN_HEIGHT-LAUNCHER_HEIGHT+LAUNCHER_CENTER-BUB_SIZE/2; 
	    current_col=rand()%NUM_COLORS+1;
	  }
	}
      }
      
		
      /* look for an event */
      if (SDL_PollEvent(&event)){
	HandleEvent(event);
      }

      /* coordinates of the images */
      rcSprite.x = SCREEN_WIDTH/2 - LAUNCHER_WIDTH/2;
      rcSprite.y = SCREEN_HEIGHT - LAUNCHER_HEIGHT;
      rcGrass.x = 0;
      rcGrass.y = 0;
      rcfond.x = 0;
      rcfond.y = 0;
			
      SDL_BlitSurface(fond, NULL, screen, &rcfond);
      SDL_BlitSurface(grass, NULL, screen, &rcGrass);


      /* draw the wheel */
      SDL_BlitSurface(wheel, &rcSrcwheel, screen, &rcWheel);

      /* draw the gear */
      SDL_BlitSurface(gear, &rcSrcgear, screen, &rcGear);


      /* draw the sprite */
      SDL_BlitSurface(sprite, &rcSrc, screen, &rcSprite);

      /* draw the bubs */ 
      for(i=0;i<BUB_NY;i++){
	for (j=0;j<BUB_NX-i%2;j++){		
	  if(bub_array[i][j]!=0){
	    SDL_Rect rc;
	    rc.x = bub_array_centers[i][j][0] - BUB_SIZE/2;
	    rc.y = bub_array_centers[i][j][1] - BUB_SIZE/2;
	    SDL_BlitSurface(bub[bub_array[i][j]-1], &rcSrc_bub, screen, &rc);
	  }
	}
      }		    



      SDL_BlitSurface(bub[current_col-1], &rcSrc_bub, screen, &rcBub);

     

    
  
      /* update the screen */
      SDL_UpdateRect(screen, 0, 0, 0, 0);

     
    }
	
  /* clean up */
  SDL_FreeSurface(gear);
  SDL_FreeSurface(wheel);
  SDL_FreeSurface(gover);
  SDL_FreeSurface(sprite);
  SDL_FreeSurface(menus);
  SDL_FreeSurface(victory);
  SDL_FreeSurface(fond);
  SDL_FreeSurface(bub[current_col]);
  SDL_FreeSurface(grass);
  /* Mix_FreeMusic(music);
     Mix_FreeMusic(scream);*/
  SDL_Quit();

  return 0;
}


/* procedure that displays a table of connected components */
void print_array(int array[BUB_NY][BUB_NX]) {
  int l,col;

  for(l = 0 ; l<BUB_NY;l++){
    if (l%2)
      printf(" ");
    for (col = 0; col < BUB_NX - l%2 ; col++){
      printf("%d ", array[l][col]);
    }
    printf("\n");
  }
}






/* procedure that defines the actions of keyboard commands */
void HandleEvent(SDL_Event event)
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
      gameover = 1;
      break;

      /*case SDLK_p: 
	if(Mix_PausedMusic() == 1)//Si la musique est en pause
	{
	Mix_ResumeMusic(); //Reprendre la musique
	printf("p1");
	}
	else
	{
	Mix_PauseMusic(); //Mettre en pause la musique
	printf("p2"); 
	}
	break; */
    case SDLK_q:
      gameover = 1;
      break;
    case SDLK_LEFT:
      if ( rcSrc.y <= LAUNCHER_HEIGHT ) 
	rcSrc.y = LAUNCHER_HEIGHT; 
      else
	rcSrc.y -= LAUNCHER_HEIGHT; 
      if (pos_arrow > 0)
	pos_arrow --;

      /* Movement of the gear */
      if (rcSrcgear.y == GEAR_HEIGHT)
	{
	  rcSrcgear.y = GEAR_HEIGHT *39 ;
	}
      else{
	rcSrcgear.y -= GEAR_HEIGHT;
      }

      /* Movement of the wheel */
      if (rcSrcwheel.y == WHEEL_WIDTH){
	rcSrcwheel.y = WHEEL_WIDTH*15;
      }
      else {
	rcSrcwheel.y -= WHEEL_WIDTH ;
      }
					
      break;
    case SDLK_RIGHT:
      if ( rcSrc.y >= 6908 )
	rcSrc.y = 6908;
      else		
	rcSrc.y += 157;
      if (pos_arrow<45)
	pos_arrow ++;

      /* Movement of the gear */
      if (rcSrcgear.y == GEAR_HEIGHT *39)
	{
	  rcSrcgear.y = GEAR_HEIGHT;
	}
      else{
	rcSrcgear.y += GEAR_HEIGHT;
      }

      /* Movement of the wheel */
      if (rcSrcwheel.y == WHEEL_WIDTH*15){
	rcSrcwheel.y = WHEEL_WIDTH;
      }
      else {
	rcSrcwheel.y += WHEEL_WIDTH ;
      }
					
      break;

    case SDLK_UP:
			        
      break;
				
    case SDLK_DOWN:
		    
      break;
			       
    case SDLK_SPACE:	
      if (move == 1)
	{
	  break ;
	}
      move = 1;	
      theta = PI - (PI/48) * (pos_arrow+2);
      dirX = cos(theta) * VELOCITY;
      dirY = -sin(theta) * VELOCITY;
      break;
    }
    break;
  }
}
