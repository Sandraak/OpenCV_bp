#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <atomic>
#include <string>
#include <thread>
#include "opencv2/opencv.hpp"
#include "GUI/SliderWindow.h"
#include "global.hpp"
#include "Recognizer.h"

class Application {
public:
	Application();
	virtual ~Application();

	void initialize(int argc, char** argv);
	void run();
	void destroy();
	bool isFinished() const;

private:
	void helpCommandlineArguments(int argc, char** argv) const;
	void parseArguments(int argc, char** argv);
	static void commandline(
			std::atomic_bool* bFinished,
			std::atomic<global::Shape>* outShape,
			std::atomic<global::Color>* outColor,
			std::string filename);
	void showCamera();
private:
	cv::VideoCapture camera;
	gui::SliderWindow calibration;
	Recognizer recognizer;
	cv::Mat staticImage;
	std::thread commandlineThread;
	std::string filename;
	bool bInteractive = true;
	std::atomic_bool bFinished;
	std::atomic<global::Shape> shape;
	std::atomic<global::Color> color;
};

#endif /* APPLICATION_H_ */
