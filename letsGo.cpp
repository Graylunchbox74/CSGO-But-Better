#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <queue>
#include <vector>
#include <climits>

//to compile run this command:  g++ letsGo.cpp -lncurses -lopencv_core -lopencv_highgui -lopencv_imgcodecs

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <ncurses.h>

using namespace std;

void stopGame();

vector<vector<int> > gameMap;
int widthsize = 100;
int heightsize = 100;
queue<int> inputQueue;



struct pos{
    int id;
    int x;
    int y;
    int dir;
};
struct carpet{
    int x;
    int y;
    int dir;
    vector<pos> prev;
};
struct Player{
    int id;
    int x;
    int y;
    int ammo;
    int grenades;
    int bombs;
    int kills;
};
struct grenade{
    int id;
    int x;
    int y;
    int dir;
    int timer;
    int counter;
};
struct explosion{
    int id;
    int life;
    int counter;
    pos topLeft;
    pos up;
    pos topright;
    pos right;
    pos bottomRight;
    pos down;
    pos bottomLeft;
    pos left;
};
struct path{
    int x1;
    int x2;
    int y1;
    int y2;
};



vector<explosion> explosions;
vector<pos> bullets;
vector<grenade> mapGrenades;
vector<carpet> carpetBombs;
Player player;
pos playerPrev;
int ammoCounter = 0;


void input(){
    int x;
    timeout(8);
    x = getch();
    inputQueue.push(x);
}
/*
    0 = ' ' nothing
    1 = '#' wall
    2 = '@' player
    3 = '|' up/down bullet
    4 = '-' left/right bullet
    5 = '*' grenade
    6 = '/' upper right and lower left explosion
    7 = '\' upper left and lower right explosion
    8 = '&' refills grenades and ammo
*/


void draw(){
        clear();
        for(int y = player.y - heightsize; y < player.y + heightsize; y++){
            if(y >= 0 && y < gameMap.size()){
            for(int x = player.x - widthsize; x < player.x + widthsize; x++){
                if(x >= 0 && x < gameMap[y].size()){
                if(gameMap[y][x] == 0){
                mvprintw(y,x," ");
                }
                else if(gameMap[y][x] == 1){
                attron(COLOR_PAIR(1));
                mvprintw(y,x," ");
                attroff(COLOR_PAIR(1));
                }
                else if(gameMap[y][x] == 8){
                    mvprintw(y,x,"&");
                }
                }
            }
            printw("\n");
        }
        }
        mvprintw(player.y,player.x,"@");

        //print out bullets
        attron(COLOR_PAIR(4));
        for(int i = 0; i < bullets.size(); i++){
            if((bullets[i].x > player.x - widthsize && bullets[i].x < player.x + widthsize) && (bullets[i].y > player.y - heightsize && bullets[i].y < player.y + heightsize))
            //up and down
            if(bullets[i].dir == 1 || bullets[i].dir == 2){
                mvprintw(bullets[i].y,bullets[i].x,"|");
            }
            //left and right
            else
                mvprintw(bullets[i].y,bullets[i].x,"-");
            if(bullets[i].x == player.x && bullets[i].y == player.y && bullets[i].id != player.id)
                stopGame();
        }


        //print out explosions
        int x,y;
        for(int i = 0; i < explosions.size(); i++){
            if((explosions[i].up.x < player.x + widthsize && explosions[i].up.x > player.x - widthsize && explosions[i].up.y > player.y -heightsize && explosions[i].up.y < player.y + heightsize)||(
                explosions[i].down.x < player.x + widthsize && explosions[i].down.x > player.x - widthsize && explosions[i].down.y > player.y -heightsize && explosions[i].down.y < player.y + heightsize)){
            x = explosions[i].topLeft.x;
            y = explosions[i].topLeft.y;
            if(player.x == x && player.y == y){
                if(player.id == explosions[i].id){
                    player.kills -= 1;
                    explosions[i].topLeft.x = -1;
                }
                else
                    stopGame();
//                stopGame();
            }

            if(x != -1 )
            mvprintw(y,x,"\\");

            x = explosions[i].bottomRight.x;
            y = explosions[i].bottomRight.y;
            if(player.x == x && player.y == y)
            if(player.id == explosions[i].id){
                player.kills -= 1;
                explosions[i].topLeft.x = -1;
            }
            else
                stopGame();            
           if(x != -1)
                mvprintw(y,x,"\\");

            x = explosions[i].up.x;
            y = explosions[i].up.y;
            if(player.x == x && player.y == y)
            if(player.id == explosions[i].id){
                player.kills -= 1;
                explosions[i].topLeft.x = -1;
            }
            else
                stopGame();
            if(x != -1)
                mvprintw(y,x,"|");

            x = explosions[i].down.x;
            y = explosions[i].down.y;
            if(player.x == x && player.y == y)
            if(player.id == explosions[i].id){
                player.kills -= 1;
                explosions[i].topLeft.x = -1;
            }
            else
                stopGame();
            if(x != -1)
                mvprintw(y,x,"|");

            x = explosions[i].left.x;
            y = explosions[i].left.y;
            if(player.x == x && player.y == y)
            if(player.id == explosions[i].id){
                player.kills -= 1;
                explosions[i].topLeft.x = -1;
            }
            else
                stopGame();
            if(x != -1)
                mvprintw(y,x,"-");

            x = explosions[i].right.x;
            y = explosions[i].right.y;
            if(player.x == x && player.y == y)
            if(player.id == explosions[i].id){
                player.kills -= 1;
                explosions[i].topLeft.x = -1;
            }
            else
                stopGame();
            if(x != -1)
                mvprintw(y,x,"-");

            x = explosions[i].topright.x;
            y = explosions[i].topright.y;
            if(player.x == x && player.y == y)
            if(player.id == explosions[i].id){
                player.kills -= 1;
                explosions[i].topLeft.x = -1;
            }
            else
                stopGame();
            if(x != -1)
                mvprintw(y,x,"/");
            x = explosions[i].bottomLeft.x;
            y = explosions[i].bottomLeft.y;
            if(player.x == x && player.y == y)
            if(player.id == explosions[i].id){
                player.kills -= 1;
                explosions[i].topLeft.x = -1;
            }
            else
                stopGame();
            if(x != -1)
                mvprintw(y,x,"/");
            }
        }

        //print out mapGrenades
        for(int i = 0; i < mapGrenades.size(); i++){
            if(mapGrenades[i].x < player.x + widthsize && mapGrenades[i].x > player.x - widthsize && mapGrenades[i].y > player.y - heightsize &&mapGrenades[i].y < player.y + heightsize){
                if(mapGrenades[i].x == player.x && mapGrenades[i].y == player.y)
                    stopGame();
                mvprintw(mapGrenades[i].y,mapGrenades[i].x,"*");
            }
        }
        attroff(COLOR_PAIR(4));

        //print out grenades

        //print out explosions

        mvprintw(0,gameMap[0].size() + 2,"AMMO: %d",player.ammo);
        mvprintw(1,gameMap[1].size() + 2,"GRENADES: %d",player.grenades);
        mvprintw(2,gameMap[2].size() + 2,"CARPET BOMBS: %d",player.bombs);
        mvprintw(3,gameMap[3].size() + 2,"KILLS: %d",player.kills);
}


bool run = true;
void stopGame() {
    run = false;
}
int prev = -1;
int counter = 0;
void physics() {
    while (!inputQueue.empty()) {
        int key = inputQueue.front();
        inputQueue.pop();
        gameMap[player.y][player.x] = 0;
        if(key == -1) continue;
        
        else if(key == 'a'){
            if(gameMap[player.y][player.x - 1] != 1)
            player.x -= 1;
        }
        else if(key == 'w'){
            if(gameMap[player.y - 1][player.x] != 1)
            player.y -= 1;
        }
        else if(key == 'd'){
            if(gameMap[player.y][player.x + 1] != 1)
            player.x += 1;
        }
        else if(key == 's'){
            if(gameMap[player.y + 1][player.x] != 1)
            player.y += 1;
        }
        else if(key == 3){
            stopGame();
        }
        else if(key == 'k' && player.ammo > 0){
            player.ammo -= 1;
            pos tmp;
            tmp.x = player.x;
            tmp.y = player.y+1;
            tmp.id = player.id;
            tmp.dir = 1;
            if(gameMap[tmp.y][tmp.x] != 1){
            bullets.push_back(tmp);
            }
        }
        else if(key == 'i'&& player.ammo > 0){
            player.ammo -= 1;
            pos tmp;
            tmp.x = player.x;
            tmp.y = player.y-1;
            tmp.id = player.id;
            tmp.dir = 2;
            if(gameMap[tmp.y][tmp.x] != 1){
            bullets.push_back(tmp);
            }
        }
        else if(key == 'j'&& player.ammo > 0){
            player.ammo -= 1;
            pos tmp;
            tmp.x = player.x-1;
            tmp.y = player.y;
            tmp.id = player.id;
            tmp.dir = 3;
            if(gameMap[tmp.y][tmp.x] != 1){
            bullets.push_back(tmp);
            }
        }
        else if(key == 'l'&& player.ammo > 0){
            player.ammo -= 1;
            pos tmp;
            tmp.x = player.x+1;
            tmp.y = player.y;
            tmp.id = player.id;
            tmp.dir = 4;
            if(gameMap[tmp.y][tmp.x] != 1){
            bullets.push_back(tmp);
            }
        }
        else if(key == 'K' && player.grenades > 0){
            player.grenades -= 1;
            grenade tmp;
            tmp.x = player.x;
            tmp.y = player.y+1;
            tmp.timer = 10;
            tmp.dir = 1;
            tmp.counter = 1;
            if(gameMap[tmp.y][tmp.x] != 1){
                mapGrenades.push_back(tmp);
            }
        }
        else if(key == 'I' && player.grenades > 0){
            player.grenades -= 1;
            grenade tmp;
            tmp.x = player.x;
            tmp.y = player.y-1;
            tmp.timer = 10;
            tmp.dir = 2;
            tmp.counter = 1;
            if(gameMap[tmp.y][tmp.x] != 1){
                mapGrenades.push_back(tmp);
            }
        } 
        else if(key == 'J' && player.grenades > 0){
            player.grenades -= 1;
            grenade tmp;
            tmp.x = player.x-1;
            tmp.y = player.y;
            tmp.timer = 10;
            tmp.dir = 3;
            tmp.counter = 1;
            if(gameMap[tmp.y][tmp.x] != 1){
                mapGrenades.push_back(tmp);
            }
        }
        else if(key == 'L' && player.grenades > 0){
            player.grenades -= 1;
            grenade tmp;
            tmp.x = player.x+1;
            tmp.y = player.y;
            tmp.timer = 10;
            tmp.dir = 4;
            tmp.counter = 1;
            if(gameMap[tmp.y][tmp.x] != 1){
                mapGrenades.push_back(tmp);
            }
        }
        else if(key == 'W' && player.bombs > 0){
            player.bombs -= 1;
            carpet tmp;
            tmp.x = player.x;
            tmp.y = player.y-1;
            tmp.dir = 2;
            if(gameMap[tmp.y][tmp.x] != 1){
                carpetBombs.push_back(tmp);
            }
        }
        else if(key == 'S' && player.bombs > 0){
            player.bombs -= 1;
            carpet tmp;
            tmp.x = player.x;
            tmp.y = player.y+1;
            tmp.dir = 1;
            if(gameMap[tmp.y][tmp.x] != 1){
                carpetBombs.push_back(tmp);
            }
        }
        else if(key == 'A' && player.bombs > 0){
            player.bombs -= 1;
            carpet tmp;
            tmp.x = player.x-1;
            tmp.y = player.y;
            tmp.dir = 3;
            if(gameMap[tmp.y][tmp.x] != 1){
                carpetBombs.push_back(tmp);
            }
        }
        else if(key == 'D' && player.bombs > 0){
            player.bombs -= 1;
            carpet tmp;
            tmp.x = player.x+1;
            tmp.y = player.y;
            tmp.dir = 4;
            if(gameMap[tmp.y][tmp.x] != 1){
                carpetBombs.push_back(tmp);
            }
        }                       
    }

    if(player.ammo == 0){
        ammoCounter++;
        if(ammoCounter == 200){
            player.ammo = 20;
            ammoCounter = 0;
        }
    }

//    gameMap[player.y][player.x] = 2;
    draw();




    if(counter == INT_MAX)
        counter = 0;



    //move the bullets
    if(counter % 3 == 0 && counter != 0){
    for(int i = 0; i < bullets.size(); i++){
        //bullet is moving up
        if(bullets[i].dir == 1){
            bullets[i].y += 1;
            //gameMap[bullets[i].y-1][bullets[i].x] = 0;
            if(gameMap[bullets[i].y][bullets[i].x] == 1 || gameMap[bullets[i].y][bullets[i].x] == 3)
                bullets.erase(bullets.begin() + i);
        }
        //bullet is moving down
        else if(bullets[i].dir == 2){
            bullets[i].y -= 1;
            if(gameMap[bullets[i].y][bullets[i].x] == 1 || gameMap[bullets[i].y][bullets[i].x] == 3)
                bullets.erase(bullets.begin() + i);
        }
        //bullet is moving left
        else if(bullets[i].dir == 3){
            bullets[i].x -= 1;
            if(gameMap[bullets[i].y][bullets[i].x] == 1 || gameMap[bullets[i].y][bullets[i].x] == 4){
                bullets.erase(bullets.begin() + i);
            }
        }
        //bullet is moving right
        else if(bullets[i].dir == 4){
            bullets[i].x += 1;
            if(gameMap[bullets[i].y][bullets[i].x] == 1 || gameMap[bullets[i].y][bullets[i].x] == 4)
                bullets.erase(bullets.begin() + i);
        }
        }
    }

/*
    0 = ' ' nothing
    1 = '#' wall
    2 = '@' player
    3 = '|' up/down bullet
    4 = '-' left/right bullet
    5 = '*' grenade
    6 = '/' upper right and lower left explosion
    7 = '\' upper left and lower right explosion
    */

    //move grenades
    if(counter % 5 == 0){
        for(int i = 0; i < mapGrenades.size(); i++){
            //has the grenade exploded
              if(mapGrenades[i].counter == mapGrenades[i].timer){
                         explosion tmp;
                         tmp.id = player.id;
                         int x = mapGrenades[i].x;
                         int y = mapGrenades[i].y;
                     if(gameMap[mapGrenades[i].y+1][mapGrenades[i].x+1] != 1 && y+1 < gameMap.size() && x+1 < gameMap[0].size()){
                         tmp.bottomRight.y = y+1;
                         tmp.bottomRight.x = x+1;
                     }
                     else
                        tmp.bottomRight.x = -1;                 
                     if(gameMap[mapGrenades[i].y-1][mapGrenades[i].x-1] != 1 && y-1 >= 0 && x - 1 >= 0 ){
                         tmp.topLeft.y = y-1;
                         tmp.topLeft.x = x-1;
                     }
                     else
                        tmp.topLeft.x = -1;
                 
                     if(gameMap[mapGrenades[i].y+1][mapGrenades[i].x] != 1 && y + 1 < gameMap.size()){
                         tmp.down.y = y+1;
                         tmp.down.x = x;
                     }
                     else
                        tmp.down.x = -1;
                 
                     if(gameMap[mapGrenades[i].y-1][mapGrenades[i].x] != 1 && y-1 >= 0){
                         tmp.up.y = y-1;
                         tmp.up.x = x;
                     }                
                     else
                        tmp.up.y = -1;

                     if(gameMap[mapGrenades[i].y][mapGrenades[i].x+1] != 1 && x+1 < gameMap[0].size()){
                         tmp.right.y = y;
                         tmp.right.x = x+1;
                     }
                     else
                        tmp.right.x = -1;

                     if(gameMap[mapGrenades[i].y][mapGrenades[i].x-1] != 1 && x - 1 >= 0){
                         tmp.left.y = y;
                         tmp.left.x = x-1;
                     }
                     else tmp.left.x = -1;
                 
                     if(gameMap[mapGrenades[i].y+1][mapGrenades[i].x-1] != 1 && x-1 >= 0 && y+1 < gameMap.size()){
                         tmp.bottomLeft.y = y+1;
                         tmp.bottomLeft.x = x-1;
                     }                 
                     else 
                        tmp.bottomLeft.x = -1;

                     if(gameMap[mapGrenades[i].y-1][mapGrenades[i].x+1] != 1 &&  y-1 >= 0 && x+1 < gameMap[0].size()){
                         tmp.topright.y = y-1;
                         tmp.topright.x = x+1;
                     }
                     else
                        tmp.topright.x = -1;
                        tmp.life = 4;
                        tmp.counter = 0;
                        explosions.push_back(tmp);
                        mapGrenades.erase(mapGrenades.begin()+i);
                        i--;
                    }
        else{
            if(mapGrenades[i].dir == 1){
                if(gameMap[mapGrenades[i].y+1][mapGrenades[i].x] != 1)
                    mapGrenades[i].y += 1;
        }
            else if(mapGrenades[i].dir == 2){
                if(gameMap[mapGrenades[i].y-1][mapGrenades[i].x] != 1)
                    mapGrenades[i].y -= 1;
        }
            else if(mapGrenades[i].dir == 3){
                if(gameMap[mapGrenades[i].y][mapGrenades[i].x-1] != 1)
                    mapGrenades[i].x -= 1;
        }

            else if(mapGrenades[i].dir == 4){
                if(gameMap[mapGrenades[i].y][mapGrenades[i].x+1] != 1)
                    mapGrenades[i].x += 1;
        }
            mapGrenades[i].counter += 1;
        }
       }

       //explosion physics, delete the explosion if it is too old
       for(int i = 0; i <explosions.size(); i++){
            if(explosions[i].counter == explosions[i].life){
                explosions.erase(explosions.begin() + i);
                i--;
            }
            else
            explosions[i].counter++;
       }
    }

    //carpet bomb physics

    for(int i = 0; i < carpetBombs.size(); i++){
        int x = carpetBombs[i].x;
        int y = carpetBombs[i].y;
        if(carpetBombs[i].dir == 1){
            //down
            if(gameMap[y+1][x] != 1){
                grenade p;
                p.x = x;
                p.y = y;
                p.timer = 20;
                p.counter = 18;
                p.dir = 1;
                mapGrenades.push_back(p);
                carpetBombs[i].y += 1;
            }
            else
                carpetBombs.erase(carpetBombs.begin() + i);
        }
        else if(carpetBombs[i].dir == 2){
            //up
            if(gameMap[y-1][x] != 1){
                grenade p;
                p.x = x;
                p.y = y;
                p.timer = 20;
                p.counter = 18;
                p.dir = 2;
                mapGrenades.push_back(p);
                carpetBombs[i].y -= 1;
            }
            else
                carpetBombs.erase(carpetBombs.begin() + i);
        }
        else if(carpetBombs[i].dir == 3){
            //left
            if(gameMap[y][x-1] != 1){
                grenade p;
                p.x = x;
                p.y = y;
                p.timer = 20;
                p.counter = 18;
                p.dir = 3;
                mapGrenades.push_back(p);
                carpet o;
                o.x = x;
                o.y = y;
                o.dir = 1;
                carpetBombs.push_back(o);
                o.dir = 2;
                carpetBombs.push_back(o);
                carpetBombs[i].x -= 1;
            }
            else
                carpetBombs.erase(carpetBombs.begin() + i);
        }
        else if(carpetBombs[i].dir == 4){
            //right
            if(gameMap[y][x+1] != 1){
                grenade p;
                p.x = x;
                p.y = y;
                p.timer = 20;
                p.counter = 18;
                p.dir = 4;
                mapGrenades.push_back(p);
                carpetBombs[i].x += 1;
            }
            else
                carpetBombs.erase(carpetBombs.begin() + i);
        }
    }


    counter++;
    
    if(gameMap[player.y][player.x] == 8){
        player.ammo = 20;
        player.grenades = 20;
        gameMap[player.y][player.x] = 0;
    }

    draw();
}

void createMap(cv:: Mat map){
    vector<int> tmp;
    for(int i = 0; i < map.rows; i++){
        for(int x = 0; x < map.cols; x++){
           // tmp.push_back(map.at<unsigned char>(i,x));
//           printf("%d ",map.at<unsigned char>(i,x));
            if(map.at<unsigned char>(i,x) == 0)
                tmp.push_back(1);
            else
                tmp.push_back(0);
        }
        gameMap.push_back(tmp);
        tmp.erase(tmp.begin(), tmp.end());
  //      cout << endl;
    }
//    cout <<endl;
    int x = rand() % 50 + 1;
    int xtmp, ytmp;
    for(int i = 0; i < x; i++){
        ytmp = rand() % gameMap.size();
        xtmp = rand() % gameMap[0].size();
        if(gameMap[ytmp][xtmp] == 0){
            gameMap[ytmp][xtmp] = 8;
        }
        else
            i--;
    }
  //  gameMap[player.y][player.x] = 2; 
}

void setup(cv::Mat map){
    player.x = 1;
    player.y = 1;
    player.ammo = 20;
    player.grenades = 99;
    player.bombs = 99;
    player.id = rand();
    player.kills = 0;
    createMap(map);
    initscr();
    raw();
    curs_set(FALSE);
    noecho();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_GREEN); //Border
    init_pair(2, COLOR_WHITE, COLOR_BLACK); //Player
    init_pair(3, COLOR_RED, COLOR_BLACK); //Enemy
    init_pair(4, COLOR_YELLOW, COLOR_BLACK); //Bullet
}

int main(int argc, char *argv[]){
    cv::Mat map;
    map = cv::imread(argv[1], 0);
    setup(map);

    while (run) {
        input();
        physics();
    }



    endwin();

    return 0;
}