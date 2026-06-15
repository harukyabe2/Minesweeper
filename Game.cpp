#include "Game.hpp"

Game::Game()
: mState(GameState::isPlaying)
, mCamera(Vec2{0, 0}, 1.0, CameraControl::None_)
, mBoard()
, mLeftClicked(false)
, mRightClicked(false)
, mClickPos(0, 0)
{

}

bool Game::Initialize()
{
    // Set up the window
	Scene::Resize(1280, 900);
	Scene::SetResizeMode(ResizeMode::Keep);
    Window::SetStyle(WindowStyle::Sizable);
    Window::Maximize();
    Window::SetTitle(U"Minesweeper Game");

    LoadData();

    return true;
}

void Game::RunLoop()
{
    while (System::Update())
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput()
{
    if (mState != GameState::isPlaying)
    {
        return;
    }

    // Set camera
    const auto t = mCamera.createTransformer();

    // Check for left mouse click
    mLeftClicked = MouseL.down();
    if (mLeftClicked)
    {
        mClickPos = Cursor::PosF();
    }
    // Check for right mouse click
    mRightClicked = MouseR.down();
    if (mRightClicked)
    {
        mClickPos = Cursor::PosF();
    }
}

void Game::UpdateGame()
{
    // Update the board
    mBoard.Update(mClickPos, mLeftClicked, mRightClicked);

    // Check for game over or clear
    if (mBoard.IsGameOver())
    {
        mState = GameState::isGameOver;
    }
    else if (mBoard.IsCleared())
    {
        mState = GameState::isGameClear;
    }
}

void Game::GenerateOutput()
{
    // Set background color
    Scene::SetBackground(Palette::Black);

    // Set camera
    const auto t = mCamera.createTransformer();

    // Draw game elements
    mBoard.Draw();

    if (mState == GameState::isGameOver)
    {
        FontAsset(U"Message")(U"GAME OVER").drawAt({0, 0}, Palette::Red);
    }
    else if (mState == GameState::isGameClear)
    {
        FontAsset(U"Message")(U"GAME CLEAR").drawAt({0, 0}, Palette::Yellow);
    }
}

void Game::LoadData()
{
    // Register font
    FontAsset::Register(U"Number", FontMethod::MSDF, 48, Typeface::Bold);
    FontAsset::Register(U"Message", FontMethod::MSDF, 48, Typeface::Bold);
    // Register texture
    TextureAsset::Register(U"Mine", U"💣"_emoji);
    TextureAsset::Register(U"Flag", U"🚩"_emoji);

    // Create a board
    mBoard.CreateBoard({24, 16}, 80);
}

void Game::Shutdown()
{
}
