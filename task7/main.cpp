#include "lazy_string.h"
#include <stdlib.h>
#include <vector>
#include <chrono>
#include <fstream>
#include <thread>


std::vector<lazy_string> vecLS;
std::vector<std::pair<std::string, std::chrono::high_resolution_clock::time_point> > coutVecString;

std::mutex m;

void createLazyString() {
	//coutVecString.push_back(std::pair<std::string, std::chrono::high_resolution_clock::time_point>("create start", std::chrono::high_resolution_clock::now()));
	std::lock_guard<std::mutex> lock(m);
	int len = rand() % 10 + 1;
	std::string tempString;
	for (int j = 0; j < len; ++j)
		tempString += char('a' + rand() % 26);
	//coutVecString.push_back( std::pair<std::string, std::chrono::high_resolution_clock::time_point>("create "+tempString, std::chrono::high_resolution_clock::now() ) );
	// vecLS.push_back(lazy_string(tempString));
	lazy_string ls(tempString);
	vecLS.push_back(ls);
}

void modifyLazyString() {
	// return;
	std::lock_guard<std::mutex> lock(m);

	//coutVecString.push_back(std::pair<std::string, std::chrono::high_resolution_clock::time_point>("modify start", std::chrono::high_resolution_clock::now()));
	if (vecLS.size() != 0) {
		int indexVecLS = rand() % vecLS.size();
		//coutVecString.push_back(std::pair<std::string, std::chrono::high_resolution_clock::time_point>("modify pre "+(std::string)(vecLS[indexVecLS]), std::chrono::high_resolution_clock::now()));
		vecLS[indexVecLS][rand() % vecLS[indexVecLS].size()] = char('a' + rand() % 26);
		//coutVecString.push_back(std::pair<std::string, std::chrono::high_resolution_clock::time_point>("modify post " + (std::string)(vecLS[indexVecLS]), std::chrono::high_resolution_clock::now()));
	}
}

void copyLazyString() {
	std::lock_guard<std::mutex> lock(m);
	//coutVecString.push_back(std::pair<std::string, std::chrono::high_resolution_clock::time_point>("copy start", std::chrono::high_resolution_clock::now()));
	if (vecLS.size() != 0) {
		int indexVecLS = rand() % vecLS.size();
		int lastIndex = vecLS.size();
		vecLS.push_back(vecLS[indexVecLS]);
		//coutVecString.push_back(std::pair<std::string, std::chrono::high_resolution_clock::time_point>((std::string)(vecLS[lastIndex]), std::chrono::high_resolution_clock::now()));
	}
}

std::vector<int> operationsThread[2];
std::ofstream fout;

void operationsProcess(int thr_ind) {
	for (int i = 0; i < operationsThread[thr_ind].size(); i++) {
		fout << (thr_ind + 1) << " process " << i << " " << operationsThread[thr_ind][i] << std::endl;
		if (operationsThread[thr_ind][i] == 1)
			createLazyString();
		else if (operationsThread[thr_ind][i] == 2)
			modifyLazyString();
		else if (operationsThread[thr_ind][i] == 3)
			copyLazyString();
	}
}

int main(int argc, char const *argv[]) {
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	try {
		srand(0);
		fout.open("output.txt");
		for (int i = 0; i < 17000; i++) {
			for(int j = 0; j < 2; ++j)
				operationsThread[j].push_back(rand() % 3 + 1);
		}
		std::thread thread1(operationsProcess, 0);
		std::thread thread2(operationsProcess, 1);
		thread1.join();
		thread2.join();
	}
	catch(const std::exception& e) {
		std::cout << "Aaaaaaaaaaaa!" << std::endl;
		std::cout << e.what() << std::endl;
	}
	return 0;
}