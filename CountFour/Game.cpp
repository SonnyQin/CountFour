#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include "SpriteComponent.h"
#include <vector>

#include "BlackChessPiece.h"
#include "WhiteChessPiece.h"

Game::Game()
	:mWindow(nullptr),
	mRenderer(nullptr),
	mIsRunning(true),
	mUpdatingActor(false),
	mTickCounts(0),
	mIsAIplaying(true),
	mIsDropped(false),
	mIsPlayerOver(true),
	mIsAIOver(true),
	count(false)
{
}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Initializing failed!");
		return false;
	}

	mWindow = SDL_CreateWindow("ConnectFour", 100, 100, 576, 672, 0);
	if (!mWindow)
	{
		SDL_Log("Unable to create window!");
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		SDL_Log("Unable to create renderer!");
		return false;
	}

	LoadData();

	mTickCounts = SDL_GetTicks();

	return true;
}

void Game::Shutdown()
{
	UnLoadData();
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::AddActor(Actor* actor)
{
	if (mUpdatingActor)
		mPendingActors.emplace_back(actor);
	else
		mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
	iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite)
{
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); iter++)
	{
		if ((*iter)->GetDrawOrder() < myDrawOrder)
			break;
	}
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	if (iter != mSprites.end())
		mSprites.erase(iter);
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s", fileName);
			return nullptr;
		}
	}
	return tex;
}

SDL_Rect MousePosition;
struct GTNode;
struct GameState;

const int Game::AlphaBetaDecide(GTNode* root, int maxDepth)
{
	int choice = 1;
	float maxValue = -std::numeric_limits<float>::infinity();
	float beta = std::numeric_limits<float>::infinity();
	AddChildren(root,false);
	for (auto child : root->mChildren)
	{
		float v = AlphaBetaMax(child, maxDepth, maxValue, beta);
		if (v > maxValue)
		{
			maxValue = v;
			choice = child->choice;
		}
	}
	return choice;
}

bool Game::IsTerminal(GTNode* root)
{
	if (root->mChildren.empty())
		return true;
	return false;
}

void Game::AddChildren(GTNode* root,bool color)
{
	for (int i = 1; i <= 6; i++)
	{
		GTNode* node = new GTNode;
		for (int row = 1; row <= 6; row++)
		{
			for (int line = 1; line <= 7; line++)
			{
				node->mState.mBoard[row][line] = root->mState.mBoard[row][line];
			}
		}
		if (!DownToBottom(node->mState.mBoard, i, color))
		{
			delete node;
			continue;
		}
			
		node->choice = i;
		root->mChildren.emplace_back(node);
	}//Add children
}

float Game::AlphaBetaMax(GTNode* root, int depth, float alpha, float beta)
{
	if (Check(root->mState.mBoard) == GameState::Chessstate::EBlack)
		return -Math::Infinity;
	else
		if (Check(root->mState.mBoard) == GameState::Chessstate::EWhite)
			return Math::Infinity;
	AddChildren(root,true);
	if (depth == 0||IsTerminal(root))
	{
		return heuristic(root->mState);
	}
	float maxValue = -std::numeric_limits<float>::infinity();
	for (auto child : root->mChildren)
	{
		maxValue = std::max(maxValue, AlphaBetaMin(child, depth - 1, maxValue, beta));
		if (maxValue >= beta)
		{
			return maxValue;
		}
		alpha = std::max(maxValue, alpha);
	}
	return maxValue;
}

float Game::AlphaBetaMin(GTNode* root, int depth, float alpha, float beta)
{
	if (Check(root->mState.mBoard) == GameState::Chessstate::EBlack)
		return -Math::Infinity;
	else
		if (Check(root->mState.mBoard) == GameState::Chessstate::EWhite)
			return Math::Infinity;
	AddChildren(root,false);
	if (depth == 0||IsTerminal(root))
	{
		return heuristic(root->mState);
	}
	float minValue = std::numeric_limits<float>::infinity();
	for (auto child : root->mChildren)
	{
		minValue = std::min(minValue, AlphaBetaMax(child, depth - 1, minValue, beta));
		if (minValue <= alpha)
		{
			return minValue;
		}
		alpha = std::min(minValue, alpha);
	}
	return minValue;
}

int Game::DownToBottom(GameState::Chessstate board[7][8], int x, bool color)
{
	int i = 7;
	bool IsFind = false;
	for (; i >= 1; i--)
	{
		if (board[x][i] == GameState::EUnropped)
		{
			IsFind = true;
			board[x][i] = color ? GameState::EBlack : GameState::EWhite;
			break;
		}
	}
	return IsFind ? i : false;//有可行的位置返还位置，返还false
}

int Game::Check(GameState::Chessstate a[7][8])
{
	int cx[] = {1,1,0,-1};
	int cy[] = {0,1,1,0};

	for (int i = 1; i <= 6; i++)
	{
		for (int j = 1; j <= 7; j++)
		{
			if (a[i][j] != GameState::Chessstate::EUnropped)
			{
				for (int direction = 0; direction < 4; direction++)
				{
					bool isFind = true;
					int x=i,y=j;
					for (int k = 1; k <= 3; k++)
					{
						x += cx[direction];
						y += cy[direction];
						if (!IsOut(x,y))
						{
							if (a[x][y] != a[i][j])
							{
								isFind = false;
								break;
							}
						}
						else
						{
							isFind = false;
							break;
						}
					}
					if (isFind)
					{
						return a[i][j];
					}
				}
			}
		}
	}
	return -1;
}

int Game::Pressing(GameState::Chessstate a[7][8])
{
	return 0;
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	int x, y;
	const Uint32 mouseState = SDL_GetMouseState(&x, &y);
	int choice;
	mIsAIplaying = mIsAIplaying ? false : true;
	if (!mIsAIplaying)
		if (0 <= x && x < 96)
		{
			MousePosition.x = 0;
			MousePosition.y = 0;
			MousePosition.w = 96;
			MousePosition.h = 672;
			choice = 1;
		}
		else
			if (96 <= x && x < 192)
			{
				MousePosition.x = 96;
				MousePosition.y = 0;
				MousePosition.w = 96;
				MousePosition.h = 672;
				choice = 2;
			}
			else
				if (192 <= x && x < 288)
				{
					MousePosition.x = 192;
					MousePosition.y = 0;
					MousePosition.w = 96;
					MousePosition.h = 672;
					choice = 3;
				}
				else
					if (288 <= x && x < 384)
					{
						MousePosition.x = 288;
						MousePosition.y = 0;
						MousePosition.w = 96;
						MousePosition.h = 672;
						choice = 4;
					}
					else
						if (384 <= x && x < 480)
						{
							MousePosition.x = 384;
							MousePosition.y = 0;
							MousePosition.w = 96;
							MousePosition.h = 672;
							choice = 5;
						}
						else
							if (480 <= x && x < 576)
							{
								MousePosition.x = 480;
								MousePosition.y = 0;
								MousePosition.w = 96;
								MousePosition.h = 672;
								choice = 6;
							}

	Vector2 ChessPieceSpwanPositon;
	ChessPieceSpwanPositon.y = 0;
	if (SDL_BUTTON(mouseState) & SDL_BUTTON_LEFT&&!mIsAIplaying&&mIsAIOver&&mIsPlayerOver&&!count)
	{
		int result = DownToBottom(mBoard, choice, true);
		if (result)
		{
			ChessPieceSpwanPositon.x = MousePosition.x;
			BlackChessPiece* bcp = new BlackChessPiece(this, ChessPieceSpwanPositon);
			mIsAIplaying = true;
			Vector2 finalPosition;
			finalPosition.x = (choice - 1) * 96;
			finalPosition.y = (result - 1) * 96;
			bcp->SetFinalPosition(finalPosition);
			SetPlayerBegin();
			count = true;
			mIsDropped = true;
		}
	}
}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTickCounts + 16))
		;
	float deltaTime = mTickCounts / 1000.0f;
	if (deltaTime > 0.05f)
		deltaTime = 0.05f;
	mTickCounts = SDL_GetTicks();
	if (mIsDropped&&mIsPlayerOver)
	{
		GTNode* Node = new GTNode;
		for (int i = 1; i <= 6; i++)
		{
			for (int j = 1; j <= 7; j++)
			{
				Node->mState.mBoard[i][j] = mBoard[i][j];
			}
		}
		int AIChoice;
		AIChoice = AlphaBetaDecide(Node, 6);
		Vector2 SpwanPosition;
		SpwanPosition.x = (AIChoice - 1) * 96;
		SpwanPosition.y = 0;
		WhiteChessPiece* wcp = new WhiteChessPiece(this, SpwanPosition);
		Vector2 FinalPosition;
		int result = DownToBottom(mBoard, AIChoice, false);
		FinalPosition.x = (AIChoice - 1) * 96;
		FinalPosition.y = (result - 1) * 96;
		wcp->SetFinalPosition(FinalPosition);
		count = false;
		mIsDropped = false;
		SetAIBegin();
		if (Check(mBoard) != -1)
		{
			mIsRunning = false;
			system("Pause");
		}
	}
	for (auto act : mActors)
	{
		act->Update(deltaTime);
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_RenderClear(mRenderer);
	SDL_SetRenderDrawColor(mRenderer, 211, 211, 211, 255);
	SDL_RenderFillRect(mRenderer, &MousePosition);
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}
	SDL_RenderPresent(mRenderer);
}

void Game::LoadData()
{
	for (int i = 1; i <= 6; i++)
	{
		for (int j = 1; j <= 7; j++)
		{
			mBoard[i][j] = GameState::EUnropped;
		}
	}
}

void Game::UnLoadData()
{
}

int Game::heuristic(struct GameState state)
{
	int result = Check(state.mBoard);
	if (result == GameState::Chessstate::EBlack)
		return -Math::Infinity;
	else
		if (result == GameState::Chessstate::EWhite)
			return Math::Infinity;

	int point = 0;
	for (int i = 1; i <= 6; i++)
	{
		for (int j = 1; j <= 7; j++)
		{
			if (state.mBoard[i][j]==GameState::EWhite)
			{
				if (i == 3 || i == 4&&j!=1&&j!=2)
					point += 2;
				else
					point += 1;
			}
		}
	}
	return point;
}

bool Game::IsOut(int x, int y)
{
	if (x <= 0 || x > 6 || y <= 0 || y > 7)
		return true;
	return false;
}
