#include <stdio.h>
#include <engine.hpp>
#include <time.h>

int main(int argcc, char* argv[]) {
	srand(1337);
	printf("Booting up project!\n");
	return Engine::getInstance()->run();
}