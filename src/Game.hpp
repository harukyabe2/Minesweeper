#pragma once
#include <Siv3D.hpp>
#include "Board.hpp"

// Game state
enum class GameState
{
    isPlaying,
    isGameOver,
    isGameClear
};

class Game
{
    public:
        Game();

        bool Initialize();
        void RunLoop();
        void Shutdown();
    private:
        void ProcessInput();
        void UpdateGame();
        void GenerateOutput();
        void LoadData();

        // Game state
        GameState mState;
        // Camera
        Camera2D mCamera;
        // The game board
        Board mBoard;
        // Input state
        bool mLeftClicked;
        bool mRightClicked;
        // Click position
        Vec2 mClickPos;
};

/*
後で見返す用
Gameクラスはタイマーやクリア判定などゲーム進行管理
Cellクラスはマスの状態管理
BoardクラスはCellの二次元配列管理
*/