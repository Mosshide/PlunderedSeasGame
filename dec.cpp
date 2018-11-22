/*






*/
#include "SDL\SDL.h"
#include "SDL\SDL_image.h"
#include "SDL\SDL_mixer.h"
#include <iostream>
#include <math.h>
#include <time.h>
using namespace std;

Uint32 sdltimer;
Uint32 lastf;
Uint32 tick;
bool quit;
int x;
int y;
int i;
int mousex;
int mousey;
bool end;
int endframe;
bool start;
int count;
//clouds
double cloudx;
double cloudy=50;
//wavec
double wavecx=-400;
double wavecxdif=0;
double wavecy=-150;
double wavecydif=0;
bool wavecback;
//waveb
double wavebx=-300;
double wavebxdif=0;
double waveby=-250;
double wavebydif=0;
bool wavebback=true;
//wavea
double waveax=-400;
double waveaxdif=0;
double waveay=-400;
double waveaydif=0;
bool waveaback;
//pboat
int speed;
int ticker;
int lastticker;
bool firing;
bool debris;
int firetimer;
bool fired;
//keys
bool adown;
bool ddown;



const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

SDL_Event event;

SDL_Surface *wavea = NULL;
SDL_Surface *waveb = NULL;
SDL_Surface *wavec = NULL;
SDL_Surface *bg = NULL;
SDL_Surface *ball = NULL;
SDL_Surface *usb = NULL;
SDL_Surface *usbs = NULL;
SDL_Surface *wrecka = NULL;
SDL_Surface *wreckb = NULL;
SDL_Surface *cloud = NULL;
SDL_Surface *king = NULL;
SDL_Surface *kingf = NULL;
SDL_Surface *kingd = NULL;
SDL_Surface *kingh = NULL;
SDL_Surface *kingj = NULL;
SDL_Surface *jeff = NULL;
SDL_Surface *jeff1 = NULL;
SDL_Surface *jeff2 = NULL;
SDL_Surface *jeff3 = NULL;
SDL_Surface *jeff4 = NULL;
SDL_Surface *jeff5 = NULL;
SDL_Surface *blue = NULL;
SDL_Surface *title = NULL;
SDL_Surface *screen = NULL;

//The music that will be played
Mix_Music *music = NULL;

//The sound effects that will be used
Mix_Chunk *cannon = NULL;
Mix_Chunk *high = NULL;
Mix_Chunk *med = NULL;
Mix_Chunk *low = NULL;

int flags;

struct usboat{
	int x;
	int y;
	int debrisx;
	int debrisy;
	int dtimer;
	bool alive;
};

usboat boat[5];

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image using SDL_image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old image
        SDL_FreeSurface( loadedImage );
        
        //If the image was optimized just fine
        if( optimizedImage != NULL )
        {
            //Map the color key
            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 255, 56, 255 );
            
            //Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
        }
    }

    //Return the optimized image
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Rectangle to hold the offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit the surface
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, (SDL_SWSURFACE,SDL_FULLSCREEN));

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }
    
    //Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return false;    
    }
    

    //Set the window caption
    SDL_WM_SetCaption( "Plundering US Seas", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load the image
    wavea = load_image( "img/wave1.png" );
    if( wavea == NULL )
    {
        return false;    
    }
    waveb = load_image( "img/wave2.png" );
    if( waveb == NULL )
    {
        return false;    
    }
    wavec = load_image( "img/wave3.png" );
    if( wavec == NULL )
    {
        return false;    
    }
    bg = load_image( "img/bg.png" );
    if( bg == NULL )
    {
        return false;    
    }
    ball = load_image( "img/ball.png" );
    if( ball == NULL )
    {
        return false;    
    }
    usb = load_image( "img/usboat.png" );
    if( usb == NULL )
    {
        return false;    
    }
    usbs = load_image( "img/usboatsmall.png" );
    if( usbs == NULL )
    {
        return false;    
    }
    wrecka = load_image( "img/wreck1.png" );
    if( wrecka == NULL )
    {
        return false;    
    }
    wreckb = load_image( "img/wreck2.png" );
    if( wreckb == NULL )
    {
        return false;    
    }
    cloud = load_image( "img/cloud.png" );
    if( cloud == NULL )
    {
        return false;    
    }
    king = load_image( "img/king.png" );
    if( king == NULL )
    {
        return false;    
    }
    kingf = load_image( "img/kingleft1.png" );
    if( kingf == NULL )
    {
        return false;    
    }
    kingd = load_image( "img/kingleft2.png" );
    if( kingd == NULL )
    {
        return false;    
    }
    kingh = load_image( "img/kingright1.png" );
    if( kingh == NULL )
    {
        return false;    
    }
    kingj = load_image( "img/kingright2.png" );
    if( kingj == NULL )
    {
        return false;    
    }
    jeff = load_image( "img/jeffer.png" );
    if( jeff == NULL )
    {
        return false;    
    }
    jeff1 = load_image( "img/jeffer1.png" );
    if( jeff1 == NULL )
    {
        return false;    
    }
    jeff2 = load_image( "img/jeffer2.png" );
    if( jeff2 == NULL )
    {
        return false;    
    }
    jeff3 = load_image( "img/jeffer3.png" );
    if( jeff3 == NULL )
    {
        return false;    
    }
    jeff4 = load_image( "img/jeffer4.png" );
    if( jeff4 == NULL )
    {
        return false;    
    }
    jeff5 = load_image( "img/jeffer5.png" );
    if( jeff5 == NULL )
    {
        return false;    
    }
    blue = load_image( "img/blue.png" );
    if( blue == NULL )
    {
        return false;    
    }
    title = load_image( "img/title.png" );
    if( title == NULL )
    {
        return false;    
    }
    //Load the music
    music = Mix_LoadMUS( "sound/lambda.wav" );
    //If there was a problem loading the music
    if( music == NULL )
    {
        return false;    
    }
    cannon = Mix_LoadWAV( "sound/cannon.wav" );
    
    //If everything loaded fine
    return true;    
}

void clean_up()
{
    //Free the surface
    SDL_FreeSurface( bg );
    
    //Free the music
    Mix_FreeMusic( music );
    

    //Quit SDL
    SDL_Quit();
}	


int main( int argc, char* args[] ) {
	init();
	load_files();
	//Set surface alpha
	SDL_SetAlpha( blue, SDL_SRCALPHA, 70 );
	
	
	//boatinit
	boat[0].alive = true;
	boat[1].alive = true;
	boat[2].alive = true;
	boat[3].alive = true;
	boat[4].alive = true;
	//srand(time(NULL));
	//boat[0].x = rand() % 640;
	boat[0].x = 0;
	//srand(time(NULL));
	boat[1].x = 400;
	//srand(time(NULL));
	boat[2].x = 0;
	//srand(time(NULL));
	boat[3].x = 200;
	//srand(time(NULL));
	boat[4].x = 400;
	
	while(quit == false){
		lastf = SDL_GetTicks() - sdltimer;
		sdltimer = SDL_GetTicks();
		tick += lastf;
		lastticker = ticker;
		if (tick > 100)
		{
			if (end == true){
				endframe += 1;
				if (endframe >= 6) endframe = 5;
			}
			if (fired == true)
			{
				firetimer += 1;
				if (firetimer >= 20)
				{
					firetimer = 0;
					fired = false;
				}
			}
			for (i=0; i<5; i++){
				if (boat[i].alive == false)
				{
					boat[i].dtimer += 1;
					if (boat[i].dtimer >= 6)
					{
						srand(time(NULL));
						if (i<2) boat[i].x = 640;
						if (i>1) boat[i].x = -200;
						boat[i].dtimer = 0;
						boat[i].alive = true;
						count += 1;
					}
				}
			}
			tick = 0;
			ticker += 1;
		}
		if (ticker == 20){
			ticker = 0;
			lastticker = -1;
			wavecback = !wavecback;
			wavebback = !wavebback;
			waveaback = !waveaback;
		}
		
		while( SDL_PollEvent( &event ) )
        {
			//If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
            
            if( event.type == SDL_KEYDOWN )
            {
				switch( event.key.keysym.sym )
                {
					case SDLK_ESCAPE: 
						quit = true;
                    	break;
                    	
                    case SDLK_a: 
						adown = true;
                    	break;
                    
                    case SDLK_d: 
						ddown = true;
                    	break;
				}
			}
			if( event.type == SDL_KEYUP )
            {
				switch( event.key.keysym.sym )
                {
                    case SDLK_a: 
						adown = false;
                    	break;
                    
                    case SDLK_d: 
						ddown = false;
                    	break;
                    	
                    case SDLK_SPACE:
						if (start == false){
							//Play the music
						    if( Mix_PlayMusic( music, -1 ) == -1 )
						    {
						        return 1;
						    } 
						    start = true;
						}
						
					    
				}
			}
			if( event.type == SDL_MOUSEBUTTONDOWN )
		    {
		        //If the left mouse button was pressed
		        if( event.button.button == SDL_BUTTON_LEFT )
		        {
		            //Get the mouse offsets
		            mousex = event.button.x;
		            mousey = event.button.y;
		        	firing = true;
		            
		        }
		    }
		}

		if (firing == true)
		{
			if (fired == false){
				//Play the high hit effect
                if( Mix_PlayChannel( -1, cannon, 0 ) == -1 )
                {
                    return 1;    
                }
				for (i=0; i<5; i++){
					if (boat[i].alive == true){
					
						if (i<2){
							if ((mousex >= boat[i].x + 40) && (mousex <= boat[i].x + 150) && (mousey >= int(145-wavebydif) + 200) && (mousey <= int(145-wavebydif) + 300))
								boat[i].alive = false;
								boat[i].debrisx = (boat[i].x);
								if (i <= 1) boat[i].debrisy = int(145-wavebydif);
								if (i > 1) boat[i].debrisy = int(200-wavebydif);
								debris = true;
								firing = false;
								boat[i].dtimer = 0;
								
								
						}
						if (i>1){
							if ((mousex >= boat[i].x + 40) && (mousex <= boat[i].x + 150) && (mousey >= int(200-wavebydif) + 200))
								boat[i].alive = false;
								boat[i].debrisx = (boat[i].x);
								if (i <= 1) boat[i].debrisy = int(145-wavebydif);
								if (i > 1) boat[i].debrisy = int(200-wavebydif);
								debris = true;
								firing = false;
								boat[i].dtimer = 0;
							
						}
					}	
				}
				fired = true;	
			}
			
			
		}
		
		//clouds
		cloudx += .5;
		if (cloudx >= SCREEN_WIDTH) cloudx = -100;
		
		//wavec
		if (wavecback == false)
		{
			wavecxdif = ticker;
			if (ticker < 10) wavecydif = ticker;	
			if (ticker > 9) wavecydif = 20-ticker;
		}
		if (wavecback == true)
		{
			wavecxdif = 20-ticker;
			if (ticker < 10) wavecydif = ticker;	
			if (ticker > 9) wavecydif = 20-ticker;
		}
		wavecydif = wavecydif/3;
		wavecxdif = wavecxdif/3;
		//waveb
		if (wavebback == false)
		{
			wavebxdif = ticker;
			if (ticker < 10) wavebydif = ticker;	
			if (ticker > 9) wavebydif = 20-ticker;
		}
		if (wavebback == true)
		{
			wavebxdif = 20-ticker;
			if (ticker < 10) wavebydif = ticker;	
			if (ticker > 9) wavebydif = 20-ticker;
		}
		wavecydif = wavebydif/2;
		wavecxdif = wavebxdif/2;
		//wavea
		if (waveaback == false)
		{
			waveaxdif = ticker;
			if (ticker < 10) waveaydif = ticker;	
			if (ticker > 9) waveaydif = 20-ticker;
		}
		if (waveaback == true)
		{
			waveaxdif = 20-ticker;
			if (ticker < 10) waveaydif = ticker;	
			if (ticker > 9) waveaydif = 20-ticker;
		}
		waveaydif = waveaydif;
		waveaxdif = waveaxdif;
		
		//camera
		if (ticker>lastticker)
		{
			if (adown == true)
				speed -= 2;
			if (ddown == true)
				speed += 2;
			if (speed > 0) speed -= 1;
			if (speed < 0) speed += 1;
			if (speed < -10) speed = -10;
			if (speed > 10) speed = 10;
		}
		x += speed;
		if (x < -4780)
		{
			x = -4780;
			speed = 0;
		}
		if (x > 5900)
		{
			x = 5900;
			speed = 0;
		}
		
		if (150 < abs(boat[0].x - boat[1].x)){
			boat[0].x -= 2 + int(speed/2);
			boat[1].x -= 2 + int(speed/2);
		}
		else boat[1].x -= 3 + int(speed/2);
		if ((150 < abs(boat[2].x - boat[3].x)) && (150 < abs(boat[3].x - boat[4].x)) && (150 < abs(boat[2].x - boat[4].x))){
			boat[2].x += 2 - int(speed/2);
			boat[3].x += 2 - int(speed/2);
			boat[4].x += 2 - int(speed/2);
		}
		else {
			boat[3].x += 3 - int(speed/2);
			boat[4].x += 4 - int(speed/2);
		}
		if (boat[0].x < -200) boat[0].x = 640;
		if (boat[1].x < -200) boat[1].x = 640;
		if (boat[2].x > 640) boat[2].x = -200;
		if (boat[3].x > 640) boat[3].x = -200;
		if (boat[4].x > 640) boat[4].x = -200;
		if (boat[2].x < -200) boat[2].x = 640;
		if (boat[3].x < -200) boat[3].x = 640;
		if (boat[4].x < -200) boat[4].x = 640;
		if (boat[1].x > 640) boat[1].x = -200;
		if (boat[0].x > 640) boat[0].x = -200;
		
		boat[0].debrisx -= int(speed/2);
		boat[1].debrisx -= int(speed/2);
		boat[2].debrisx -= int(speed/2);
		boat[3].debrisx -= int(speed/2);
		boat[4].debrisx -= int(speed/2);
		
		
		//bg
		apply_surface(0, 0, bg, screen);
		apply_surface(int(cloudx), int(cloudy), cloud, screen);
		//wavec
		apply_surface(int(wavecx+wavecxdif)-int(x/3)-2000, int(wavecy+wavecydif), wavec, screen);
		apply_surface(int(wavecx+wavecxdif)-int(x/3), int(wavecy+wavecydif), wavec, screen);
		apply_surface(int(wavecx+wavecxdif)-int(x/3)+2000, int(wavecy+wavecydif), wavec, screen);
		//boatsb
		if (boat[0].alive == true) apply_surface(boat[0].x, int(145-wavebydif), usb, screen);
		if (boat[1].alive == true) apply_surface(boat[1].x, int(145-wavebydif), usb, screen);
		if (boat[0].alive == false) apply_surface(boat[0].debrisx, boat[0].debrisy, wrecka, screen);
		if (boat[1].alive == false) apply_surface(boat[1].debrisx, boat[1].debrisy, wrecka, screen);
		//king
		apply_surface(int(220+wavebxdif), int(145-wavebydif), king, screen);
		//waveb
		apply_surface(int(wavebx+wavebxdif)-int(x/2)-5200, int(waveby+wavebydif), waveb, screen);
		apply_surface(int(wavebx+wavebxdif)-int(x/2)-3900, int(waveby+wavebydif), waveb, screen);
		apply_surface(int(wavebx+wavebxdif)-int(x/2)-2600, int(waveby+wavebydif), waveb, screen);
		apply_surface(int(wavebx+wavebxdif)-int(x/2)-1300, int(waveby+wavebydif), waveb, screen);
		apply_surface(int(wavebx+wavebxdif)-int(x/2), int(waveby+wavebydif), waveb, screen);
		apply_surface(int(wavebx+wavebxdif)-int(x/2)+1300, int(waveby+wavebydif), waveb, screen);
		apply_surface(int(wavebx+wavebxdif)-int(x/2)+2600, int(waveby+wavebydif), waveb, screen);
		apply_surface(int(wavebx+wavebxdif)-int(x/2)+3900, int(waveby+wavebydif), waveb, screen);
		apply_surface(int(wavebx+wavebxdif)-int(x/2)+5200, int(waveby+wavebydif), waveb, screen);
		//boatsa
		if (boat[2].alive == true) apply_surface(boat[2].x, int(200-wavebydif), usb, screen);
		if (boat[3].alive == true) apply_surface(boat[3].x, int(200-wavebydif), usb, screen);
		if (boat[4].alive == true) apply_surface(boat[4].x, int(200-wavebydif), usb, screen);
		if (boat[2].alive == false) apply_surface(boat[2].debrisx, boat[2].debrisy, wrecka, screen);
		if (boat[3].alive == false) apply_surface(boat[3].debrisx, boat[3].debrisy, wrecka, screen);
		if (boat[4].alive == false) apply_surface(boat[4].debrisx, boat[4].debrisy, wrecka, screen);
		//wavea
		apply_surface(int(waveax+waveaxdif)-int(x)-6800, int(waveay+waveaydif), wavea, screen);
		apply_surface(int(waveax+waveaxdif)-int(x)-5100, int(waveay+waveaydif), wavea, screen);
		apply_surface(int(waveax+waveaxdif)-int(x)-3400, int(waveay+waveaydif), wavea, screen);
		apply_surface(int(waveax+waveaxdif)-int(x)-1700, int(waveay+waveaydif), wavea, screen);
		apply_surface(int(waveax+waveaxdif)-int(x), int(waveay+waveaydif), wavea, screen);
		apply_surface(int(waveax+waveaxdif)-int(x)+1700, int(waveay+waveaydif), wavea, screen);
		apply_surface(int(waveax+waveaxdif)-int(x)+3400, int(waveay+waveaydif), wavea, screen);
		apply_surface(int(waveax+waveaxdif)-int(x)+5100, int(waveay+waveaydif), wavea, screen);
		apply_surface(int(waveax+waveaxdif)-int(x)+6800, int(waveay+waveaydif), wavea, screen);
		//boat counter
		if (count >= 1) apply_surface(-54-10, -200, usbs, screen);
		if (count >= 2) apply_surface(-22-10, -200, usbs, screen);
		if (count >= 3) apply_surface(14-10, -200, usbs, screen);
		if (count >= 4) apply_surface(46-10, -200, usbs, screen);
		if (count >= 5) apply_surface(78-10, -200, usbs, screen);
		if (count >= 6) apply_surface(110-10, -200, usbs, screen);
		if (count >= 7) apply_surface(142-10, -200, usbs, screen);
		if (count >= 8) apply_surface(174-10, -200, usbs, screen);
		if (count >= 9) apply_surface(208-10, -200, usbs, screen);
		if (count >= 10) apply_surface(240-10, -200, usbs, screen);
		if (count >= 11) apply_surface(272-10, -200, usbs, screen);
		if (count >= 12) apply_surface(304-10, -200, usbs, screen);
		if (count >= 13) apply_surface(336-10, -200, usbs, screen);
		if (count >= 14) apply_surface(368-10, -200, usbs, screen);
		if (count >= 15) apply_surface(400-10, -200, usbs, screen);
		if (count >= 16) apply_surface(432-10, -200, usbs, screen);
		if (count >= 17) apply_surface(464-10, -200, usbs, screen);
		if (count >= 18) apply_surface(496-10, -200, usbs, screen);
		if (count >= 19) apply_surface(528-10, -200, usbs, screen);
		if (count >= 20) apply_surface(560-10, -200, usbs, screen);
		
		if (count >= 20) end = true;
		
		if (end == true){
			switch (endframe){
				case 0:
					apply_surface(0, 0, jeff5, screen);
					break;
				case 1:
					apply_surface(0, 0, jeff4, screen);
					break;
				case 2:
					apply_surface(0, 0, jeff3, screen);
					break;
				case 3:
					apply_surface(0, 0, jeff2, screen);
					break;
				case 4:
					apply_surface(0, 0, jeff1, screen);
					break;
				case 5:
					apply_surface(0, 0, jeff, screen);
					break;
			}
		}
		
		if (start == false) apply_surface(0, 0, title, screen);
		
		
		SDL_Flip( screen );
	}
	
	clean_up();
	return 0;
	
}
