#include "Board.hpp"

Board::Board()
: mSafeCellCount(0)
, mIsGameOver(false)
{
}

void Board::Update(Vec2 clickPos, bool leftClicked, bool rightClicked)
{
    // Get the cell index from the click position
    Point pos = GetIndexFromPos(clickPos);
    if (pos.x < 0 || pos.x >= mCells.width() ||
        pos.y < 0 || pos.y >= mCells.height())
    {
        return;
    }

    // Open the cell on left click, flag the cell on right click
    if (leftClicked && !mCells[pos].GetIsOpened() && !mCells[pos].GetIsFlagged())
    {
        OpenCell(pos);
    }
    if (rightClicked)
    {
        mCells[pos].SetIsFlagged(!mCells[pos].GetIsFlagged());
    }
}

void Board::Draw() const
{
    const Size size = mCells.size();
    for (int32 y = 0; y < size.y; ++y)
    {
        for (int32 x = 0; x < size.x; ++x)
        {
            const Point pos = {25 + 50 * x - (50 * size.x) / 2, 25 + 50 * y - (50 * size.y) / 2};
            mCells[Point{x, y}].Draw(pos);
        }
    }
}

void Board::Reset()
{
	mCells.clear();
	mMines.clear();
	mSafeCellCount = 0;
	mIsGameOver = false;
}

void Board::CreateBoard(const Size& size, int32 mineCount)
{
    mCells = Grid<Cell>(size);
    mSafeCellCount = size.x * size.y - mineCount;
	// avoid reallocation
	mMines.reserve(mineCount);

    // Randomly place mines
    while (mineCount)
    {
        const Point pos = {Random(0, size.x - 1), Random(0, size.y - 1)};
        if (mCells[pos].GetMineCount() == 0)
        {
            mCells[pos].SetMineCount(-1);
			mMines.push_back(pos);
            --mineCount;
        }
    }

    // Calculate mine counts for each cell
    for (int32 y = 0; y < size.y; ++y)
    {
        for (int32 x = 0; x < size.x; ++x)
        {
            const Point pos = {x, y};
            mCells[pos].SetMineCount(GetMineCount(pos));
        }
    }
}

int32 Board::GetMineCount(const Point& pos)
{
    // If this cell is a mine, return -1
    if (mCells[pos].GetMineCount() == -1)
    {
        return -1;
    }
    
    int32 mineCount = 0;
    // Check all 8 neighbors
    for (const auto& offset : mOffsets)
    {
        const Point neighborPos = pos + offset;
        if (neighborPos.x < 0 || neighborPos.x >= mCells.width() ||
            neighborPos.y < 0 || neighborPos.y >= mCells.height())
        {
            continue;
        }
        if (mCells[neighborPos].GetMineCount() == -1)
        {
            ++mineCount;
        }
    }
    return mineCount;
}

Point Board::GetIndexFromPos(const Vec2& pos)
{
    Point idx;
    idx.x = static_cast<int>(Math::Floor((pos.x + (50 * mCells.width()) / 2.0) / 50.0));
    idx.y = static_cast<int>(Math::Floor((pos.y + (50 * mCells.height()) / 2.0) / 50.0));
    return idx;
}

void Board::OpenCell(const Point& pos)
{
	// If the position is out of bounds, or the cell is already opened or flagged, do nothing
    if (pos.x < 0 || pos.x >= mCells.width() ||
        pos.y < 0 || pos.y >= mCells.height() ||
        mCells[pos].GetIsOpened() ||
        mCells[pos].GetIsFlagged())
    {
        return;
    }
    mCells[pos].SetIsOpened(true);
    --mSafeCellCount;

	// If this cell has no mines around, open all its neighbors recursively
	// If this cell is a mine, set it as exploded and open all mines, then end the game
    if (mCells[pos].GetMineCount() == 0)
    {
        for (const auto& offset : mOffsets)
        {
            const Point neighborPos = pos + offset;
            OpenCell(neighborPos);
        }
    }
    else if (mCells[pos].GetMineCount() == -1)
    {
		mCells[pos].SetIsExploded(true);
		for (const auto& minePos : mMines)
		{
			if (minePos != pos)
			{
				mCells[minePos].SetIsOpened(true);
			}
		}
        mIsGameOver = true;
    }
}
