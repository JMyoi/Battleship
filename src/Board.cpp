#include "Board.hpp"
#include "raylib.h"
#include "Ship.hpp"
#include <vector>
#include <iostream>
using namespace std;

bool Tile::sTexturesLoaded = false;
Texture2D Tile::sImpact = {};
Texture2D Tile::sMissSplash = {};
//for images that need to be cropped 
static Texture2D LoadCropped(const char* path, Rectangle crop, int w, int h) {
    Image img = LoadImage(path);
    ImageCrop(&img, crop);
    ImageResize(&img, w, h);
    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);
    return tex;
}
void Tile::LoadSharedTexturesOnce() {
    if (sTexturesLoaded) return;

    sImpact = LoadCropped("src/assets/Poof.png", {300, 80, 170, 170}, 50, 50);
    sMissSplash = LoadCropped("src/assets/drip.png", {570, 400, 340, 270}, 50, 50);
    sTexturesLoaded = true;
}

void Tile::UnloadSharedTextures() {
    if (!sTexturesLoaded) return;

    UnloadTexture(sImpact);
    UnloadTexture(sMissSplash);
    sImpact = {};
    sMissSplash = {};
    sTexturesLoaded = false;
}
// ===== End shared textures =====

//Tile Implementation

Tile::Tile(){
    rect = {0,0,0,0}; 
    state = TileState::Empty;

    LoadSharedTexturesOnce();
    
}


void Tile::Draw(){
    DrawRectangleLinesEx(rect, 1, BLACK );
}


void Tile::drawHitsAndMiss(){
    // based on the state, display different sprites, impact for hit, drip for miss, nothing for miss.
    switch (state){
        case TileState::Hit:{
            DrawTextureV(sImpact, {rect.x, rect.y}, WHITE);
                break;
        }
        case TileState::Miss:{
            DrawTextureV(sMissSplash, {rect.x, rect.y}, WHITE);
            break;
        }
        case TileState::Empty:
        case TileState::Ship:
            break;
    }
}


bool Tile::isClicked(){
    if(CheckCollisionPointRec(GetMousePosition(), rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        return true;
    return false;
}


//Board Implementation


//constructor
Board::Board(){
    grid.resize(10);
    for(int i = 0; i < 10; i++){
        grid.at(i).resize(10);
    }

    //render background ocean
    Image img = LoadImage("src/assets/Ocean.png");
    ImageResize(&img, 500, 500);
    Ocean = LoadTextureFromImage(img);
    UnloadImage(img);

    // load explosion sprite with animation info
    Explosion = LoadTexture("src/assets/Explosion.png");
    explosionFrameCount = 7;
    explosionPosition = {0, 0};
    //offset 2 width 12 pixels tot he right because animation was leaking to the left
    explosionFrameRec = {0, 0, static_cast<float>(Explosion.width/explosionFrameCount)-12, 
    static_cast<float>(Explosion.height)};
    explosionCurrentFrame = 0;
    explosionFrameCounter = 0;
    explosionFramesSpeed = 10;
    explosionFramesSpeed = 10;
    explosionActive = false;
        //load miss splash 
        //load miss splash 
    MissSplashAnimation = LoadTexture("src/assets/poofDrip.png");
    SetTextureFilter(MissSplashAnimation, TEXTURE_FILTER_POINT);
    missSplashFrameCount = 6;
    missSplashFrames[0] = {0, 0, 200, static_cast<float>(MissSplashAnimation.height)};
    missSplashFrames[1] = {200, 0, 340, static_cast<float>(MissSplashAnimation.height)};
    missSplashFrames[2] = {500, 0, 340, static_cast<float>(MissSplashAnimation.height)};
    missSplashFrames[3] = {850, 0, 340, static_cast<float>(MissSplashAnimation.height)};
    missSplashFrames[4] = {1220, 0, 340, static_cast<float>(MissSplashAnimation.height)};
    missSplashFrames[5] = {1600, 0, 400, static_cast<float>(MissSplashAnimation.height)};
    missSplashFrameRec = missSplashFrames[0];
    missSplashPosition = {0, 0};
    missSplashCurrentFrame = 0;
    missSplashFrameCounter = 0;
    missSplashFramesSpeed = 14;
    missSplashFramesSpeed = 14;
    missSplashActive = false;

    //Load sounds
    MissSplash = LoadSound("src/assets/missSplash.mp3");
    HitBoom = LoadSound("src/assets/HitExplosion.mp3");

    //Load sounds
    MissSplash = LoadSound("src/assets/missSplash.mp3");
    HitBoom = LoadSound("src/assets/HitExplosion.mp3");
}

void Board::UpdateAnimations(){
    if(explosionActive){
        explosionFrameCounter++;
        if(explosionFrameCounter >= (60/explosionFramesSpeed)){
            explosionFrameCounter = 0;
            explosionCurrentFrame++;

            if(explosionCurrentFrame >= explosionFrameCount){
                explosionCurrentFrame = 0;
                explosionFrameRec.x = 0;
                explosionActive = false;
            }
            else{
                explosionFrameRec.x = static_cast<float>(explosionCurrentFrame) * (static_cast<float>(Explosion.width/explosionFrameCount)-12);
            }
        }
    }

    if(missSplashActive){
        missSplashFrameCounter++;
        if(missSplashFrameCounter >= (60/missSplashFramesSpeed)){
            missSplashFrameCounter = 0;
            missSplashCurrentFrame++;

            if(missSplashCurrentFrame >= missSplashFrameCount){
                missSplashCurrentFrame = 0;
                missSplashFrameRec = missSplashFrames[0];
                missSplashActive = false;
            }
            else{
                missSplashFrameRec = missSplashFrames[missSplashCurrentFrame];
            }
        }
    }
}

void Board::DrawExplosion(){
    if(explosionActive){
        DrawTextureRec(Explosion, explosionFrameRec, explosionPosition, WHITE);
    }
}

void Board::DrawMissSplashAnimation(){
    if(missSplashActive){
        const float splashScale = 0.4f;
        Rectangle destRec = {
            missSplashPosition.x,
            missSplashPosition.y,
            missSplashFrameRec.width * splashScale,
            missSplashFrameRec.height * splashScale
        };
        DrawTexturePro(MissSplashAnimation, missSplashFrameRec, destRec, {0, 0}, 0.0f, WHITE);
    }
}

bool Board::IsShotAnimationActive() const{
    return explosionActive || missSplashActive;
}

void Board::StartExplosionAtTile(int row, int col){
    const Tile& tile = grid.at(row).at(col);
    explosionPosition = {
        tile.rect.x + tile.rect.width/2.0f - explosionFrameRec.width/2.0f,
        tile.rect.y + tile.rect.height/2.0f - explosionFrameRec.height/2.0f
    };
    explosionCurrentFrame = 0;
    explosionFrameCounter = 0;
    explosionFrameRec.x = 0;
    explosionActive = true;
}

void Board::StartMissSplashAtTile(int row, int col){
    const Tile& tile = grid.at(row).at(col);
    const float splashScale = 0.4f;
    missSplashFrameRec = missSplashFrames[0];
    missSplashPosition = {
        tile.rect.x + tile.rect.width/2.0f - (missSplashFrameRec.width * splashScale)/2.0f,
        tile.rect.y + tile.rect.height/2.0f - (missSplashFrameRec.height * splashScale)/2.0f
    };
    missSplashCurrentFrame = 0;
    missSplashFrameCounter = 0;
    missSplashActive = true;
}


//draws the players board, 
void Board::Draw(Vector2 start){
    //background ocean
    DrawTextureV(Ocean,start,WHITE);
    float TileWidth = 50; // board width / 10 = 500 / 10 = 50
    float TileHeight = 50; // 50
    float TileX = start.x;  
    float TileY = start.y; 
    int fontSize = 20;
    float offsetTop = 30;
    float offsetLeft = 30;

    // Column labels: A-J (bottom)
    for (int col = 0; col < 10; col++) {
        char letter = 'A' + col;
        DrawText(TextFormat("%c", letter),
                (int)(start.x + col * TileWidth + 18),
                (int)(start.y + 10 * TileHeight + 10), // below the grid
                fontSize,
                BLACK);
    }   
    // Row labels: 1-10
    for (int row = 0; row < 10; row++) {
        DrawText(TextFormat("%d", row + 1),
                (int)(start.x - offsetLeft),
                (int)(start.y + row * TileHeight + TileHeight * 0.30f),
                fontSize,
                BLACK);
    }


        // assign all tiles appropriate width, height,  and position 
        for(int row = 0; row<10; row++){
            TileX = start.x;  // Reset X at the start of each row
            for(int col = 0; col<10; col++){
                grid.at(row).at(col).rect.width = TileWidth;
                grid.at(row).at(col).rect.height = TileHeight;
                grid.at(row).at(col).rect.x = TileX;
                grid.at(row).at(col).rect.y = TileY;
                grid.at(row).at(col).Draw();
                TileX += TileWidth;  // Move to next column
            
            }
            TileY += TileHeight;  // Move to next row
        }

}


void Board::DrawHitsAndMiss(Vector2 start){
    // assign all tiles appropriate width, height,  and position 
        for(int row = 0; row<10; row++){
            for(int col = 0; col<10; col++){
                grid.at(row).at(col).drawHitsAndMiss();
            }
        }

}

/* Trackingboard funciton calls this funciton,
should return true only if a tile is clicked
false otherwise
should check for clicks on the board and update result parameter accordingly
    if tileSate == Empty, return Miss
    if TileState == Ship, return Hit
    if TileState == Hit, return AlreadyFired
    if TileState == Miss, return AlreadyFired
should return the tile it has fired at if hit so players ship vector can update it's state.
*/ 
bool Board::HandleFire(ShotResult& result, position& at){
    for(int row = 0; row<10; row++){
        for(int col = 0; col<10; col++){
            if(grid.at(row).at(col).isClicked()){
                TileState thisTileState = grid.at(row).at(col).state;
                switch (thisTileState)
                {
                case TileState::Empty:
                    grid.at(row).at(col).state = TileState::Miss;// update tile state on board to hit.
                    result = ShotResult::Miss;
                    at.col = col;
                    at.row = row;
                    at.hit = false;
                    //play miss fire sound
                    PlaySound(MissSplash);
                    //play miss fire sound
                    PlaySound(MissSplash);
                    StartMissSplashAtTile(row, col);
                    return true;
                    break;
                case TileState::Ship:
                    grid.at(row).at(col).state = TileState::Hit;// update tile state on board to hit.
                    result = ShotResult::Hit;
                    at.col = col;
                    at.row = row;
                    at.hit = true;
                    PlaySound(HitBoom);
                    PlaySound(HitBoom);
                    StartExplosionAtTile(row, col);
                    return true;
                    break;
                case TileState::Hit:
                case TileState::Miss:
                    result = ShotResult::AlreadyFired;
                    at.col = col;
                    at.row = row;
                    at.hit = true;
                    return true;
                    break;
                }

            }
        }
    }
    return false;
}

//returns the positions where it was placed.
bool Board::HandlePlaceShip(int shipSize, vector<position>& newPositions, Direction direction, string& ErrorMessage){
    for(int row = 0; row<10; row++){
        for(int col = 0; col<10; col++){

            if(grid.at(row).at(col).isClicked()){
                //is this tile empty and there is no ship on it, then maybe we can place it.
                if(grid.at(row).at(col).state == TileState::Empty){
                    bool canPlace = true;
                    int currRow = row, currCol = col;
                    // detect if the other parts of the ship are out of bound, !empty, or has a ship already.
                    //for the respective direction, vertical or horizontal
                    if(direction == Direction::Vertical){
                        for(int i = 0; i<shipSize-1; i++){
                            // to access the vertical directions index, we do x+1, for shipSize-1 times.
                            currRow++; // for vertical placement col does not change, only X, row.
                            bool outOfBound = currRow>=10;
                            // check for not out of bound and tile is empty.
                            if((!outOfBound) && (grid.at(currRow).at(col).state == TileState::Empty)){
                                // all conditions met, canPlace remains true
                                }
                            else{
                                cout<<"ship cannot be placed, either out of bound or ship already there.\n";
                                ErrorMessage = "Cannot place, either out of bound or ship already there";
                                return canPlace = false; 
                            }
                        }
                        if(canPlace){
                            // place ship on board, update tile states to ship.
                            currRow = row; // reset X
                            for(int i = 0; i<shipSize; i++){
                                grid.at(currRow).at(col).state = TileState::Ship;
                                newPositions.at(i).row = currRow;
                                newPositions.at(i).col = col;
                                newPositions.at(i).hit = false;
                                cout<<"Ship is placed at: Row:"<<currRow<<", Col: "<<col<<endl;
                                currRow++;
                            }
                            return true;
                        }
                        // detect if ship trying to be placed is colliding with other ships that are already placed.
                    }
                    else if(direction == Direction::Horizontal){
                         for(int i = 0; i<shipSize-1; i++){
                            // to access the horizontal directions index, we do curCol+1, for shipSize-1 times.
                            currCol++; 
                            bool outOfBound = currCol>=10;
                            // check for not out of bound and tile is empty.
                            if((!outOfBound) && (grid.at(row).at(currCol).state == TileState::Empty)){
                                // all conditions met, canPlace remains true
                                }
                            else{
                                cout<<"ship cannot be placed, either out of bound or ship already there.\n";
                                ErrorMessage = "Cannot place, either out of bound or ship already there";

                                return canPlace = false; 
                            }
                        }
                        if(canPlace){
                            // place ship on board, update tile states to ship.
                            currCol = col; // reset X
                            for(int i = 0; i<shipSize; i++){
                                grid.at(row).at(currCol).state = TileState::Ship;
                                newPositions.at(i).row = row;
                                newPositions.at(i).col = currCol;
                                newPositions.at(i).hit = false;
                                cout<<"Ship is placed at: Row: "<<row<<" Col: "<<currCol<<endl;
                                currCol++;
                            }
                            return true;
                        }
                        
                    }

                }else{
                    cout<<"ship cannot be placed ship already there.\n";
                    ErrorMessage = "Cannot place, ship already there.";
                    return false;
                }
            }
        }
    }
    return false;
}