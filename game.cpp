//============================================================================
// Name        : game.cpp
// Author      : fizza hussain
// Version     : final hopefully
// Copyright   : (c) Reserved
// Description : Basic 2D game...
//============================================================================
#ifndef RushHour_CPP_
#define RushHour_CPP_

#include "util.h"
#include <iostream>
#include <string>
#include <cmath> // for basic math functions such as cos, sin, sqrt
//#include <ctime>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
using namespace std;


const int WINDOW_WIDTH    = 1300;
const int WINDOW_HEIGHT   =  980;
const int BOARD_ROWS      =   26;
const int BOARD_COLS      =   26;
const int TILE_SIZE       =   35;
const int BOARD_WIDTH     = TILE_SIZE * BOARD_COLS;
const int GAME_DURATION   =  180;
const int TARGET_SCORE    =  100;
const int MAX_FUEL        = 500;
const int FUEL_COST       =    1;
const int REFUEL_COST     =    1;
const int BOARD_HEIGHT    = TILE_SIZE * BOARD_ROWS;
Mix_Music* backgroundMusic = nullptr;
Mix_Chunk* pickupSound = nullptr;
Mix_Chunk* dropoffSound = nullptr;
Mix_Chunk* collisionSound = nullptr;
Mix_Chunk* gameOverSound = nullptr;
const int MAX_OTHER_CARS  =   15;
const int MAX_PASSENGERS  =    4;
const int MAX_PACKAGES    =    4;
const int MAX_FUEL_STATIONS =  3;
const int MAX_OBSTACLES     =   7;
int BoardGrid[BOARD_ROWS][BOARD_COLS];
bool occupied[BOARD_ROWS][BOARD_COLS];
const int MAX_LEADERBOARD = 10;
int currentTheme = 0;  // 0 = NORMAL, 1 = DARK, 2 = RETRO









struct MenuCar {
    int x, y, speed;
    bool directionRight;

    MenuCar(int _x, int _y, int _speed, bool _dir) : x(_x), y(_y), speed(_speed), directionRight(_dir) {}

    void update() {
        if (directionRight)
            x += speed;
        else
            x -= speed;

        if (x > WINDOW_WIDTH + 50) x = -60;
        if (x < -60) x = WINDOW_WIDTH + 50;
    }

    
    void draw() {
    float* bodyColor = colors[DEEP_PINK];
    float* roofColor = colors[KHAKI];
    float* windowColor = colors[WHITE];

    
    DrawRectangle(x, y, 50, 20, bodyColor);

    
    DrawRectangle(x + 10, y + 10, 30, 10, roofColor);

    
    DrawRectangle(x + 12, y + 12, 8, 6, windowColor);
    DrawRectangle(x + 30, y + 12, 8, 6, windowColor);

   
    if (directionRight) {
        DrawCircle(x + 50, y + 15, 2, colors[WHITE]); 
        DrawCircle(x + 50, y + 5, 2, colors[WHITE]);
    }
    else {
        DrawCircle(x, y + 15, 2, colors[WHITE]); 
        DrawCircle(x, y + 5, 2, colors[WHITE]);
    }

  
    DrawCircle(x + 10, y - 3, 4, colors[BLACK]);
    DrawCircle(x + 40, y - 3, 4, colors[BLACK]);
}

    
    
    
    
};


struct ScorePopup {
    int x, y, score, life;

    ScorePopup(int _x, int _y, int _score) : x(_x), y(_y), score(_score), life(30) {}

    void update() {
        y += 2;
        life--;
    }

    void draw() {
        char text[10];
        sprintf(text, "+%d", score);
        DrawString(x, y, text, colors[ORANGE]);
    }
};











//--------------------------------------------------------===leaderboard and pos==----------------------------------------------------------------------------------------------




struct LeaderboardEntry {
    int score;
    char name[38];
    
};




class Leaderboard {


public:

    LeaderboardEntry entries[MAX_LEADERBOARD];
    int count;

    Leaderboard() { count = 0;  load(); }

    void load() {
    
        FILE* f = fopen("highscores.txt", "rb");
        
        
        
        if (f == NULL){
          return;
        }
        
        count = 0;
        int size  = 0;
        while (fread(&entries[count],  38 , 1, f) && count < MAX_LEADERBOARD) {
            count++;
        }
        fclose(f);
        
        
    }
    
    
    

    void save() {
        FILE* f =fopen("highscores.txt", "wb");
        
        
        if (f == NULL){
          return;
        }
        
        
        for (int i = 0; i < count; i++) {
         fwrite(&entries[i], 38, 1, f);
        }
        
        
        fclose(f);
        
        
    }



    void tryAddScore(const char* playerName,int score) {
        int insertIndex = -1 , i = 0 , l =0 , k =0 ;
        
        
        for (int i = 0; i < count; i++) {
            if ( entries[i].score < score) {
            
                insertIndex = i;
                break;
            }
            
            
        }
        
        
        if (insertIndex== -1 && count < MAX_LEADERBOARD){
          insertIndex = count;
          }
          
        if (insertIndex == -1) {
        return;
        }
        
        l =0;
        for (int j = MAX_LEADERBOARD - 1; j >insertIndex;  j--) {
            entries[j] = entries[j - 1];
        }

        
        while (playerName[i] != '\0' && i< 29) {
            entries[insertIndex].name[i] = playerName[i];
            i+= 1;
        }
        
        
        entries[insertIndex].name[i] = '\0';
        entries[insertIndex].score = score;
        
        
        if (count< MAX_LEADERBOARD) {
        count += 1;
        
        }
        
        save();
        
        
        
        
    }
    
    
    

    void draw() {
       DrawString(500, 700, "Leaderboard", colors[BLACK]);
       int len = 0 , j = 0 ;
        char scoreStr[10];
         char line[150];
       
        for (int i = 0; i < count;i++) {
        
            intToStr(i + 1, line);
            len = strLength(line);
            line[len++] = '.';
            line[len++] = ' ';
            //cout <<" error writing " << endl;
      
            while (entries[i].name[j] != '\0'){
            
              line[len++] = entries[i].name[j++];
            
             }
            
            
            line[len++] = ' '; 
            line[len++] = '-';
            cout <<" error writing " << endl;
            line[len++] = ' ';
            
           
            
            intToStr(entries[i].score, scoreStr);
            int count ;
            j = 0;
            
            while (scoreStr[j] != '\0'){
            line[len++] = scoreStr[j++];
            count++;
              }
            
            
            line[len] = '\0';
            DrawString(500, 650 - i * 30, line, colors[BLACK]);
        
        
           }
        
        
  }



    int strLength(const char* str){
        int i = 0;
        
        while (str[i] != '\0'){
        i++;
        }
        
        return i;
        
     }
     
     

    void intToStr(int n, char str[]) {
    int i = 0, j, temp;
    bool isNegative = false;
    int x , y;
    
    
    if (n == 0) {
        str[i++] = '0';
        str[i] = '\0';
        x = 0;
        return;
    }
    
    if (n < 0) {
        isNegative = true;
        n = -n;
  }
    
    
    while (n > 0) {
    
    
        str[i++] = (n % 10) + '0';
        n =  n / 10;
        
     }
    
    
    if (isNegative){
    str[i++] = '-';
    }
    
    str[i] = '\0';
    for (j = 0; j < i / 2; j++) {
        temp = str[j];
        y++;
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
 
 
 }
 
 
}

};









struct Position {
    int x, y;
    
    Position(int _x = 0, int _y =0) : x(_x), y(_y) {}
    
    bool equals(Position p) {
    return x == p.x && y == p.y; 
    
    
    }
    
    
};



Position randomRoadPosition() {
  int r , c , q;
  
  
    while (true) {
       r = rand() % BOARD_ROWS;
         c = rand() % BOARD_COLS;
         
         q = r;
        if (BoardGrid[r][c]  == 1&& !occupied[q][c]) {
        
            return Position(c * TILE_SIZE, r * TILE_SIZE);
            
            }
          
 }
}



Position findBesideBuildingSpot() {
    int r , c , nr , nc;
    
    
    for (int attempt = 0; attempt < 500; ++attempt) {
        c = rand() % BOARD_COLS;
        
       r = rand() % BOARD_ROWS;
       
        
        if (BoardGrid[r][c] != 1 || occupied[r][c]){
        continue;
          }
        
        for (int dr= -1; dr<= 1;  ++dr) {
            for (int dc = -1 ; dc <= 1; ++dc) {
                if (dc == 0&& dr == 0){
                  continue;
                }
                  nc = c + dc;
                  
                 nr = r + dr;
                
                 
                if (nr >= 0 && nr <BOARD_ROWS && nc >=0 && nc <  BOARD_COLS) {
                    if (BoardGrid[nr][nc] ==0) {
                     return Position(c * TILE_SIZE, r * TILE_SIZE);
                     
                     }
                    
                    
                }
                
                 }
         }
        
        
        
    }
    return randomRoadPosition();
}


void markOccupied(Position p) {
  int r , c;
    c = p.x / TILE_SIZE;
     int rx;
    r = p.y / TILE_SIZE;
     
     
    occupied[r][c] = true;
    
}



bool visited[BOARD_ROWS][BOARD_COLS];

bool dfsVisit(int r, int c) {

    if (r < 0 || r >= BOARD_ROWS|| c >=BOARD_COLS ||  c < 0) {
      return false;
     
     }
    
    if (visited[r][c]){
      return false;
     }
    if (BoardGrid[r][c] != 1|| occupied[r][c]) {
     return false;
   }

      visited[r][c] = true;

    
    if ((r == 0 && c == 0)|| c == 0 ||r == 0  ||c == BOARD_COLS - 1 ||  r == BOARD_ROWS - 1 ){
      
      return true;
        }
        
        
        

    return dfsVisit(r + 1, c)|| dfsVisit(r - 1, c)  || dfsVisit(r, c + 1) ||dfsVisit(r, c - 1);
    
}



bool isReachable(int row, int col) {


    for (int i = 0; i< BOARD_ROWS; i++){
        for (int j = 0; j < BOARD_COLS;j++){
            visited[i][j] = false;
            }
            }
    return dfsVisit(row, col);
    
    
}








// ---------------------------------------------------===itemsonboard Base ===---------------------------------------------------------------------------------------------------


class itemsonboard { //BASE HOGAI ANDAR OBJS AND ABSTRACT CLASS 
    int x, y;

public:

    itemsonboard(int _x = 0, int _y = 0) : x(_x), y(_y) {}
    
    virtual void draw() = 0;
    
    virtual void update(double) {}
    
    int getX() const {
    return x; 
    }
    void setY(int val) {
    y = val; }
    
    
    int getY() const { 
    return y; 
    
    }
    void setX(int val) {
    x = val; 
    }
    
    
    
};



//---------------------- === Obstacles ===------------------------------

class Obstacle : public itemsonboard { // andar 2 aur types box and tree so abstract 

public:

    Obstacle(int _x, int _y) : itemsonboard(_x, _y) {}
    
    
    virtual void draw() = 0; 
    
};



class BoxObstacle : public Obstacle {

public:

    BoxObstacle(int _x, int _y) : Obstacle(_x, _y) {}
    void draw() override {
       int x1 , y1; 
       
       y1 = getY();
       
       x1 = getX();
       
        DrawSquare(x1, y1, TILE_SIZE - 10, colors[GRAY]);
     }


};



class TreeObstacle : public Obstacle {


public:
    TreeObstacle(int _x, int _y) : Obstacle(_x, _y) {}
    
    
    void draw() override {
         int x2, y2; 
       y2 = getY();
       x2 = getX();
       
    
        DrawRectangle(x2 + TILE_SIZE / 3, y2 + 10, TILE_SIZE / 4, TILE_SIZE / 2, colors[SADDLE_BROWN]); 
        DrawCircle(x2 + TILE_SIZE / 2, y2 + TILE_SIZE / 2 + 5, TILE_SIZE / 4, colors[GREEN]); 
    }


};


//---------------------------------------------------------- === OtherCar ===

class OtherCar : public itemsonboard {
    int dirX;
    int dirY;
    int frameCounter;


public:

    OtherCar(int _x, int _y) : itemsonboard(_x, _y) {
        if (rand() % 2) {
          dirX = 2;
          dirY = 0; 
        
        }
        else            {
        dirY = 2; 
        
        dirX = 0;
      
        
        }
        
        frameCounter = 0;

     }


    void draw() override {
        DrawRectangle(getX() + 5, getY() + 10, TILE_SIZE - 10, TILE_SIZE - 20, colors[BLUE]);
        DrawCircle(getX() + TILE_SIZE - 10, getY() + 10, 5, colors[BLACK]);
        DrawCircle(getX() + 10, getY() + 10, 5, colors[BLACK]);
        
    }


     void changeDirection() {
   int dir;
    
    dir = rand() % 4;
    
    
    if (dir == 0) { 
      dirX = TILE_SIZE;
      dirY = 0; 
    
    }
    else if (dir == 2) {
       dirY = TILE_SIZE; 
      dirX = 0;
     
    }
    else if (dir == 1) {
    dirY = 0; 
    dirX = -TILE_SIZE;
    
     }
    else {
    dirX = 0; 
    dirY = -TILE_SIZE; 
    }
    
}



    void update(double) {
     frameCounter++;
     int r , c, newR , newC , newX , newY;
     
     
    if (frameCounter % 2 != 0){
       return; 
       
       }
    
    
    int x3 = getX();
    
     newX = x3 + dirX;
     
       r = (getY() + TILE_SIZE / 2) / TILE_SIZE;
     c = (x3 + TILE_SIZE / 2) / TILE_SIZE;

     int dir = dirY;
    newY = getY() + dirY;

     newR = (newY + TILE_SIZE / 2) / TILE_SIZE;
     newC = (newX + TILE_SIZE / 2) / TILE_SIZE;

   
    if (newR < 0 || newR >= BOARD_ROWS || newC < 0 || newC >= BOARD_COLS || BoardGrid[newR][newC] != 1) {
        changeDirection();  
    }
    
    else {
        setY(newY);
        setX(newX);
        
    
    }
    
    
}




    int getDirX() const { return dirX; }
    void setDirY(int val);
    void setddr(int);
    int getDirY() const { return dirY; }
    void setDirX(int val) { dirX = val; }
    
};




void OtherCar::setDirY(int dy) { dirY = dy; }



//------------------------------------------------------------------ === Passenger ===

class Passenger : public itemsonboard {
    bool picked;
    int destX;
    int destY;
    int fare;

public:

    Passenger(int _x, int _y) : itemsonboard(_x, _y), picked(false) {
    Position start;
    Position d;
    
    
    do {
        start = randomRoadPosition();
    } while (!isReachable(start.y / TILE_SIZE, start.x / TILE_SIZE));
    setY(start.y);

    setX(start.x);
    
    markOccupied(start);

  
    
    do {
        d = randomRoadPosition();
    } while (!isReachable(d.y / TILE_SIZE, d.x / TILE_SIZE));

      destY = d.y;
    destX = d.x;
  
    markOccupied(d);

    fare = rand() % 36 + 5;
    
    
    
}


    void draw() override {
    
        if (!picked) {
              DrawCircle(getX() + 50 / 2, getY() + 50 / 2 + 5, 3, colors[BLACK]);
              DrawLine(getX() + 50 / 2, getY() + 50 / 2, getX() + 50 / 2, getY() + 50 / 2 - 10, 2, colors[BLACK]);
              DrawLine(getX() + 50 / 2, getY() + 50 / 2 - 5, getX() + 50 / 2 - 5, getY() + 50 / 2 - 10, 2, colors[BLACK]);
              DrawLine(getX() + 50 / 2, getY() + 50 / 2 - 5, getX() + 50 / 2 + 5, getY() + 50 / 2 - 10, 2, colors[BLACK]);
              DrawLine(getX() + 50 / 2, getY() + 50 / 2 - 10, getX() + 50 / 2 - 5, getY() + 50 / 2 - 15, 2, colors[BLACK]);
              DrawLine(getX() + 50 / 2, getY() + 50 / 2 - 10, getX() + 50 / 2 + 5, getY() + 50 / 2 - 15, 2, colors[BLACK]);
        }
        
        else {
            DrawSquare(destX, destY, TILE_SIZE -5 , colors[GREEN]);
          
          
          }
          
          
    }



    bool isPicked() const{
    return picked; }
    
     int getDestY() const  {
    return destY; }
    int getFare() const {
    
    return fare; }
    
    void setPicked(bool val) {
    picked = val; }
    int getDestX() const {
    return destX; }
    
   
    
    
};



// ------------------------------------------------------------=== Package ===

class Package : public itemsonboard {
    bool picked;
    int destX ;
    int destY;
    int fare;

public:


    Package(int _x, int _y) : itemsonboard(_x, _y), picked(false) {
    Position start;
    Position d;
    
    
    do {
        start = findBesideBuildingSpot();
    } while (!isReachable(start.y / TILE_SIZE, start.x / TILE_SIZE));


    setY(start.y);

    setX(start.x);
    
    markOccupied(start);

    
    do {
        d = findBesideBuildingSpot();
    } while (!isReachable(d.y / TILE_SIZE, d.x / TILE_SIZE));

    destY = d.y;
    destX = d.x;
    
    markOccupied(d);

    fare = rand() % 36 + 5;


}

    void draw() override {
        if (!picked)
            DrawSquare(getX(), getY(), TILE_SIZE - 15, colors[SADDLE_BROWN]);
        else
            DrawSquare(destX, destY, TILE_SIZE - 5, colors[GREEN]);
    }
    
    
     int getDestX() const { 
     return destX; }
    void setPicked (bool val) {
    picked = val; }
    bool isPicked() const  {
    return picked; }
    int getDestY() const {
    return destY; }
    int getFare() const  {
    return fare; }




};



//----------------------------------------------------------- === Fuel Station ===

class FuelStation : public itemsonboard {

public:

    FuelStation(int _x, int _y) : itemsonboard(_x, _y) {
    
        markOccupied(Position(_x, _y));
     }


    void draw() override {
        DrawRectangle(getX() + 8, getY() + 5, TILE_SIZE - 10, TILE_SIZE, colors[RED]);
    }
    
    
    
};



//--------------------------------------------------------------------- === Player ===

class Player : public itemsonboard {

protected:
    

    int fuel;
    int score;
    Passenger* targetPassenger;
    Package* targetPackage;
    int wallet;
    int jobsDone;

public:
     static const int DELIVERY = 1;
     static const int TAXI = 0;
    
    
    
    Player(int _x = 0, int _y = BOARD_HEIGHT - TILE_SIZE):itemsonboard(_x, _y),fuel(MAX_FUEL),  score(0), wallet(0), jobsDone(0),  targetPassenger(nullptr), targetPackage(nullptr) {}

    virtual void draw() = 0;
    virtual int getRole() = 0;
    
    
    int getJobsDone() const { 
    return jobsDone; 
    }
    int getScore() const {
    return score; }
    int getWallet() const  {
    return wallet; }
    
    int getFuel() const {
    return fuel; }
    void setFuel(int f) {
    fuel = f; 
    }
    void setTargetPassenger(Passenger* p) {
    targetPassenger = p; }
    void setWallet(int w) { 
    wallet = w; }
    void setJobsDone(int j) { 
    jobsDone = j;
    }

    Passenger* getTargetPassenger()  { 
    return targetPassenger; }
    
    Package* getTargetPackage()  { 
    return targetPackage;
    }
     void setScore(int s) {
    score = s; }
    
    void setTargetPackage(Package* pkg) {
    
    targetPackage = pkg; }
    
    
    
    
    
};








//---------------------------------------------------------- === TaxiDriver ===



class TaxiDriver : public Player {
public:


    TaxiDriver() : Player() {}

    void draw() override {
    
        
        DrawRectangle(getX() + 5, getY() + 10, TILE_SIZE - 10, TILE_SIZE - 20, colors[ORANGE]);
        DrawCircle(getX() + 10, getY() + 10, 5, colors[BLACK]);
        DrawCircle(getX() + TILE_SIZE - 10, getY() + 10, 5, colors[BLACK]);
        DrawString(getX(), getY() - 10, "Taxi", colors[BLACK]);
    
    }
    
    



    int getRole() override {
        
        return TAXI;
     }



};


//-------------------------------------------- === DeliveryDriver ===

class DeliveryDriver : public Player {
public:
    DeliveryDriver() : Player() {}

    void draw() override {
        DrawRectangle(getX() + 5, getY() + 10, TILE_SIZE - 10, TILE_SIZE - 20, colors[DARK_SALMON]);
        DrawCircle(getX() + 10, getY() + 10, 5, colors[BLACK]);
        DrawCircle(getX() + TILE_SIZE - 10, getY() + 10, 5, colors[BLACK]);
        DrawString(getX(), getY() - 10, "Delivery", colors[BLACK]);
     }

    int getRole() override {
        return DELIVERY;
        
     }
    
    
};



// ---------------------------------------------------------=== Board ===

class Board {

    Player* player;

    Package*   packages[MAX_PACKAGES];
    FuelStation* fuels[MAX_FUEL_STATIONS];
    OtherCar*  cars[MAX_OTHER_CARS];
    Passenger* passengers[MAX_PASSENGERS];
    
    Obstacle*    obstacles[MAX_OBSTACLES];

    int carCount;
    int passengerCount;
    int packageCount;
    int fuelCount;
    int obstacleCount;



public:


    Board() {
    this->player = new TaxiDriver();
        init();
        
    }

    void init() {
        InitRandomizer();
           
           
            int totalTiles , reservedRoads = 0 ,  freeTiles , targetBuildings , assignedBuildings = 0, r , c ;
            
        //buildings 
        
         totalTiles = (BOARD_COLS) * (BOARD_ROWS);
         

        cout << " totalTiles " << totalTiles << endl;
        
       
       
        for (int i = 0; i <BOARD_ROWS; i++) {
            for (int j = 0; j < BOARD_COLS;j++) {
                if (i %5 == 0 ||j % 5 == 0) {
                    BoardGrid[i][j] = 1; // road
                    reservedRoads++;
                }
                else {
                    BoardGrid[i][j]= -1; // unassigned
                    }
        }
        }





         freeTiles = totalTiles - reservedRoads;
         assignedBuildings = 0;
         targetBuildings = (int)(totalTiles * 0.35);
         

       
        while (assignedBuildings < targetBuildings) {
        
             c = rand() % BOARD_COLS;
             r = rand() % BOARD_ROWS;
             
            if (BoardGrid[r][c] == -1) {
                BoardGrid[r][c] = 0; 
                
                assignedBuildings++;
                
               }
             }
        
        
           cout  << " assignedBuildings " << assignedBuildings << endl;
       
        for (int i = 0; i < BOARD_ROWS; i++) {
            for (int j = 0; j < BOARD_COLS; j++) {
            
                if (BoardGrid[i][j] == -1) {
                    BoardGrid[i][j] = 1; // road
                    
                    
             }
            }
         }

        
        
        
        
        
        
        
        
        

        //obstacles
        obstacleCount = MAX_OBSTACLES;
        for (int i = 0; i < obstacleCount; i++) {
            Position p = randomRoadPosition();
            if (rand() % 2 == 0){
                obstacles[i] = new BoxObstacle(p.x, p.y);
                }
            else{
            
                obstacles[i] = new TreeObstacle(p.x, p.y);
              }
            
            markOccupied(p);
           }
        int count = 2;


        //fuel
        fuelCount = MAX_FUEL_STATIONS;
        for (int i = 0;i < fuelCount;  i++) {
            Position p =findBesideBuildingSpot();
            fuels[i] = new FuelStation(p.x, p.y);
            
            
          }

        //packages 
        packageCount = rand() % 3 + 2;
        for (int i = 0;i < packageCount;i++) {
            Position p = findBesideBuildingSpot();
            
            packages[i] = new Package(p.x, p.y);
           
           }

        //passengers
        passengerCount = rand() % 3 + 2;
        for (int i = 0; i < passengerCount; i++) {
              Position p = randomRoadPosition();
            passengers[i] = new Passenger(p.x, p.y);
           
           
        }

        //cars
        carCount = 5;
        count = 0 ;
        for (int i = 0; i < carCount; i++) {
        
            Position p = randomRoadPosition();
              markOccupied(p);
            cars[i] = new OtherCar(p.x, p.y);
          
        }
        
        
    }
    
    
    

    void maintainEntities() {
        
        int unpickedP = 0, r , c , dr , dc , nr , nc ;
       bool nearBuilding = false;
        int unpickedK = 0;
        
        
        for (int i = 0; i < passengerCount; i++)
            if (!passengers[i]->isPicked()){
              unpickedP++;
              }

        while (unpickedP <2 && passengerCount < MAX_PASSENGERS) {
            Position p = randomRoadPosition();
            bool isOccupied = false;
            
            for (int j = 0; j < fuelCount; j++)
                if (fuels[j]->getX() ==p.x &&  fuels[j]->getY()== p.y){
                
                    isOccupied = true;
                    }


            for (int j = 0; j < obstacleCount; j++)
                if (obstacles[j]->getX() ==p.x && obstacles[j]->getY()== p.y){
                
                    isOccupied = true;}

            

            if (!isOccupied) {
                passengers[passengerCount++] = new Passenger(p.x, p.y);
                unpickedP++;
            }
            
            
        }

       
        
        for (int i = 0; i < packageCount; i++)
            if (!packages[i]->isPicked()){
                unpickedK++;
             }

        while (unpickedK < 1 && packageCount < MAX_PACKAGES) {
             c = rand() % BOARD_COLS;
             
             r = rand() % BOARD_ROWS;
             
            if (BoardGrid[r][c] != 1){
            continue;
            
            }

            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                   nc = c + dc;
                     nr = r + dr;
                   if (nr >= 0 && nr <BOARD_ROWS && nc >= 0 && nc <BOARD_COLS) {
                          if (BoardGrid[nr][nc] == 0){
                            nearBuilding = true;
                            }
                }
                }
      
      
      }
      
            bool isOccupied = false;
            
            
            if (!nearBuilding){
            continue;
            }
            
            
            Position p(c * TILE_SIZE, r * TILE_SIZE);
            for (int j = 0; j < fuelCount; j++)
            
            
                if (fuels[j]->getX() == p.x && fuels[j]->getY() == p.y){
                
                
                    isOccupied = true;
                    }
                    
                    
            for (int j = 0; j < obstacleCount; j++)
            
            
                if (obstacles[j]->getX() == p.x && obstacles[j]->getY() == p.y){
                    isOccupied = true;
                    }
                    
                    
            

            if (!isOccupied) {
                packages[packageCount++] = new Package(p.x, p.y);
                unpickedK++;
            
            }
        }



}
    
    
    
    
    
    void addCar(OtherCar* c) {
    if (carCount < MAX_OTHER_CARS) {
        cars[carCount++] = c;
        markOccupied(Position(c->getX(),c->getY()));
    }
}



void removePassenger(int index) {
    if (index >= 0 && index <passengerCount) {
        delete passengers[index];
        
        
        for  (int i = index; i < passengerCount -1; i++) {
            passengers[i] = passengers[i +1];
         }
        
        
        passengerCount-= 1;
    }
}


void removePackage(int index) {
    if (index >= 0 && index < packageCount) {
        delete packages[index];
        
        
        for (int i = index; i < packageCount - 1; i++) {
            packages[i] = packages[i + 1];
          }
        packageCount--;
        
        
    }
}


   void drawBackground() {
    float* tileColor = colors[WHITE];      // For roads
    float* buildingColor = colors[BLACK];  // For buildings

    if (currentTheme == 1) { // DARK MODE
        tileColor = colors[GRAY];
        buildingColor = colors[BLACK];
    }
    else if (currentTheme == 2) { // RETRO MODE
        tileColor = colors[MISTY_ROSE];
        buildingColor = colors[MAROON];
    }

    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            DrawRectangle(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE,
                BoardGrid[i][j] == 1 ? tileColor : buildingColor);
        }
    }
}



    void draw() {
        drawBackground();
        for (int i = 0; i < carCount; i++)   {    
        cars[i]->draw();
        }
        for (int i = 0; i < obstacleCount; i++){ 
        obstacles[i]->draw();
        }
        
        
        for (int i = 0; i < fuelCount; i++)   {   fuels[i]->draw(); }

        if (player->getRole() == Player::TAXI) {
            for (int i = 0; i < passengerCount; i++) passengers[i]->draw();
        }
        else {
            for (int i = 0; i < packageCount; i++) {
            packages[i]->draw();
        } }

        
        int x = 0;
        int y = 0;
        
        DrawRectangle( 0 ,  0 , TILE_SIZE, TILE_SIZE - 10, colors[SLATE_GRAY]);

        player->draw();
        
    }

    void update(double dt) {
        for (int i = 0; i < carCount; i++) {
            cars[i]->update(dt);
        }
    }

    Player* getPlayer() { return player; }


    void setPlayer(Player* newPlayer) {
    if (player){
    delete player;
    }
    player = newPlayer;
}


    int getCarCount() const { 
    return carCount;
    }
    int getPackageCount() const { 
    return packageCount; 
    }
     int getObstacleCount() const {
     return obstacleCount; }
    int getPassengerCount() const {
    return passengerCount; 
    }
    
    
    int getFuelCount() const {
    return fuelCount;
    }
   

    OtherCar* getCar(int i) {
        if (i >= 0 && i < carCount){
        return cars[i];
        }
        return nullptr;
    }

    Passenger* getPassenger(int i) {
        if (i >= 0 && i< passengerCount){
        return passengers[i];
        }
        
        
        return nullptr;
    }

    Package* getPackage(int i) {
        if (i >= 0 && i< packageCount){
        return packages[i];
        }
        
        return nullptr;
    }

    FuelStation* getFuel(int i) {
        if (i >= 0 && i< fuelCount){
        
        return fuels[i];
        }
        
        return nullptr;
    
    
    }

    Obstacle* getObstacle(int i) {
        if (i >= 0 && i <obstacleCount) {
        return obstacles[i];
        
        }
        return nullptr;
    }
    
    
    
    
    
    
};

//----------------------------------------------------------------==GAME==-------------------------------------------------------------------------------------------------------

class Game {

public:
    Board board;
    double elapsed;
    bool selectingRole, gameOver;
    char playerName[30];
    int nameLength;
    int fuelInputLength;
    bool refuelFailed;
    bool enteringName;
    bool viewingLeaderboard;
    int menuSelection;
    bool confirmingRoleSwitch;
    bool refueling;
    char fuelInput[5];
//additiona;
bool selectingTheme;
int themeSelection;
MenuCar* menuCars[5]; 
int pulseSize;
bool growing;
int trafficLightState;  
int trafficLightTimer; 
int flashFrames;
ScorePopup* scorePopups[10];
int popupCount;




    

    Leaderboard leaderboard;

    Game() : elapsed(0), gameOver(false) {
        selectingRole = false;
        nameLength = 0;
        playerName[0] = '\0';
        confirmingRoleSwitch = false;
        refueling = false;
        refuelFailed = false;
        fuelInput[0] = '\0';
        fuelInputLength = 0;
        enteringName = false;
        viewingLeaderboard = false;
        menuSelection = 0;
        selectingTheme = true;
themeSelection = 0;

    pulseSize = 0;
growing = true;
flashFrames = 0;


for (int i = 0; i < 5; i++) {
    int y = 150 + i * 80;
    int x = rand() % WINDOW_WIDTH;
    bool dir = rand() % 2;
    int speed = 2 + rand() % 3;
    menuCars[i] = new MenuCar(x, y, speed, dir);
}
  trafficLightState = 0;
trafficLightTimer = 0;
popupCount = 0;
for (int i = 0; i < 10; i++) {
scorePopups[i] = nullptr;
}



    }




    void drawRoleSelection() {
        DrawString(500, 600, "Select Role", colors[BLACK]);
        DrawString(500, 550, "1 - Taxi Driver", colors[YELLOW]);
        DrawString(500, 500, "2 - Delivery Driver", colors[ORANGE]);
        DrawString(500, 450, "3 - Random", colors[DARK_GREEN]);
    }
    
    

    void drawGameOver() {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        if (board.getPlayer()->getScore() >= TARGET_SCORE){
            glClearColor(0.678f, 0.847f, 0.902f, 1.0f);  // light blue (RGB for LightSkyBlue)
    glClear(GL_COLOR_BUFFER_BIT); 
            DrawString(500, 600, "CONGRATULATIONS!!!!   YOU WON!", colors[PINK]);
            for (int i = 0; i < 25; i++) {
              int fx = rand() % BOARD_WIDTH;
              int fy = rand() % BOARD_HEIGHT;
              float* color = colors[rand() % 56];
              DrawCircle(fx, fy, 5, color);
          }

            }
        else{
            DrawString(500, 600, "GAME OVER! you lostt  ", colors[WHITE]);
            }
        char scoreStr[10], walletStr[10];
        leaderboard.intToStr(board.getPlayer()->getScore(), scoreStr);
        leaderboard.intToStr(board.getPlayer()->getWallet(), walletStr);

        DrawString(500, 550, "Score:  ", colors[WHITE]);
        DrawString(600, 550, scoreStr, colors[WHITE]);
        DrawString(500, 500, "Wallet: ", colors[WHITE]);
        DrawString(600, 500, walletStr, colors[WHITE]);
        DrawString(500, 450, "Press SPACE to Play Again", colors[WHITE]);

        glutSwapBuffers();
    }

    void drawHUD() {
        char scoreStr[10], walletStr[10], fuelStr[10];
        leaderboard.intToStr(board.getPlayer()->getScore(), scoreStr);
        leaderboard.intToStr(board.getPlayer()->getWallet(), walletStr);
        leaderboard.intToStr(board.getPlayer()->getFuel(), fuelStr);
        int remaining , minutes , seconds , cy , cx , py , px;
        DrawString(BOARD_WIDTH + 20, WINDOW_HEIGHT - 50, "Score: ", colors[BLACK]);
        DrawString(BOARD_WIDTH + 100, WINDOW_HEIGHT - 50, scoreStr, colors[BLACK]);
        DrawString(BOARD_WIDTH + 20, WINDOW_HEIGHT - 80, "Wallet: ", colors[BLACK]);
        DrawString(BOARD_WIDTH + 100, WINDOW_HEIGHT - 80, walletStr, colors[BLACK]);
        DrawString(BOARD_WIDTH + 20, WINDOW_HEIGHT - 110, "Fuel: ", colors[BLACK]);
        DrawString(BOARD_WIDTH + 100, WINDOW_HEIGHT - 110, fuelStr, colors[BLACK]);

        remaining = GAME_DURATION - (int)elapsed;
         minutes = remaining / 60;
         seconds = remaining % 60;
        char timeStr[6] = {'0', '0', ':', '0', '0', '\0'};
        timeStr[1] = '0' + minutes % 10;
        timeStr[3] = '0' + (seconds / 10);
        timeStr[4] = '0' + (seconds % 10);

        DrawString(BOARD_WIDTH + 20, WINDOW_HEIGHT - 140, "Time: ", colors[BLACK]);
        DrawString(BOARD_WIDTH + 100, WINDOW_HEIGHT - 140, timeStr, colors[BLACK]);

        DrawString(BOARD_WIDTH + 20, WINDOW_HEIGHT - 170, "Carrying:", colors[BLACK]);

        if (board.getPlayer()->getRole() == Player::TAXI &&
            board.getPlayer()->getTargetPassenger() != nullptr) {
            cx = BOARD_WIDTH + 130;
             cy = WINDOW_HEIGHT - 180;
            DrawCircle(cx, cy + 15, 5, colors[BLACK]);
            DrawLine(cx, cy + 15, cx, cy + 5, 2, colors[BLACK]);
            DrawLine(cx, cy + 10, cx - 5, cy + 5, 2, colors[BLACK]);
            DrawLine(cx, cy + 10, cx + 5, cy + 5, 2, colors[BLACK]);
            DrawLine(cx, cy + 5, cx - 5, cy, 2, colors[BLACK]);
            DrawLine(cx, cy + 5, cx + 5, cy, 2, colors[BLACK]);
        }
        
        
        
        else if (board.getPlayer()->getRole() == Player::DELIVERY &&
                 board.getPlayer()->getTargetPackage() != nullptr) {
            px = BOARD_WIDTH + 130;
            
             py = WINDOW_HEIGHT - 185;
            DrawSquare(px, py, 15, colors[SADDLE_BROWN]);
        }
        else {
            DrawString(BOARD_WIDTH + 130, WINDOW_HEIGHT - 170, "-", colors[RED]);
        }


       int fuelX , fuelY, tankWidth , tankHeight , filledWidth;

        fuelX = BOARD_WIDTH + 20;
        tankWidth = 100;
        tankHeight = 15;
         fuelY = WINDOW_HEIGHT - 220;
         



        DrawRectangle(fuelX, fuelY, tankWidth, tankHeight, colors[BLACK]);
        filledWidth = (board.getPlayer()->getFuel() * tankWidth) / MAX_FUEL;

        float* fuelColor = colors[GREEN];
        if (board.getPlayer()->getFuel() < 100) fuelColor = colors[RED];
        else if (board.getPlayer()->getFuel() < 250) fuelColor = colors[ORANGE];



        DrawRectangle(fuelX, fuelY, filledWidth, tankHeight, fuelColor);
        DrawString(fuelX + 110, fuelY, "Fuel Tank", colors[BLACK]);
        
        
    }
    
    
    
    
    
    

    void draw() {
    
      

    
    
    
    
    
        if (selectingTheme) {
        glClearColor(0.678f, 0.847f, 0.902f, 1.0f); // LightSkyBlue
        glClear(GL_COLOR_BUFFER_BIT);
    DrawString(500, 600, "Choose Theme:", colors[BLACK]);
    if (themeSelection == 0) DrawString(500, 550, "> Normal", colors[RED]);
    else DrawString(500, 550, "  Normal", colors[BLACK]);

    if (themeSelection == 1) DrawString(500, 500, "> Pastel Mode more like grey mode ", colors[RED]);
    else DrawString(500, 500, "  Dark Mode", colors[BLACK]);

    if (themeSelection == 2) DrawString(500, 450, "> Retro more like red mode", colors[RED]);
    else DrawString(500, 450, "  Retro", colors[BLACK]);

    DrawString(500, 400, "Use W/S to move, ENTER to select", colors[BLACK]);
    return;
}

    
        if (viewingLeaderboard) {
            leaderboard.draw();
            DrawString(500, 100, "Press B to go back", colors[BLACK]);
            return;
        }

        if (selectingRole) {
            drawRoleSelection();
            return;
        }



        if (enteringName) {
            glClearColor(0.678f, 0.847f, 0.902f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
            DrawString(500, 600, "Enter Name: ", colors[BLACK]);
            DrawString(620, 600, playerName, colors[BLACK]);
            return;
        }
     
     
     
         if (playerName[0] == '\0') {
    glClearColor(0.678f, 0.847f, 0.902f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw moving cars
    for (int i = 0; i < 5; i++) {
        menuCars[i]->draw();
    }

    
    DrawString(460 - pulseSize, 700 + pulseSize / 2, "RUSH HOUR", colors[PINK]);

    // Menu options
    if (menuSelection == 0)
        DrawString(500, 600, "> Play Game", colors[RED]);
    else
        DrawString(500, 600, "  Play Game", colors[BLACK]);

    if (menuSelection == 1)
        DrawString(500, 550, "> View Leaderboard", colors[RED]);
    else
        DrawString(500, 550, "  View Leaderboard", colors[BLACK]);

    DrawString(480, 480, "Use W/S to move, ENTER to select", colors[BLACK]);
    
   
      int tlX = WINDOW_WIDTH - 300;
      int tlY = WINDOW_HEIGHT - 500;
      DrawRectangle(tlX, tlY, 70, 180, colors[BLACK]);

      
      DrawCircle(tlX + 33, tlY + 140, 15, trafficLightState == 2 ? colors[RED] : colors[MAROON]);
      DrawCircle(tlX + 33, tlY + 95, 15, trafficLightState == 1 ? colors[YELLOW] : colors[DARK_GOLDEN_ROD]);
      DrawCircle(tlX + 33, tlY + 45, 15, trafficLightState == 0 ? colors[GREEN] : colors[DARK_GREEN]);

    
    
    
    return;
}

         

        if (confirmingRoleSwitch) {
            DrawString(450, 600, "Switch Role?", colors[BLACK]);
            DrawString(450, 550, "Press ENTER to confirm", colors[DARK_GREEN]);
            DrawString(450, 500, "Press X to cancel", colors[RED]);
            return;
        }

       
        
        

        if (refueling) {
            board.draw(); 
          drawHUD();   
            DrawString(BOARD_WIDTH + 20, WINDOW_HEIGHT - 300, "Enter fuel amount (Press Enter):", colors[RED]);
            DrawString(BOARD_WIDTH + 20, WINDOW_HEIGHT - 330, fuelInput, colors[BLACK]);
            if (refuelFailed) {
                DrawString(BOARD_WIDTH + 20, WINDOW_HEIGHT - 360, "Money not enough. Retry press x to exit", colors[RED]);
            }
            return;
              }

        if (gameOver) {
            drawGameOver();
            return;
   }

        board.draw();
        drawHUD();
        for (int i = 0; i < popupCount; i++) {
    if (scorePopups[i]) scorePopups[i]->draw();
}

        
        
        
        if (flashFrames > 0) {
    flashFrames--;
    glColor4f(1.0f, 0.0f, 0.0f, 0.3f); 
    DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, colors[RED]);
    glColor4f(1, 1, 1, 1); 
}

}




    void update(double dt) {
    
          if (playerName[0] == '\0') {
    for (int i = 0; i < 5; i++) {
        menuCars[i]->update();
    }

    if (growing) {
        pulseSize++;
        if (pulseSize > 10) growing = false;
    } else {
        pulseSize--;
        if (pulseSize < 0) growing = true;
    }
    
    
    // 
    trafficLightTimer++;
    if (trafficLightTimer >= 10) {
        trafficLightTimer = 0;
        trafficLightState = (trafficLightState + 1) % 3;
    }



    return;
}

for (int i = 0; i < popupCount; i++) {
    if (scorePopups[i]) {
        scorePopups[i]->update();
        if (scorePopups[i]->life <= 0) {
            delete scorePopups[i];
            for (int j = i; j < popupCount - 1; j++) {
                scorePopups[j] = scorePopups[j + 1];
            }
            popupCount--;
            i--;
        }
    }
}
    
    
    
    
        if (selectingRole || gameOver) return;

        elapsed += dt;
        board.update(dt);
        board.maintainEntities();



        if (elapsed >= GAME_DURATION || board.getPlayer()->getFuel() <= 0 || board.getPlayer()->getScore() < 0) {
            gameOver = true;
            leaderboard.tryAddScore(playerName, board.getPlayer()->getScore());
            Mix_PlayChannel(-1, gameOverSound, 0);

           }
        
        
    }




    void resetGame() {
        board = Board();  
         nameLength = 0;
         playerName[0] = '\0';
          menuSelection = 0;
        selectingRole = false;   
         
    enteringName = false;  
        elapsed = 0;
        
        gameOver = false;
        confirmingRoleSwitch = false;
        refueling = false;
        refuelFailed = false;
        fuelInput[0] = '\0';
        fuelInputLength = 0;
        
        viewingLeaderboard = false;
       
    }
      
      
        
        
        
        
        
        
};

static Game* game = nullptr;

//============================================================================
// === GLUT Callbacks ===

/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */
void SetCanvasSize(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1, 1); // Set coordinate system
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



/*
 * Main Canvas drawing function.
 * */
void GameDisplay() {
    glClearColor(1, 1, 1, 1); // white background
    glClear(GL_COLOR_BUFFER_BIT);
    game->draw();
    glutSwapBuffers();
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */
 

void NonPrintableKeys(int key, int x, int y) {

    int newX , newY, row ,col ,ox ,oy;
    if (game->selectingRole || game->gameOver){
    return;
    }

    Player* p = game->board.getPlayer();
    newX = p->getX();
     newY = p->getY();

    // Move one tile at a time
    if (key == GLUT_KEY_LEFT) {
    newX -= TILE_SIZE;
    }
    if (key == GLUT_KEY_RIGHT) {
    newX += TILE_SIZE;}
    if (key == GLUT_KEY_UP) {
    newY += TILE_SIZE;
    }
    if (key == GLUT_KEY_DOWN)  {
    newY -= TILE_SIZE;
    
}

    
    if (newX < 0 ||newX >= BOARD_WIDTH || newY < 0 || newY >= BOARD_HEIGHT){
    return;
    }

     row = newY / TILE_SIZE;
     col = newX / TILE_SIZE;

    // Must be a road
    if (BoardGrid[row][col] != 1) return;

    // Check obstacles
    for (int i = 0; i < game->board.getObstacleCount(); i++) {
    
         ox = game->board.getObstacle(i)->getX();
         oy = game->board.getObstacle(i)->getY();
         
         
        if (ox == col * TILE_SIZE && oy == row * TILE_SIZE) {
        
            if (p->getRole() == Player::TAXI)   {
            p->setScore(p->getScore() - 2);
            game->flashFrames = 4;  

                Mix_PlayChannel(-1, collisionSound, 0);
}
            
            
            else if (p->getRole() == Player::DELIVERY){
            p->setScore(p->getScore() - 4);
            game->flashFrames = 4; 

                Mix_PlayChannel(-1, collisionSound, 0);
}
            if (p->getScore() < 0) game->gameOver = true;
            return;
            
        }
    
    
  }
int cx , cy;

    for (int i = 0; i < game->board.getCarCount(); i++) {
    
        cy = game->board.getCar(i)->getY() / TILE_SIZE;
        cx = game->board.getCar(i)->getX() / TILE_SIZE;
        
        
        
        if (cx == col && cy == row) {
        
            if (p->getRole() == Player::TAXI) {
            p->setScore(p->getScore() - 3);
            game->flashFrames = 4;  
                Mix_PlayChannel(-1, collisionSound, 0);

            }
            else if (p->getRole() == Player::DELIVERY) {
            p->setScore(p->getScore() - 5);
            game->flashFrames = 4;  
                Mix_PlayChannel(-1, collisionSound, 0);
}
            
            if (p->getScore() < 0) {
              game->gameOver = true;
            }
            return;
            
       
       
       }
    }

     p->setY(newY);
    p->setX(newX);
    p->setFuel(p->getFuel() - FUEL_COST);
    
    
}



bool collides(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) < TILE_SIZE && abs(y1 - y2) < TILE_SIZE;
}


/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */
void PrintableKeys(unsigned char key, int x, int y) {
    Player* p = game->board.getPlayer();
    Board& b = game->board;

    int px , fuel , score , wallet , jobsDone , py;
    
    
    
    if (game->selectingTheme) {
    if (key == 'w' || key == 'W') {
        game->themeSelection = (game->themeSelection + 2) % 3;
    } else if (key == 's' || key == 'S') {
        game->themeSelection = (game->themeSelection + 1) % 3;
    } else if (key == 13) {
        currentTheme = game->themeSelection;  // Save selected theme
        game->selectingTheme = false;
    }
    return;
}

    
    
    
    
    if (game->confirmingRoleSwitch) {
        if (key == 13) {  // ENTER
            px = p->getX();
            py = p->getY();
            fuel = p->getFuel();
             jobsDone = p->getJobsDone();
             score = p->getScore();
             wallet = p->getWallet();

            Player* newPlayer = (p->getRole() == Player::TAXI) ? static_cast<Player*>(new DeliveryDriver()): static_cast<Player*>(new TaxiDriver());

            newPlayer->setY(py);

            newPlayer->setX(px);
            
            newPlayer->setFuel(fuel);
            newPlayer->setWallet(wallet);
            newPlayer->setJobsDone(jobsDone);
            newPlayer->setScore(score);
            

            game->board.setPlayer(newPlayer);
            game->confirmingRoleSwitch = false;
            
            
        }
        
        
        else if (key == 'x' || key == 'X') {
            game->confirmingRoleSwitch = false;
        
        }
        
        
        return;
    }

   
    if (game->selectingRole) {
    
    
        if (key == '1'){
        game->board.setPlayer(new TaxiDriver());
        }
        
        else if (key == '2') {
        game->board.setPlayer(new DeliveryDriver());
        }
        
        
        else if (key == '3') {
        
            if (rand() % 2) {
            game->board.setPlayer(new TaxiDriver());
            }
            else {
            game->board.setPlayer(new DeliveryDriver());
        
        
        }}
        
        
        game->selectingRole = false;
        
        game->enteringName = true;
        return;
        
        
    }

   
   
   
   
   
    if (game->enteringName) {
        if (key == 13){
        game->enteringName = false;
        }
        
        else if ((key == 8 || key == 127) && game->nameLength > 0) {
            game->nameLength--;
            
            game->playerName[game->nameLength] = '\0';
          }
        
        else if (key >= 32 && key <= 126 && game->nameLength < 29) {
            game->playerName[game->nameLength++] = key;
            game->playerName[game->nameLength] = '\0';
          }
          
          
            return;
  }

   
   
  int need , afford, refillAmount , actualRefill;
  
    if (game->refueling) {
    
        if (key >= '0' && key <= '9' && game->fuelInputLength < 4) {
            game->fuelInput[game->fuelInputLength++] = key;
            game->fuelInput[game->fuelInputLength] = '\0';
        }
        
        
        else if ((key == 8 || key == 127) && game->fuelInputLength > 0) {
            game->fuelInputLength--;
            game->fuelInput[game->fuelInputLength] = '\0';
        }
        
        else if (key == 13) {
             refillAmount = 0;
            for (int i = 0; i < game->fuelInputLength; i++){
                refillAmount = refillAmount * 10 + (game->fuelInput[i] - '0');

}

             need = MAX_FUEL - p->getFuel();
             afford = p->getWallet() / REFUEL_COST;

            if (refillAmount > afford) {
                game->refuelFailed = true;
                game->fuelInputLength = 0;
                game->fuelInput[0] = '\0';
                
                return;
            

}

            actualRefill = (refillAmount < need) ? refillAmount : need;
            p->setFuel(p->getFuel() + actualRefill);
            p->setWallet(p->getWallet() - actualRefill * REFUEL_COST);
            
            
            game->refueling = false;
            game->refuelFailed = false;
        
        }
        else if (key == 'x' || key == 'X') {
    game->refueling = false;
    game->refuelFailed = false;
    game->fuelInputLength = 0;
    game->fuelInput[0] = '\0';
}

        return;
       
       }

    
    
    
    if (game->viewingLeaderboard && (key == 'b' || key == 'B')) {
        game->viewingLeaderboard = false;
        return;
        
    }

    
    
    if (game->playerName[0] == '\0') {
        if (key == 'w' || key == 's'){
            game->menuSelection = (game->menuSelection + 1) % 2;
            }
            
        if (key == 13) {
            if (game->menuSelection == 0){
              game->selectingRole = true;
            }
            else{
            game->viewingLeaderboard = true;
        }}
        
        
        return;
     }

  
  
  
    if (game->gameOver && key == ' ') {
        game->resetGame();
        return;
    }

    if (game->gameOver || key == 27) {
    return;
    }
    

    // Pickup / Dropoff 
    if (key == ' ' || key == 's') {
    
        if (p->getRole() == Player::TAXI) {
            if (!p->getTargetPassenger()) {
            
                for (int i = 0; i < b.getPassengerCount(); i++) {
                
                    if (!b.getPassenger(i)->isPicked() &&collides(p->getX(), p->getY(), b.getPassenger(i)->getX(), b.getPassenger(i)->getY())) {
                        
                        b.getPassenger(i)->setPicked(true);
                        p->setTargetPassenger(b.getPassenger(i));
                        Mix_PlayChannel(-1, pickupSound, 0);
                        
                        break;
                        
                    }
                }
                
            } 
            
            else if (collides(p->getX(), p->getY(),p->getTargetPassenger()->getDestX(), p->getTargetPassenger()->getDestY())) {
                p->setWallet(p->getWallet() + p->getTargetPassenger()->getFare());
                p->setScore(p->getScore() + 10);
               
                Mix_PlayChannel(-1, dropoffSound, 0);
                if (game->popupCount < 10) {
    game->scorePopups[game->popupCount++] = new ScorePopup(p->getX(), p->getY(), 10); 
}


                for (int j = 0; j < b.getPassengerCount(); j++) {
                    if (b.getPassenger(j) == p->getTargetPassenger()) {
                        b.removePassenger(j);
                        break;
                        
                }
                }
                 p->setJobsDone(p->getJobsDone() + 1);


                p->setTargetPassenger(nullptr);
               
                if (p->getJobsDone() % 2 == 0 && b.getCarCount() < MAX_OTHER_CARS) {
                
                    Position pr = randomRoadPosition();
                    b.addCar(new OtherCar(pr.x, pr.y));

                    for (int ci = 0; ci < b.getCarCount(); ci++) {
                    
                        if (b.getCar(ci)->getDirY() > 0){
                           b.getCar(ci)->setDirY(b.getCar(ci)->getDirY() + 2);
                           }
                           
                        else if (b.getCar(ci)->getDirY() < 0){
                        b.getCar(ci)->setDirY(b.getCar(ci)->getDirY() - 2);}
                        
                        if (b.getCar(ci)->getDirX() > 0) {
                        b.getCar(ci)->setDirX(b.getCar(ci)->getDirX() + 2);
                        }
                        else if (b.getCar(ci)->getDirX() < 0){
                        b.getCar(ci)->setDirX(b.getCar(ci)->getDirX() - 2);
                        }
                        
                        
                        
                        
                 }
                }
               }
  } 
        
        else {
            if (!p->getTargetPackage()) {
            
                for (int i = 0; i < b.getPackageCount(); i++) {
                
                    if (!b.getPackage(i)->isPicked() && collides(p->getX(), p->getY(), b.getPackage(i)->getX(), b.getPackage(i)->getY())) {
                        b.getPackage(i)->setPicked(true);
                        p->setTargetPackage(b.getPackage(i));
                        Mix_PlayChannel(-1, pickupSound, 0);
                        break;
                     
                     
                     }
                
                }
            } 
            else if (collides(p->getX(), p->getY(), p->getTargetPackage()->getDestX(), p->getTargetPackage()->getDestY())) {
                p->setScore(p->getScore() + 20);
                p->setWallet(p->getWallet() + p->getTargetPackage()->getFare());
                
                Mix_PlayChannel(-1, dropoffSound, 0);
                if (game->popupCount < 10) {
    game->scorePopups[game->popupCount++] = new ScorePopup(p->getX(), p->getY(), 20);  
}


                for (int j = 0; j < b.getPackageCount(); j++) {
                    if (b.getPackage(j) == p->getTargetPackage()) {
                        b.removePackage(j);
                        break;
                       }
               
               }

                p->setTargetPackage(nullptr);
                p->setJobsDone(p->getJobsDone() + 1);

                if (p->getJobsDone() % 2 == 0 && b.getCarCount() < MAX_OTHER_CARS) {
                    Position pr = randomRoadPosition();
                    b.addCar(new OtherCar(pr.x, pr.y));

                    for (int ci = 0; ci < b.getCarCount(); ci++) {
                      
                      if (b.getCar(ci)->getDirX() > 0){
                        b.getCar(ci)->setDirX(b.getCar(ci)->getDirX() + 2);
                         }
                        else if (b.getCar(ci)->getDirX() < 0)
                         {
                        b.getCar(ci)->setDirX(b.getCar(ci)->getDirX() - 2);
                         }
                        
                        if (b.getCar(ci)->getDirY() > 0){
                        b.getCar(ci)->setDirY(b.getCar(ci)->getDirY() + 2);
                        }
                        
                        else if (b.getCar(ci)->getDirY() < 0){
                        b.getCar(ci)->setDirY(b.getCar(ci)->getDirY() - 2);
                        
                        }
               
               
               }
            }
            }
      }
      }



    // Refueling trigger 
    if (key == 'r' || key == 'R') {
    
        for (int i = 0; i < b.getFuelCount(); i++) {
            if (collides(p->getX(), p->getY(), b.getFuel(i)->getX(), b.getFuel(i)->getY())) {
                game->refueling = true;
                game->fuelInputLength = 0;
                game->fuelInput[0] = '\0';
                return;
                
          }
        
        
        }
    }
    

   
    if (p->getFuel() <= 0 && !game->gameOver) {
        game->gameOver = true;
        
        Mix_PlayChannel(-1, gameOverSound, 0);
        return;
    }

  
    if (key == 'p' || key == 'P') {
        if (game->gameOver) {
            game->resetGame();
            return;
            
        }
        
        if (p->getX() == 0 && p->getY() == 0) {
            
            game->confirmingRoleSwitch = true;
            return;
        
        }
    }

    
    
    
    
    
    
    
    
    
    
    glutPostRedisplay();


}





/*
 * This function is called after every 1000.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */
void Timer(int) {
    game->update(0.1);
    glutPostRedisplay();
    glutTimerFunc(100, Timer, 0);
}


void music(const string &filePath) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        cerr << "SDL Init Error: " << SDL_GetError() << endl;
        return;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "SDL_mixer Init Error: " << Mix_GetError() << endl;
        return;
    }

    backgroundMusic = Mix_LoadMUS(filePath.c_str());
    if (!backgroundMusic) {
        cerr << "Failed to load: " << filePath << ". Error: " << Mix_GetError() << endl;
    } 
    
    else {
        Mix_PlayMusic(backgroundMusic, -1);
        Mix_VolumeMusic(70);
    }

    pickupSound = Mix_LoadWAV("pickup.mp3");
    dropoffSound = Mix_LoadWAV("dropoff.mp3");
    collisionSound = Mix_LoadWAV("collision.mp3");
    gameOverSound = Mix_LoadWAV("gameover.mp3");

    if (!pickupSound || !dropoffSound || !collisionSound || !gameOverSound) {
        cerr << "Error loading sound effects: " << Mix_GetError() << endl;
        
        
    }
}

void cleanupAudio() {
    if (pickupSound){
    
    Mix_FreeChunk(pickupSound);
    }
    if (dropoffSound){
    Mix_FreeChunk(dropoffSound);
    }
    if (collisionSound){
    Mix_FreeChunk(collisionSound);}
    if (gameOverSound){
    Mix_FreeChunk(gameOverSound);
    }
    if (backgroundMusic){
    Mix_FreeMusic(backgroundMusic);
    }
    
    
    Mix_CloseAudio();
    SDL_Quit();
}
/*This function is called (automatically) whenever your mouse moves witin inside the game window
 *
 * You will have to add the necessary code here for finding the direction of shooting
 *
 * This function has two arguments: x & y that tells the coordinate of current position of move mouse
 *
 * */
void MousePressedAndMoved(int x, int y) {
	//cout << x << " " << y << endl;
	glutPostRedisplay();
}
void MouseMoved(int x, int y) {
	//cout << x << " " << y << endl;
	glutPostRedisplay();
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
 *
 * You will have to add the necessary code here for shooting, etc.
 *
 * This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
 * x & y that tells the coordinate of current position of move mouse
 *
 * */
void MouseClicked(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
			{
		//cout << GLUT_DOWN << " " << GLUT_UP << endl;

	} else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
			{
			//cout<<"Right Button Pressed"<<endl;

	}
	glutPostRedisplay();
}





void SetCanvasSize(int w, int h);
void GameDisplay();
void Timer(int);
void NonPrintableKeys(int key, int x, int y);
void PrintableKeys(unsigned char key, int x, int y);




/*
 * our gateway main function
 * */


int main(int argc, char* argv[]) {
    srand(time(0));
    game = new Game();

    music("music.mp3");    
    
	//int width = 1020, height = 840; // i have set my window size to be 800 x 600

	InitRandomizer(); // seed the random number generator...
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	 glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // set the size of our window
	glutCreateWindow("OOP Project"); // set the title of our game window
	SetCanvasSize(WINDOW_WIDTH, WINDOW_HEIGHT); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	//glutDisplayFunc(display); // tell library which function to call for drawing Canvas.

	glutDisplayFunc(GameDisplay); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0, Timer, 0);

	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse
	glutMotionFunc(MousePressedAndMoved); // Mouse

	// now handle the control to library and it will call our registered functions when
	// it deems necessary...
	glutMainLoop();
	
        cleanupAudio();
          delete game;

          return 0;
          
    
    
    
    
}
#endif // RushHour_CPP_

