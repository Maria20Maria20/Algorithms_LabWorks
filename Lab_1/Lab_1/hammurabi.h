#pragma once

#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>


struct GameData {
	int year;
	int population;
	int bushels;
	int acres;
	int acre_price;
	double starving_statistic[10];
};

void GameLoop();

bool LaunchMainMenu();

void CreateNewGame();
bool LoadGame();
bool SaveGame();

void ShowFirstMessage();
void ShowMessage(int deaths, int newcomers, bool epidemic,
	int harvested_bushels_from_acre, int plant_acres,
	int rat_loss);

void AskExitGame();

bool UpdateGame();

void PrintStat(const std::string& description_begin, int value,
	const std::string& description_end);

void ShowGameScore();