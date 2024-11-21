#include "hammurabi.h"

#include <string>
#include <iostream>
#include <algorithm>
#include <windows.h>

using namespace std;

constexpr char CSI[] = "\x1B[";

constexpr char RED_COLOR[] = "\x1B[31m";
constexpr char GREEN_COLOR[] = "\x1B[32m";
constexpr char YELLOW_COLOR[] = "\x1B[33m";
constexpr char BLUE_COLOR[] = "\x1B[34m";
constexpr char MAGENTA_COLOR[] = "\x1B[35m";
constexpr char CYAN_COLOR[] = "\x1B[36m";
constexpr char WHITE_COLOR[] = "\x1B[37m";

constexpr char RESET_COLOR[] = "\033[0m";

GameData g_gamestate;

void GameLoop() {
	SetConsoleOutputCP(CP_UTF8);

	srand(time(0));


	if (!LaunchMainMenu()) 
		return;

	ShowFirstMessage();

	while (g_gamestate.year <= 10) { 
		if (!UpdateGame()) { 
			break;
		}
		++g_gamestate.year; 
		SaveGame();
		AskExitGame();
	}
	if (g_gamestate.year == 11) 
		ShowGameScore();
}

inline void ClearConsole() {
	cout << CSI << "3J" << CSI << "H" << CSI << "J" << flush;
}

void ClearConsoleLastLines(int n) {
	cout << CSI << "3J" << CSI << n << "F" << CSI << "J" << flush;
}

inline void PrintColorText(const string& text, const char* color) { 
	cout << color << text << RESET_COLOR;
}

void PrintMenuOptions() {
	PrintColorText("N", CYAN_COLOR);
	PrintColorText(" Начать новую игру\n", MAGENTA_COLOR);
	PrintColorText("L", CYAN_COLOR);
	PrintColorText(" Загрузить игру\n", MAGENTA_COLOR);
	PrintColorText("E", CYAN_COLOR);
	PrintColorText(" Выйти\n", MAGENTA_COLOR);
}

bool LaunchMainMenu() {
	PrintColorText("Добро пожаловать в Королевство Сластей!\n\n", CYAN_COLOR);

	bool game_is_started = false;
	while (!game_is_started) {
		PrintMenuOptions();
		string choice;
		cin >> choice;

		if (choice == "L") {
			ClearConsole();
			if (!LoadGame()) {
				PrintColorText("Не удалось загрузить игру.\n", RED_COLOR);
			}
			else {
				game_is_started = true;
			}
		}
		else if (choice == "N") {
			CreateNewGame();
			game_is_started = true;
		}
		else if (choice == "E") {
			return false;
		}
		else {
			ClearConsoleLastLines(5);
			PrintColorText("Некорректный ввод - попробуй ещё раз.\n", RED_COLOR);
		}
	}

	return true;
}

void ShowFirstMessage() {
	ClearConsole();

	PrintColorText("Великая и Мудрая ", RESET_COLOR);
	PrintColorText("Принцесса Бубльгум", MAGENTA_COLOR);
	PrintColorText("!\n", RESET_COLOR);

	PrintColorText("Ваши ", RESET_COLOR);
	PrintColorText("бананорыцари", YELLOW_COLOR);
	PrintColorText(" пришли к вам с докладом.\n\n", RESET_COLOR);

	PrintStat("Начинается ", g_gamestate.year, " год вашего правления.\n");
	PrintStat("Население Королевства составляет ", g_gamestate.population, " сластей.\n");
	PrintStat("Запасы сахара составляют ", g_gamestate.bushels, " тонн.\n");
	PrintStat("В Вашей власти сейчас находится ", g_gamestate.acres, " акров земли.\n");
	PrintStat("Цена 1 акра земли сейчас ", g_gamestate.acre_price, " тонн сахара.\n");

	cout << "\n";
}

void ShowMessage(int deaths, int newcomers, bool epidemic,
	int harvested_bushels_from_acre, int plant_acres,
	int rat_loss) {

	ClearConsole();

	PrintColorText("Великая и Мудрая ", RESET_COLOR);
	PrintColorText("Принцесса Бубльгум", MAGENTA_COLOR);
	PrintColorText("!\n", RESET_COLOR);

	PrintColorText("Ваши ", RESET_COLOR);
	PrintColorText("бананорыцари", YELLOW_COLOR);
	PrintColorText(" пришли к вам с докладом.\n\n", RESET_COLOR);

	PrintStat("Закончился ", g_gamestate.year, " год вашего правления.\n");
	PrintStat("В наше Королевство прибыло ", newcomers, " новых сластей.\n");

	if (epidemic)
		PrintColorText("    Эпидемия плесени уничтожила половину населения.\n", RED_COLOR);

	PrintStat("Население Королевства составляет ", g_gamestate.population, " сластей.\n");
	PrintStat("Сласти смогли собрать в этом году ", harvested_bushels_from_acre * plant_acres, " тонн сахара.\n");
	PrintStat("Лимонята украли ", rat_loss, " тонн сахара.\n");

	PrintStat("Запасы сахара составляют ", g_gamestate.bushels, " тонн.\n");
	PrintStat("В Вашей власти сейчас находится ", g_gamestate.acres, " акров земли.\n");
	PrintStat("Цена 1 акра земли сейчас ", g_gamestate.acre_price, " тонн сахара.\n");

	cout << "\n";
}

bool SaveGame() {
	ofstream output_file("save_file.txt");
	if (!output_file) {
		return false;
	}

	output_file << g_gamestate.year << '\n'
		<< g_gamestate.population << '\n'
		<< g_gamestate.bushels << '\n'
		<< g_gamestate.acres << '\n'
		<< g_gamestate.acre_price << '\n';

	for (double stat : g_gamestate.starving_statistic) { 
		output_file << stat << '\n';
	}

	return true;
}

bool LoadGame() {
	ifstream input_file("save_file.txt"); 

	if (!input_file) {
		return false;
	}

	input_file >> g_gamestate.year
		>> g_gamestate.population
		>> g_gamestate.bushels
		>> g_gamestate.acres
		>> g_gamestate.acre_price;

	for (double& stat : g_gamestate.starving_statistic) { 
		input_file >> stat;
	}

	return true;
}


void CreateNewGame() {
	g_gamestate = { 1, 100, 2800, 1000, rand() % 10 + 17, { 0,0,0,0,0,0,0,0,0,0 } };
}

int AskPlayerInput(const string& question) {

	auto IsNumber = [](const string& s) 
		{
			return !s.empty() && find_if(s.begin(),
				s.end(), [](unsigned char c) 
				{
					return !isdigit(c); 
				}
			) == s.end();
		};

	string answer;
	while (true) {
		cout << question << " >" << CYAN_COLOR;
		cin >> answer;
		cout << RESET_COLOR;

		if (IsNumber(answer)) {
			break;
		}
		ClearConsoleLastLines(1);
		PrintColorText("Мы ничего не поняли... ", RED_COLOR);

	}
	return atoi(answer.c_str());; 
}

bool UpdateGame() {
	PrintColorText("Великая и Мудрая ", RESET_COLOR);
	PrintColorText("Принцесса Бубльгум", MAGENTA_COLOR);
	PrintColorText(" что вы прикажите своим поданным?\n", RESET_COLOR);

	int buy_acres;
	int sell_acres;
	int feed_bushels;
	int plant_acres;

	while (true) {

		buy_acres = AskPlayerInput("Сколько акров земли вы хотите купить?");
		sell_acres = AskPlayerInput("Сколько акров земли вы хотите продать?");
		feed_bushels = AskPlayerInput("Сколько сахара выделить для пропитания?");
		plant_acres = AskPlayerInput("Сколько акров вы хотите засыпать сахаром?");

		ClearConsoleLastLines(5);

		if ((buy_acres >= 0 && buy_acres * g_gamestate.acre_price <= g_gamestate.bushels)

			&& (sell_acres >= 0 && sell_acres <= g_gamestate.acres + buy_acres)

			&& (feed_bushels >= 0 && feed_bushels <= g_gamestate.bushels + (sell_acres - buy_acres) * g_gamestate.acre_price)

			&& (plant_acres >= 0 && plant_acres <= g_gamestate.acres + buy_acres - sell_acres
				&& plant_acres <= (g_gamestate.bushels + (sell_acres - buy_acres) * g_gamestate.acre_price) * 2
				&& plant_acres <= g_gamestate.population * 10)) {

			break;
		}
		else {
			PrintColorText("Наше Королевство не может позволить себе такой план.\n", RED_COLOR);
		}
	}

	g_gamestate.acres += buy_acres - sell_acres;
	g_gamestate.bushels += ((sell_acres - buy_acres) * g_gamestate.acre_price)
		- feed_bushels - plant_acres / 2;

	int harvested_bushels_from_acre = rand() % 6 + 1; 
	g_gamestate.bushels += harvested_bushels_from_acre * plant_acres; 
	int rat_loss = static_cast<int>(0.07 * g_gamestate.bushels * (rand() / double(RAND_MAX))); 
	g_gamestate.bushels -= rat_loss; 

	int previous_population = g_gamestate.population;
	g_gamestate.population = min(feed_bushels / 20, g_gamestate.population);  

	int deaths = previous_population - g_gamestate.population; 
	g_gamestate.starving_statistic[g_gamestate.year - 1] = deaths * 1. / previous_population; 

	if (deaths > 0.45 * previous_population) { 
		PrintColorText("Принцесса, Королество разорилось.\n", RED_COLOR);
		PrintColorText("Игра окончена.\n", RED_COLOR);
		return false;
	}
	int newcomers = max(0, min(50, deaths / 2 + (5 - harvested_bushels_from_acre / plant_acres) * g_gamestate.bushels / 600));
	g_gamestate.population += newcomers;

	bool epidemic = false;
	if (rand() / double(RAND_MAX) < 0.15) { 
		g_gamestate.population /= 2;
		epidemic = true;
	}

	g_gamestate.acre_price = rand() % 10 + 17; 

	ShowMessage(deaths, newcomers, epidemic, harvested_bushels_from_acre,
		plant_acres, rat_loss); 

	return true;
}

void ShowGameScore() { 
	double average_starvation = 0; 
	for (double stat : g_gamestate.starving_statistic) { 
		average_starvation += stat; 
	}
	average_starvation /= 10; 
	average_starvation *= 100;

	double acres_per_villager = static_cast<double>(g_gamestate.acres) / g_gamestate.population; 

	cout << "P = " << average_starvation << "% \n";
	cout << "L = " << acres_per_villager << "\n";

	if (average_starvation > 33 && acres_per_villager < 7) {
		PrintColorText("Жителям Королевства Сластей жилось с вами совсем несладко, пожалуй править вам больше не стоит.\n", RED_COLOR);
	}
	else if (average_starvation > 10 && acres_per_villager < 9) {
		PrintColorText("Сласти жили неплохо, но горьковатый привкус остался...\n", YELLOW_COLOR);
	}
	else if (average_starvation > 3 && acres_per_villager < 10) {
		PrintColorText("Королевства давно жило такой сладкой жизнью. А жизнь то налаживается...\n", GREEN_COLOR);
	}
	else {
		PrintColorText("Алгеабраично! Самая лучшая ", CYAN_COLOR);
		PrintColorText("Принцесса Бубльгум", MAGENTA_COLOR);
		PrintColorText(", вас запомнят навсегда!!!\n", CYAN_COLOR);
	}
}

void AskExitGame() {
	string choice = "\0";
	while (choice != "Y" && choice != "N") {
		PrintColorText("[ Выйти из игры? Y/N ]\n>", RED_COLOR);
		cin >> choice;
		choice = choice;

		if (choice == "Y") {
			PrintColorText("Выход из игры...\n", RESET_COLOR);
			exit(0);
		}
		else if (choice != "N") {
			PrintColorText("Некорректный ввод. Повторите попытку.\n", RED_COLOR);
		}
	}
	ClearConsoleLastLines(2);
}


void PrintStat(const string& description_begin, int value,
	const string& description_end) {
	cout << "  " << description_begin
		<< CYAN_COLOR << value << RESET_COLOR
		<< description_end;
};