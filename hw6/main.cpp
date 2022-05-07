#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <list>
#include <cmath>

///This game includes a page for choosing difficulty level before the game starts based on that
///the number of asteroids in the game change
///The UFO used in the game is an animation
///The score is shown in the game and if the score gets equal to zero or becomes negative the gameover page will be shown
using namespace sf;
////Sources:
////////Sound files :
////////http://www.moviesoundclips.net/sound.php?id=105
////////https://freesound.org
////////UFO icon:
////////https://opengameart.org/content/animated-spaceships
////////Background Pictures
////////http://www.colonialkc.org/astronomy-background-compressor
////////https://www.pixilart.com/art/pixel-galaxy-40b33079899fe98
////////Font:
////////https://www.1001freefonts.com/badaboom.font


/////Length and width of the screen

const int W = 1200;
const int H = 800;

////convert degrees to radians
float DEGTORAD = 0.017453f;
////This class contains the animation constructor which includes the position, texture the size and number of frames
////For example for the rock picture there are 16 pictures of rocks and they keep changing so count keeps the number of frames
class Animation
{
public:
	float Frame, speed;
	Sprite sprite;
    std::vector<IntRect> frames;

	Animation(){}

    Animation (Texture &t, int x, int y, int w, int h, int count, float Speed)
	{
	    Frame = 0;
        speed = Speed;

		for (int i=0;i<count;i++) {
            frames.push_back(IntRect(x + i * w, y, w, h));
        }
		sprite.setTexture(t);
		sprite.setOrigin(w/2,h/2);
        sprite.setTextureRect(frames[0]);
	}


	void update()
	{
    	Frame += speed;
		int n = frames.size();
		if (Frame >= n) {
		    Frame -= n;
		}
		if (n>0) {
		    sprite.setTextureRect( frames[int(Frame)] );
		}
	}

	bool isEnd()
	{
	  return Frame+speed>=frames.size();
	}

};

////Anything that moves and has a life like the player, bullet, asteroid and UFO are entities -- Entity is the base class
class Entity
{
public:
float x,y,dx,dy,R,angle;
bool life;
std::string name;
Animation anim;
///class constructor
Entity()
{
  life=1;
}
///set the settings of the entity
void settings(Animation &a,int X,int Y,float Angle=0,int radius=1)
{
  anim = a;
  x=X;
  y=Y;
  angle = Angle;
  R = radius;
}
////virtual function update that updates the location of the entity
virtual void update(){};
////a function for drawing the entity
void draw(RenderWindow &app)
{
  anim.sprite.setPosition(x,y);
  anim.sprite.setRotation(angle+90);
  app.draw(anim.sprite);

  CircleShape circle(R);
  circle.setFillColor(Color(255,0,0,170));
  circle.setPosition(x,y);
  circle.setOrigin(R,R);
  //app.draw(circle);
}
////virtual destructor
virtual ~Entity(){};
};

////asteroid a derived class of entity
class asteroid: public Entity
{
public:
    ////class constructor
  asteroid()
  {
      count++;
      ////give random positions to the asteroids
    dx=rand()%8-4;
    dy=rand()%8-4;
    name="asteroid";
  }
  ////class destructor
  ~asteroid(){
      count--;
  }
static int getCount(){
        return count;
    }
      ////define a function to update the location of the asteroids
void  update()
  {
   x+=dx;
   y+=dy;

   if (x>W) {
       x = 0;
   }
   if (x<0){
       x=W;
   }
   if (y>H) {
       y=0;
   }
   if (y<0) {
       y=H;
   }
  }
  ////the number of asteroid is constant over the game
    private:

    static int count;

};
////defined count as a global variable
int asteroid::count=0;


////derived class bullet
class bullet: public Entity
{
public:
    ////class constructor
  bullet()
  {
      name="bullet";
  }
////define a function to update the location of the bullet
void  update()
  {
      ////6 is the speed of the bullet.
   dx=cos(angle*DEGTORAD)*6;
   dy=sin(angle*DEGTORAD)*6;
  // angle+=rand()%6-3;
   x+=dx;
   y+=dy;
////bullet exits the screen, it disappears.
   if (x>W || x<0 || y>H || y<0) {
       life=0;
   }
  }

};
////derived class, UFO
class UFO: public Entity
{
public:
    ///class constructor
    UFO()
    {
        dx=rand()%8-4;
        dy=rand()%8-4;
        name="UFO";
        u_count++;
    }
    ~UFO()
    {
        u_count--;
    }
    ////function to update the location of the object
    void update(){
        x+=dx;


        if (x>W) {
            x = 0;
        }
        if (x<0){
            x=W;
        }
        if (y>H) {
            y=0;
        }
        if (y<0) {
            y=H;
        }


    }
    static int getuCount(){
        return u_count;
    }
    private:
    static int u_count;
};
int UFO::u_count=0;

/////derived class, player
class player: public Entity
{
public:
   bool thrust;
///class constructor
   player()
   {
     name="player";
   }
///function defined to update the location of player
   void update()
   {
     if (thrust){
            dx+=cos(angle*DEGTORAD)*0.2;
            dy+=sin(angle*DEGTORAD)*0.2;
      }
     else
      {
         dx*=0.99;
         dy*=0.99;
      }
    ////It increases until reaches the maximum speed of 15
    int maxSpeed=15;
    float speed = sqrt(dx*dx+dy*dy);
    if (speed>maxSpeed)
     {
        dx *= maxSpeed/speed;
        dy *= maxSpeed/speed;
     }

    x+=dx;
    y+=dy;

    if (x>W){
        x=0;
    }
    if (x<0) {
        x=W;
    }
    if (y>H) {
        y=0;
    }
    if (y<0) {
        y=H;
    }
   }

};

////function for when two entities collide on screen
bool isCollide(Entity *a,Entity *b)
{
  return (b->x - a->x)*(b->x - a->x)+
         (b->y - a->y)*(b->y - a->y)<
         (a->R + b->R)*(a->R + b->R);
}


int main() {
    srand(time(0));
    int score = 0;
    int num = 60;
    int keep=0;
    bool Game;
////// choosing Level window
    RenderWindow window0(VideoMode(W, H), "Instructions");
    Font f;
    f.loadFromFile("Font/badaboom/BADABB__.TTF"); // font is a sf::Font
    Texture t0;
    t0.loadFromFile("images/Sky.gif");
    Sprite background0(t0);
    ////Instructions
    Text inst;
    inst.setFont(f);
    inst.setString("Welcome, please select the level of difficulty");
    inst.setCharacterSize(60);
    inst.setFillColor(sf::Color::Yellow);
    inst.move(100, 30);
    /////// Hard Button
    sf::RectangleShape rectangle(sf::Vector2f(200, 50));
    rectangle.setOutlineThickness(5);
    rectangle.setFillColor(sf::Color(103, 144, 206));
    rectangle.setPosition(500, 500);
    rectangle.getGlobalBounds();
    Text btn1;
    btn1.setFont(f);
    btn1.setString("Hard");
    btn1.setCharacterSize(40);
    btn1.setFillColor(sf::Color::White);
    btn1.move(550, 500);
    /////// Normal Button
    sf::RectangleShape rectangle2(sf::Vector2f(200, 50));
    rectangle2.setOutlineThickness(5);
    rectangle2.setFillColor(sf::Color(103, 144, 206));
    rectangle2.setPosition(500, 400);
    rectangle2.getGlobalBounds();
    Text btn2;
    btn2.setFont(f);
    btn2.setString("Easy");
    btn2.setCharacterSize(40);
    btn2.setFillColor(sf::Color::White);
    btn2.move(550, 400);
    ////// Easy button
    sf::RectangleShape rectangle3(sf::Vector2f(200, 50));
    rectangle3.setOutlineThickness(5);
    rectangle3.setFillColor(sf::Color(103, 144, 206));
    rectangle3.setPosition(500, 300);
    rectangle3.getGlobalBounds();
    Text btn3;
    btn3.setFont(f);
    btn3.setString("Normal");
    btn3.setCharacterSize(40);
    btn3.setFillColor(sf::Color::White);
    btn3.move(550, 300);
    /////Draw the instruction and buttons on the first page
    window0.draw(background0);
    window0.draw(rectangle);
    window0.draw(rectangle2);
    window0.draw(rectangle3);
    window0.draw(inst);
    window0.draw(btn1);
    window0.draw(btn2);
    window0.draw(btn3);
    window0.display();
    // change the size to 100x100
    ///if any of these buttons is pressed modify the game
    while (window0.isOpen()) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

            if (rectangle.getGlobalBounds().contains(window0.mapPixelToCoords(sf::Mouse::getPosition(window0)))) {
                window0.close();
                keep=25;
            }
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

            if (rectangle2.getGlobalBounds().contains(window0.mapPixelToCoords(sf::Mouse::getPosition(window0)))) {
                window0.close();
                keep=20;
            }
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

            if (rectangle3.getGlobalBounds().contains(window0.mapPixelToCoords(sf::Mouse::getPosition(window0)))) {
                window0.close();
                keep=15;
            }
        }


        Event e0;
        while (window0.pollEvent(e0)) {
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
    }
/////Render the game window

    RenderWindow app(VideoMode(W, H), "Asteroids!");
    app.setFramerateLimit(60);
/////Load images for entities
    Texture t1, t2, t3, t4, t5, t6, t7, t8, tg;
    t1.loadFromFile("images/spaceship.png");
    t2.loadFromFile("images/Backg.jpg");
    t3.loadFromFile("images/explosions/type_C.png");
    t4.loadFromFile("images/rock.png");
    t5.loadFromFile("images/fire_blue.png");
    t6.loadFromFile("images/rock_small.png");
    t7.loadFromFile("images/explosions/type_B.png");
    t8.loadFromFile("images/UFO.png");
    tg.loadFromFile("images/gameover.png");
    t1.setSmooth(true);
    t2.setSmooth(true);
////load images for background and gameover
    Sprite background(t2);
    Sprite sgameover(tg);
    sgameover.setPosition(500, 500);
    Animation sExplosion(t3, 0, 0, 256, 256, 48, 0.2);
    Animation sRock(t4, 0, 0, 64, 64, 5, 0.2);
    Animation sRock_small(t6, 0, 0, 64, 64, 16, 0.2);
    Animation sBullet(t5, 0, 0, 32, 64, 16, 0.8);
    Animation sPlayer(t1, 40, 0, 40, 40, 1, 0);
    Animation sPlayer_go(t1, 40, 40, 40, 40, 1, 0);
    Animation sExplosion_ship(t7, 0, 0, 192, 192, 64, 0.5);
    Animation sUFO(t8, 0, 0, 32, 32, 4, 0.05);


/////music to play for UFO
    Music game, explosion;
    game.openFromFile("Sounds/UFO.ogg");
    explosion.openFromFile("Sounds/explosion-04.ogg");
    float timer = 0, delay = 0.2;
    Clock clock;
    float time = clock.getElapsedTime().asSeconds();
    timer += time;


    std::list<Entity *> entities;




//////Instantiate the player only once on the screen
    player *p = new player();
    p->settings(sPlayer,200,200,0,20);
    entities.push_back(p);

    /////main loop/////
    while (app.isOpen()){

 /////close the window if the close button is pressed
        Event event;
        while (app.pollEvent(event)) {
            if (event.type == Event::Closed)
                app.close();
    /////shoot bullets if space is pressed
            if (event.type == Event::KeyPressed)
                if (event.key.code == Keyboard::Space) {
                    bullet *b = new bullet();
                    b->settings(sBullet, p->x, p->y, p->angle, 10);
                    entities.push_back(b);
                }
        }
////When the right arrow key is pressed the spaceship rotates clockwise by 3 degrees
////When the left arrow key is pressed the spaceship rotates unclockwise by 3 degrees

        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            p->angle += 3;
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            p->angle -= 3;
        }
        ///When the up arrow key is pressed the spaceship accelerates in that direction
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            p->thrust = true;
        } else {
            p->thrust = false;
        }

        if(asteroid::getCount()==0){
            for (int i = 0; i < keep; i++) {

                asteroid *a = new asteroid();
                a->settings(sRock, rand() % W, rand() % H, rand() % 360, 25);
                entities.push_back(a);


            }
        }

        if(UFO::getuCount()==0){
            if (rand() % 400 == 0) {
                UFO *a = new UFO();
                a->settings(sUFO, 0, rand() % H, rand() % 360, 25);
                entities.push_back(a);
            }
        }
////if a bullet hits an asteroid, the asteroid explodes and the bullet disappears.
        for (auto a:entities) {
            for (auto b:entities) {
                if (a->name == "asteroid" && b->name == "bullet") {
                    if (isCollide(a, b)) {
                        a->life = false;
                        b->life = false;

////create a new entity called explosion
                        Entity *e = new Entity();
                        e->settings(sExplosion, a->x, a->y);
                        e->name = "explosion";

                        entities.push_back(e);



                        for (int i = 0; i < 2; i++) {
                            ////if a small asteroid
                            if (a->R == 15) {
                                continue;
                                score = score+1;
                            }
                            ///if a big asteroid, breaks the asteroid
                            Entity *e = new asteroid();
                            e->settings(sRock_small, a->x, a->y, rand() % 360, 15);
                            entities.push_back(e);

                            score = score +3;
                        }

                    }
                }
                ////If a bullet hits the UFO, the UFO explodes
                if (a->name == "UFO" && b->name == "bullet") {
                    if (isCollide(a, b)) {

                        a->life = false;
                        b->life = false;

                        Entity *e = new Entity();
                        e->settings(sExplosion, a->x, a->y);
                        e->name = "explosion";

                        entities.push_back(e);
                        score = score + 10;
                        game.stop();


                    }
                }
                if (a->name == "player" && b->name == "asteroid") {
                    if (isCollide(a, b)) {
                        b->life = false;

                        Entity *e = new Entity();
                        e->settings(sExplosion_ship, a->x, a->y);
                        e->name = "explosion";
                        entities.push_back(e);
                        score = score -50;


                        p->settings(sPlayer, W / 2, H / 2, 0, 20);
                        p->dx = 0;
                        p->dy = 0;
                    }
                }
                if (a->name == "player" && b->name == "UFO") {
                    if (isCollide(a, b)) {
                        b->life = false;

                        Entity *e = new Entity();
                        e->settings(sExplosion_ship, a->x, a->y);
                        e->name = "explosion";
                        entities.push_back(e);
                        game.stop();


                        p->settings(sPlayer, W / 2, H / 2, 0, 20);
                        p->dx = 0;
                        p->dy = 0;
                    }
                }
            }
        }




        if (p->thrust) {

            p->anim = sPlayer_go;
        }
        else {
            p->anim = sPlayer;
        }


        for (auto e:entities) {
            if (e->name == "explosion") {
//                explosion.play();
                if (e->anim.isEnd()) {
                    e->life = 0;
                }
            }
        }


        for (auto r:entities) {
            if (r->name == "UFO") {
                if (r->life == true) {

                    game.play();


                }


            }
        }


        ////Checks if the entity is alive if not it will remove the entity from the list of entities
        for (auto i = entities.begin(); i != entities.end();) {
            Entity *e = *i;

            e->update();
            e->anim.update();

            if (e->life == false) {
                i = entities.erase(i);
                delete e;
            } else {
                i++;
            }
        }



        Text inst1;
        inst1.setFont(f);
        inst1.setString("Score="+std::to_string(score));
        inst1.setCharacterSize(40);
        inst1.setFillColor(sf::Color::White);
        inst1.move(20,100);
        /////

        if(score<-150){

            app.close();



        }


        //////draw//////
        app.draw(background);
        app.draw(inst1);


            for (auto i:entities)
                i->draw(app);

            app.display();


    }

    RenderWindow window1(VideoMode(W, H), "Results!");

    window1.draw(sgameover);
    window1.draw(background0);

    Text inst2;
    inst2.setFont(f);
    inst2.setString("GAMEOVER.");
    inst2.setCharacterSize(100);
    inst2.setFillColor(sf::Color::White);
    inst2.move(200,100);
    window1.draw(inst2);
    Music end;
    end.openFromFile("Sounds/gameover2.ogg");
    end.play();

    window1.display();
    while (window1.isOpen()) {



        Event e0;
        while (window1.pollEvent(e0)) {
            ///close the window
            if (e0.type == Event::Closed) {
                window1.close();
            }

            if (e0.type == Event::KeyPressed) {
                ////if escape key is pressed the players block moves to the top of the rim/ reset the game
                if (e0.key.code == Keyboard::Escape) {
                    window1.close();
                }

            }
        }
    }

    return 0;
}
