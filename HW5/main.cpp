///////Name: Annice Najafi
///////Date: 13 March 2019
///////Homework 5


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>
#include <string>


////////Citations
////////Sound files :
////////http://www.moviesoundclips.net/sound.php?id=105
////////images- enemy:
////////https://www.habbowidgets.com/habinfo/hhus-1960ff189c3515aaa7738c914be0a218
////////images- instructions page
////////https://www.infoworld.com/article/3054071/jigsaw-crypto-ransomware-deletes-more-files-the-longer-you-delay-paying.html
////////Font:
////////https://www.1001freefonts.com/bfonts.php

using namespace sf;


//////Construct a matrix grid with the length of 25 squares and width of 40 squares
const int M = 25;
const int N = 40;
//////set the value of all squares in the grid equal to 0.
int grid[M][N] = {0};
/////Set the size of the tile (each square of the grid total of 1000) equal to 18
int ts = 18; //tile size

class Enemy
{

public:

      ////x,y determine the first location of the enemy.
      ////Setters and getter function for the four values x, y, dx, dy
      ////class enemy function prototypes
      void setX(int x);
      ////function name:setX
      ////function: void - sets the value of the private variable x equal to the input in the main function
      ////function input - int
      ////function output --> no output
      int getX();
      ////function name:getX
      ////function: int - returns the value of the private variable x
      ////function input - int no input
      ////function output: int, the private variable x
      void setY(int y);
      ////function name:setY
      ////function: void - sets the value of the private variable y equal to the input in the main function
      ////function input - int
      ////function output --> no output
      int getY();
      ////function name:getY
      ////function: int - returns the value of the private variable Y
      ////function input - int no input
      ////function output: int, the private variable Y
      void setdx();
      ////function name:setdx
      ////function: void - sets the value of the private variable dx equal to the input in the main function
      ////function input - int
      ////function output --> no output
      int getdx();
      ////function name:getdx
      ////function: int - returns the value of the private variable dx
      ////function input - int no input
      ////function output: int, the private variable dx
      void setdy();
      ////function name:setdy
      ////function: void - sets the value of the private variable dy equal to the input in the main function
      ////function input - int
      ////function output --> no output
      int getdy();
      ////function name:getdy
      ////function: int - returns the value of the private variable dy
      ////function input - int no input
      ////function output: int, the private variable dy
      void move();
      ////function name: move
      ////function: void - moves the enemy around the grid
      ////function input: uses the private variables
      ////function output --> no output void function

private:
    int x,y,dx,dy;
};
/////class function definitions
    void Enemy::setX(int numx){
    x=numx;
    }
    void Enemy::setY(int numy){
    y=numy;
    }
    void Enemy::setdx(){
    dx=4-rand()%8;
    }
    void Enemy::setdy(){
    dy=4-rand()%8;
    }
    int Enemy::getX(){
    return x;
    }
    int Enemy::getY(){
    return y;
    }
    void Enemy::move()
    {
    /////when function is called the location of the enemies changes randomly(dx and dy added to x and y, dx and dy are random numbers)
    /////if they hit the rim of the grid they turn back, because the value of the rim is 1.
        x=x+dx;
        y=y+dy;

        if (grid[y/ts][x/ts]==1) {
        dx=-dx;
        x+=dx;
        }

        if (grid[y/ts][x/ts]==1) {
        dy=-dy;
        y+=dy;
        }
    }
    ////function name: drop
    ////function:
    ////fills in the area when you successfully make a rectangle inside the grid
    ////input: grid square
    ////output - no output void
    void drop(int y,int x)
    {
        if (grid[y][x]==0) {

            grid[y][x]=-1;
         }
        if (grid[y-1][x]==0) {

             drop(y-1,x);
         }
        if (grid[y+1][x]==0) {

            drop(y+1,x);
        }
        if (grid[y][x-1]==0) {

             drop(y, x - 1);
        }
        if (grid[y][x+1]==0) {

             drop(y,x+1);
        }
    }

int main() {
    int score =0;
    srand(time(0));
    int diff;
    int num;
    //////I have added a page that gives the player the option to choose the level of difficulty

    RenderWindow window0(VideoMode(N * ts, M * ts), "Instructions");
    //////Font added to the game
    Font f;
    f.loadFromFile("Font/bakery.ttf"); // font is a sf::Font

    Text text;
    text.setFont(f);
    text.setString("I want to play a game. Select the level of difficulty");

    // set the character size
    text.setCharacterSize(30); // in pixels, not points!

    text.setFillColor(sf::Color::Red);
    ////////
    ////////The player can choose the level of difficulty by clicking the arrow keys or enter just close the page
    Text inst1;
    inst1.setFont(f);
    inst1.setString("Press the 'up' arrow for HARD");
    text.setCharacterSize(30);
    inst1.setFillColor(sf::Color::White);
    inst1.move(20,200);
    ////////
    Text inst2;
    inst2.setFont(f);
    inst2.setString("Press the 'down' arrow for EASY");
    text.setCharacterSize(30);
    inst2.setFillColor(sf::Color::White);
    inst2.move(20,100);
    ////////
    Text instn;
    instn.setFont(f);
    instn.setString("Press 'Enter' for Normal or just close the window");
    text.setCharacterSize(30);
    instn.setFillColor(sf::Color::White);
    instn.move(20,300);
    ////////
    ////////Player will be greeted with jigsaw saying "I want to play a game"
    Texture jigsaw;
    jigsaw.loadFromFile("images/jigsaw.jpg");
    Sprite sjig(jigsaw);
    sjig.setPosition(-10,50);
    window0.draw(sjig);
    Music game;
    game.openFromFile("sounds/game.ogg");
    game.play();



    window0.draw(text);
    window0.draw(inst1);
    window0.draw(inst2);
    window0.draw(instn);
    window0.display();


    if (Keyboard::isKeyPressed(Keyboard::Enter)) {
        window0.close();

    }

    Music m_game;
    m_game.openFromFile("sounds/gameover2.ogg");
    ///render the n by m matrix with the tile size of ts and name the window "Xonix Game!"
    /////
    while (window0.isOpen())
    {

        Event e0;
        while (window0.pollEvent(e0))
        {
            ///close the window
            if (e0.type == Event::Closed) {
                window0.close();
            }

            if (e0.type == Event::KeyPressed) {
                ////if escape key is pressed the players block moves to the top of the rim/ reset the game
                if (e0.key.code == Keyboard::Escape) {
                    window0.close();
                      }

            }
        }

        ///move the square around by pressing the arrow keys on the keyboard
        if (Keyboard::isKeyPressed(Keyboard::Enter)) {
            diff=50;
            num=5;
           window0.close();
        }
        if(Keyboard::isKeyPressed(Keyboard::Up)){
            diff=70;
            num=6;
            window0.close();
        }
        if(Keyboard::isKeyPressed(Keyboard::Down)){
            diff=30;
            num=4;
            window0.close();
        }
        else{
            diff=50;
        }
    }

    ////Once the instructions page is closed the game starts
    ////
	RenderWindow window(VideoMode(N*ts, M*ts), "Xonix Game - saw theme!");
	///determine the frame rate which can basically change the speed of the spinners
	window.setFramerateLimit(diff);



    ///load the images and if not loaded prints and error message
	Texture t1,t2,t3;
	t1.loadFromFile("images/tiles.png");
	if(!t1.loadFromFile("images/tiles.png")){
	    std::cout<<"image file could not be loaded"<<std::endl;
	    return -1;
	}

    t2.loadFromFile("images/gameover.png");
	if(!t2.loadFromFile("images/gameover.png")){
	    std::cout<<"image file could not be loaded"<<std::endl;
	    return -1;
	}
    t3.loadFromFile("images/jigmask.gif");
	if(!t3.loadFromFile("images/jigmask.gif")){
	    std::cout<<"image file could not be loaded"<<std::endl;
	    return -1;
	}

	Sprite sTile(t1), sGameover(t2), sEnemy(t3);

	sGameover.setPosition(100,100);
	////set the origin of the enemies their rotation is performed around the origin
	sEnemy.setOrigin(20,20);
    ///determine the number of enemies and implement the class to use the move function to move them around
	int enemyCount = num;
    Enemy a[num];
    ////set the initial location of the enemies
    for (int i=0;i<6;i++){
        a[i].setX(300);
        a[i].setY(300);
        a[i].setdx();
        a[i].setdy();
    }
    ///set the value of the squares in the grid equal to 0
	bool Game=true;
	int x=0, y=0, dx=0, dy=0;
    float timer=0, delay=0.07; 
    Clock clock;
    ///set the value of the squares in the rim of the grid equal to 1
	for (int i=0;i<M;i++) {
        for (int j = 0; j < N; j++) {
            if (i == 0 || j == 0 || i == M - 1 || j == N - 1) {

                grid[i][j] = 1;
            }
        }
    }
    while (window.isOpen())
    {
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer+=time;
        Event e;
        while (window.pollEvent(e))
        {
            ///close the window
            if (e.type == Event::Closed) {
                window.close();
            }

			if (e.type == Event::KeyPressed) {
			    ////if escape key is pressed the players block moves to the top of the rim/ reset the game
                if (e.key.code == Keyboard::Escape) {
                    for (int i = 1; i < M - 1; i++) {
                        for (int j = 1; j < N - 1; j++) {
                            grid[i][j] = 0;
                        }
                    }
                    x = 0;
                    y = 0;
                    Game = true;
                }
            }
		}
        ///move the square around by pressing the arrow keys on the keyboard
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
		    dx=-1;dy=0;
		}
	    if (Keyboard::isKeyPressed(Keyboard::Right))  {
	        dx=1;dy=0;
	    }
	    if (Keyboard::isKeyPressed(Keyboard::Up)) {
	        dx=0;dy=-1;
	    }
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
		    dx=0;dy=1;
		}
        
		if (!Game) {
		    continue;
		}

		if (timer>delay)
		{
         x+=dx;
         y+=dy;
         ///if you press the up arrow when at the upper rim or press the down arrow when at the lower rim the red square does not move.
		 if (x<0) {
             x = 0;
         }

		 if (x>N-1){
		     x=N-1;
		 }

         if (y<0) {
             y = 0;
         }

         if (y>M-1) {
             y=M-1;
         }
        ///if the value of one of the squares in the grid gets equal to 2 then the game fails
		 if (grid[y][x]==2){
		     Game=false;
		     m_game.play();
		 }
         if (grid[y][x]==0) {
             grid[y][x]=2;
         }
         timer=0;
		}

		for (int i=0;i<enemyCount;i++) {
		    a[i].move();
		}

		if (grid[y][x]==1)
          {
           dx=dy=0;
        //////fill in the other squares of the grid(fill in the inside of the rectangle) when you make a full rectangle
           for (int i=0;i<enemyCount;i++) {
               drop(a[i].getY() / ts, a[i].getX() / ts);
           }

           for (int i=0;i<M;i++) {
               for (int j = 0; j < N; j++) {
                   if (grid[i][j] == -1) {
                       grid[i][j] = 0;
                   } else {
                       grid[i][j] = 1;
                   }
               }
           }
          }

    //////fail if you touch the enemy

        for (int i=0;i<enemyCount;i++){
           if  (grid[a[i].getY()/ts][a[i].getX()/ts]==2){
               Game=false;
               m_game.play();
           }
        }

      /////////draw//////////
      window.clear();


	  for (int i=0;i<M;i++)
		for (int j=0;j<N;j++)
		 {
		    ///if the value of the square is equal to 0 nothing happens
            if (grid[i][j]==0) {
                continue;
            }
            ///if the value of the square is equal to 1 the square in the grid becomes red
            if (grid[i][j]==1) {
                sTile.setTextureRect(IntRect( 36,0,ts,ts));
            }
            ///if the value of the square in the grid becomes 2 it turns orange *18*6=108 --> seventh square in the image
            if (grid[i][j]==2) {
                sTile.setTextureRect(IntRect(0,0,ts,ts));
            }
            ///draw the squares
			sTile.setPosition(j*ts,i*ts);
			window.draw(sTile);
		 }
        ////otherwise the square will be purple
      sTile.setTextureRect(IntRect(18,0,ts,ts));
	  sTile.setPosition(x*ts,y*ts);
	  window.draw(sTile);

	  sEnemy.rotate(10);
      for (int i=0;i<enemyCount;i++)
       {
	    sEnemy.setPosition(a[i].getX(),a[i].getY());
	    window.draw(sEnemy);
       }
      ////////////////////////////////Print the score to the page
        Text scores;
        scores.setFont(f);

        scores.setString("score = " + std::to_string(score));
        scores.setCharacterSize(50);
        scores.setFillColor(sf::Color::White);
        scores.setPosition(10,0);
        window.draw(scores);


        for(int i=0; i<M; i++){
            for(int j=0; j<N; j++){
                if(grid[i][j] ==2 ){
                    score++;


                }
            }
        }


      if (!Game) {
          window.draw(sGameover);
      }

 	  window.display();
	}



    return 0;
}

