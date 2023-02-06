#pragma once
#include "SDL.h"
#include "Math.h"
#include "vector"
#include "string"
#include <unordered_map>

class Game
{
public:
	Game();
	bool Initialize();
	void Shutdown();
	void RunLoop();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	SDL_Texture* GetTexture(const std::string& fileName);

	struct GameState {
		enum Chessstate//record black or white chesspiece
		{
			EBlack,
			EWhite,
			EUnropped
		};
		Chessstate mBoard[7][8];
	};
	struct GTNode {
		std::vector<GTNode*> mChildren;
		int choice;
		GameState mState;
	};
	const int AlphaBetaDecide(GTNode* root, int maxDepth);
	bool IsTerminal(GTNode* root);
	void AddChildren(GTNode* root,bool color);
	float AlphaBetaMax(GTNode* root, int depth, float alpha, float beta); 
	float AlphaBetaMin(GTNode* root, int depth, float alpha, float beta);

	int DownToBottom(GameState::Chessstate board[7][8],int x, bool color);

	void SetAIOver() { mIsAIOver = true; }
	void SetAIBegin() { mIsAIOver = false; }

	void SetPlayerOver() { mIsPlayerOver = true; }
	void SetPlayerBegin() { mIsPlayerOver = false; }

	int Check(GameState::Chessstate a[7][8]);
	int Pressing(GameState::Chessstate a[7][8]);
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnLoadData();

	std::unordered_map<std::string, SDL_Texture*> mTextures;
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class SpriteComponent*> mSprites;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTickCounts;
	bool mIsRunning;
	bool mUpdatingActor;
	bool mIsAIplaying;
	bool mIsDropped;
	bool count;//check if double dropped
	int heuristic(struct GameState state);
	bool IsOut(int x, int y);
	bool mIsPlayerOver;
	bool mIsAIOver;
	GameState::Chessstate mBoard[7][8];
};

