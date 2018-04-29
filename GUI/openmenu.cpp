//Let's assume that your method of input is a mouse
//since this is long, you can create a class for this then recall it in the gameloop


//WE CREATE THE COORDINATES OF THE MOUSE
int Mx = 0;
int My = 0;
bool play1 = false;

//Let's make the menu's background
SDL_Texture *background = IMG_LoadTexture(renderer, "FilePath");
//Now the Background's rect, it should give us power to control it's size and location
SDL_Rect background_rect;
background_rect x = 0;   //X COORDINATE
background_rect y = 0;   //Y COORDINATE
background_rect w = 400; //WIDTH
background_rect h = 400; //HEIGHT
/*Now, our background should start from the top left since it's coordinates are in 
(0, 0) and it should stretch 400 to the right and 400 down making it a square, 400X400*/

//The New Game button
SDL_Texture *newGame = IMG_LoadTexture(renderer, "FilePath");
//The game button's rect
SDL_Rect newGame_Rect;
newGame_Rect.x = 0;
newGame_Rect.y = 100;
newGame_Rect.w = 50;
newGame_Rect.h = 25;

//I'll just skip to the exit button
SDL_Texture *exit = IMG_LoadTexture(renderer, "FilePath");
//The exit button's rect
SDL_Rect exit_Rect;
exit_Rect.x = 0;
exit_Rect.y = 200;
exit_Rect.w = 50;
exit_Rect.h = 25;

IN THE GAME LOOP

SDL_GetMouseState(&Mx, &My); //This would constantly input the the x coordinate into Mx and the y coordinate of the mouse into My


//WARNING: THIS AREA COULD BE A BIT TRICKY IF YOU DON'T UNDERSTAND COLLISION

//Let's make an if statement that checks if the mouse is touching the New Game Button

if (Mx >= newGame_Rect.x && Mx <= newGame_Rect.x + newGame_Rect.w && My >= newGame_Rect.y && My <= newGame_Rect.y + newGame_Rect.h)

/*The if statement basically checks if the pointer is inside the area of the rect, 
it may look confusing but it's actually pretty simple, since (0, 0) is at the 
top-left and the going down is +y then there's no negative coordinate, that also goes
for the texture, the top left point is the origin, so basically, it checks if
the mouse's x-coordinate is greater than the origin but less than the origin plus
width (newGame_Rect.x + newGame_Rect.w to make sure that the pointer is inside, the
same goes for the y coordinate*/ 
{
SDL_SetTextureColorMod(newGame, 250, 0, 0 ); 
    /*This would make the texture change color if the mouse is inside the rect,
    it takes the name of the texture which is newGame, and the following variables
    are the r, g, and b, if you were to make all except red into 0 then the texture
    would be tinted red, I assumed you wanted the player to know that it is hovering
    over the new game so I added this as a bonus*/

if (event->type == SDL_MOUSEBUTTONDOWN)  //this calls an event, I assume that you already know how to make an event right? 
{
    if (event->button.button == SDL_BUTTON_LEFT)
    {    //if it is pressed then play1 becomes true which you could use to initiate the newgame
        play1 = true;
    }
}
}
else 
{
SDL_SetTextureColorMod(Gettexture(), 250, 250, 250); 
    // Tints the texture white if the cursor isn't over the button
}