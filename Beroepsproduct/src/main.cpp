#include "Application.h"

int main(int argc, char** argv) {
	Application application;

	application.initialize(argc, argv);
	while (!application.isFinished())
	{
		application.run();
	}
	application.destroy();

	return 0;
}
