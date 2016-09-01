#include "lazy_string.h"
#include <stdlib.h>
#include <vector>
#include <chrono>
#include <fstream>
#include <thread>


std::vector<lazy_string> vecLS;
std::vector<std::pair<std::string, std::chrono::high_resolution_clock::time_point> > coutVecString;

void createLazyString() {
	coutVecString.push_back(std::pair<std::string, std::chrono::high_resolution_clock::time_point>("create start", std::chrono::high_resolution_clock::now()));
	int len = rand() % 10 + 1;
	std::string tempString;
	for (int j = 0; j < len; ++j)
		tempString += char('a' + rand() % 26);
	coutVecString.push_back( std::pair<std::string, std::chrono::high_resolution_clock::time_point>("create "+tempString, std::chrono::high_resolution_clock::now() ) );
	vecLS.push_back(lazy_string(tempString));
}

void modifyLazyString() {
	coutVecString.push_back(std::pair<std::string, std::chrono::high_resolution_clock::time_point>("modify start", std::chrono::high_resolution_clock::now()));
	if (vecLS.size() != 0) {
		int indexVecLS = rand() % vecLS.size();
		coutVecString.push_back(std::pair<std::string, std::chrono::high_resolution_clock::time_point>("modify pre "+(std::string)(vecLS[indexVecLS]), std::chrono::high_resolution_clock::now()));
		vecLS[indexVecLS][rand() % vecLS[indexVecLS].size()] = char('a' + rand() % 26);
		coutVecString.push_back(std::pair<std::string, std::chrono::high_resolution_clock::time_point>("modify post " + (std::string)(vecLS[indexVecLS]), std::chrono::high_resolution_clock::now()));
	}
}

void copyLazyString() {
	coutVecString.push_back(std::pair<std::string, std::chrono::high_resolution_clock::time_point>("copy start", std::chrono::high_resolution_clock::now()));
	if (vecLS.size() != 0) {
		int indexVecLS = rand() % vecLS.size();
		int lastIndex = vecLS.size();
		vecLS.push_back(vecLS[indexVecLS]);
		coutVecString.push_back(std::pair<std::string, std::chrono::high_resolution_clock::time_point>((std::string)(vecLS[lastIndex]), std::chrono::high_resolution_clock::now()));
	}
}

std::vector<int> operations1Thread;
std::vector<int> operations2Thread;
std::ofstream fout;

void operations1Process() {
	for (int i = 0; i < operations1Thread.size(); i++) {
		fout << "1 process " << i << " " << operations1Thread[i] << std::endl;
		if (operations1Thread[i] == 1)
			createLazyString();
		else if (operations1Thread[i] == 2)
			modifyLazyString();
		else if (operations1Thread[i] == 3)
			copyLazyString();
	}
}

void operations2Process() {
	for (int i = 0; i < operations2Thread.size(); i++) {
		fout << "2 process " << i << " " << operations2Thread[i] << std::endl;
		if (operations2Thread[i] == 1)
			createLazyString();
		else if (operations2Thread[i] == 2)
			modifyLazyString();
		else if (operations2Thread[i] == 3)
			copyLazyString();
	}
}

int main(int argc, char const *argv[]) {
	try {
		srand(0);
		fout.open("output.txt");
		for (int i = 0; i < 170; i++) {
			operations1Thread.push_back(rand() % 3 + 1);
			operations2Thread.push_back(rand() % 3 + 1);
		}
		std::thread thread1(operations1Process);
		std::thread thread2(operations2Process);
		thread1.join();
		thread2.join();
	}
	catch(const std::exception& e) {
		std::cout << "Aaaaaaaaaaaa!" << std::endl;
		std::cout << e.what() << std::endl;
	}
	return 0;
}