#include <stdio.h>
#include <engine.hpp>

int main(int argcc, char* argv[]) {
	printf("Booting up project!\n");
	return Engine::getInstance()->run();
}