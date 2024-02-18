#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <Windows.h>
#include <algorithm>

struct rule {
	std::string qstate;																		// dabartine busena
	std::string csymbol;																	// dabartinis simbolis
	std::string nsymbol;																	// naujasis simbolis
	std::string direction;																	// i kuria puse eis galvute
	std::string nstate;																		// naujoji busena
};

void printingText(std::vector<std::string> tape, std::string state, int position, long long steps, int cursorPos);
void outsideLoop(std::vector<rule> rules, std::vector<std::string> tape, std::string state, int position, int& cursorPos);
void fileReading(int& tapeCount, int& position, std::vector<rule>& rules, std::vector<std::string>& tape);
void infiniteTape(int& position, std::vector<std::string>& tape);
void clearScreen();


int main()
{
	int tapeCount;
	int position = 0;
	long long steps = 0;
	std::vector <rule> rules;
	std::vector <std::string> tape;
	fileReading(tapeCount, position, rules, tape);
	std::string tempSymbol;																	// laikino simbolio saugojimui, kad galetume patikrinti galutine busena ar nera taisykles

	position--;																				// input nuo 1 iki inf, o masyvas nuo 0 iki inf, tam ir sumazinu
	int cursorPos = 0;																		// saugosim vieta, kurioje vietoje prasideda taisykles su reikiama busena
	std::string state = "0";																// saugosim busena, kad zinotume, kokioje busenoje siuo metu esame
	
	while (true) {
		if (state == rules[cursorPos].qstate) {
			if (tape[position] == rules[cursorPos].csymbol) {
				tape[position] = rules[cursorPos].nsymbol;
				if (rules[cursorPos].direction == "L") {
					steps++;
					position--;
					infiniteTape(position, tape);
					if (rules[cursorPos].nstate != state) {
						state = rules[cursorPos].nstate;
					}
				}
				else if (rules[cursorPos].direction == "R") {
					steps++;
					position++;
					infiniteTape(position, tape);
					if (rules[cursorPos].nstate != state) {
						state = rules[cursorPos].nstate;
					}
				}
			}
			else if (tape[position] != rules[cursorPos].csymbol) {
				outsideLoop(rules, tape, state, position, cursorPos);
			}
		}
		else if (rules[cursorPos].qstate != state) {
			outsideLoop(rules, tape, state, position, cursorPos);
		}
		printingText(tape, state, position, steps, cursorPos);

		// Tikrinam ar pasiekė galutinė būseną

		auto end = std::find_if(rules.begin(), rules.end(), [state](const rule& d) {									
			return d.qstate == state;
			});
		if (rules.end() == end) {
			std::cout << "\n\nHalted.\n";
			break;
		}

		// Baigiam tikrinimą

		// Tikrinam ar tiesiog nėra taisyklės simboliui

		tempSymbol = tape[position];

		auto endSymbol = std::find_if(rules.begin(), rules.end(), [tempSymbol](const rule& d) {
			return d.csymbol == tempSymbol;
			});
		if (rules.end() == endSymbol) {
			std::cout << "\n\nHalted. No rule for state '" << state << "' and symbol '" << tape[position] << "'.\n";
			break;
		}

		// Baigiam tikrinimą

		if (steps >= 2000 && rules.end() != endSymbol && rules.end() != end) {
			std::cout << "\nIt seems this might be an infinite loop, as there is no final state and there's always a rule for a symbol. Click <ENTER> if you want to end it";
			if (GetKeyState(VK_END) > 0) {
				break;
			}
		}

		// std::cout << " " << cursorPos << " " << position << " " << state << " " << rules[cursorPos].qstate << " " << tempSymbol;				// Eilute naudojama klaidu paieskai
		clearScreen();
	}
}

void printingText(std::vector<std::string> tape, std::string state, int position, long long steps, int cursorPos) {

	std::cout << "Head position can be seen with '' symbols\n\n";

	for (int i = 0; i < tape.size(); i++) {
		if (i == position) {
			std::cout << "'" << tape[i] << "'";
		}
		else {
			std::cout << tape[i];
		}
	}

	std::cout << "\n\nSteps: " << steps;
	std::cout << "\nCurrent state: " << state;
}

void outsideLoop(std::vector<rule> rules, std::vector<std::string> tape, std::string state, int position, int& cursorPos) {
	for (int i = 0; i < rules.size(); i++) {
		if (rules[i].qstate == state) {
			if (rules[i].csymbol == tape[position]) {
				cursorPos = i;
			}
		}
	}
}

void fileReading(int& tapeCount, int& position, std::vector<rule>& rules, std::vector<std::string>& tape) {
	std::string fileName;
	rule tempRules;
	std::string tempTape;

	
	std::cout << "Files: 1.txt, 2.txt, 3.txt, 4.txt\n";
	std::cout << "Choose which file you want to open: ";
	std::cin >> fileName;

	clearScreen();

	while (true) {

		std::ifstream file(fileName.c_str());

		if (!file.is_open()) {
			std::cout << "File is either corrupted or doesn't exist, try again.\n";
			std::cout << "Your file options are: 1.txt, 2.txt, 3.txt, 4.txt.\n";
			std::cout << "Type in the name of the file: ";
			std::cin >> fileName;
		}
		else {
			std::cout << "File " << fileName << " was opened succesfully\n";
			std::cout << "Reading...";
			file >> tapeCount >> tempTape >> position;

			for (int i = 0; i < tempTape.size(); i++)
			{
				tape.push_back(tempTape.substr(i, 1));
			}

			while (file >> tempRules.qstate) {
				file >> tempRules.csymbol;
				file >> tempRules.nsymbol;
				file >> tempRules.direction;
				file >> tempRules.nstate;
				rules.push_back(tempRules);
			}
			Sleep(1200);
			clearScreen();
			std::cout << "Tape count:  " << tapeCount << std::endl;
			std::cout << "Startning head position: " << position << std::endl;
			std::cout << "Input: ";
			for (int i = 0; i < tape.size(); i++) {
				std::cout << tape[i];
			}
			file.close();
			Sleep(1500);
			break;
		}
	}
}

void infiniteTape(int& position, std::vector<std::string>& tape) {
	if (position > tape.size()) {
		tape.push_back(" ");
	}
	if (position < 0) {
		position = 0;
		tape.insert(tape.begin(), " ");
	}
}

void clearScreen() {
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR)' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	SetConsoleCursorPosition(hStdOut, homeCoords);
}