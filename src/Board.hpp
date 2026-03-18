#pragma once
#include "raylib.h"
#include "Ship.hpp"
#include <string>
#include <vector>
using namespace std;

enum class TileState {Empty,Ship,Hit,Miss};
enum class ShotResult{Hit, Miss, AlreadyFired};

class Tile{
    public:
        // data members
        Rectangle rect; // has x, y, width, height
        TileState state;
        // member funcitons
        Tile();
        void Draw();
        void drawHitsAndMiss();// called by Board::DrawHitAndMiss()
        bool isClicked();

        // shared tile markers loaded once for all tiles
        static void LoadSharedTexturesOnce();
        static void UnloadSharedTextures();

    private:
        static bool sTexturesLoaded;
        static Texture2D sImpact;
        static Texture2D sMissSplash;
};

class Board{
    public:
        Board();
        void Draw(Vector2 start); 
        void DrawHitsAndMiss(Vector2 start); // because the ship renders over the hits form Draw(), we need a separate funciton to call after the ships are rendered.
        bool HandlePlaceShip(int shipSize, vector<position>& newPositions, Direction direction,string& ErrorMessage); // returns true if the ship is placed and false if not.
        bool PlaceShipAt(int row, int col, int shipSize, vector<position>& newPositions, Direction direction, string& ErrorMessage); // position-based placement (no click), used by AI
        bool HandleFire(ShotResult& result, position& at); // checks for a click on board and tries to verify hit or miss.
        bool FireAt(int row, int col, ShotResult& result, position& at); // programmatic fire (no click), used for AI turn
        //animation related funcitons
        void UpdateAnimations();
        void DrawExplosion();
        void DrawMissSplashAnimation();
        bool IsShotAnimationActive() const;
    private:
        vector<vector<Tile>> grid; // 10 X 10 2D grid of tiles

        Texture2D Ocean; // board background
        //animation related members
        Texture2D Explosion;
        Vector2 explosionPosition;
        Rectangle explosionFrameRec;
        int explosionCurrentFrame;
        int explosionFrameCounter;
        int explosionFramesSpeed;
        int explosionFrameCount;
        bool explosionActive;

        Texture2D MissSplashAnimation;
        Rectangle missSplashFrames[6];
        Rectangle missSplashFrameRec;
        Vector2 missSplashPosition;
        int missSplashCurrentFrame;
        int missSplashFrameCounter;
        int missSplashFramesSpeed;
        int missSplashFrameCount;
        bool missSplashActive;

        //firing Sounds for miss and hit
        Sound HitBoom;
        Sound MissSplash; // need to shorten beggining? or time animation slower?

        void StartExplosionAtTile(int row, int col);
        void StartMissSplashAtTile(int row, int col);

};