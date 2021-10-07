#include "Application.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include "Utility/Timer.h"
#include "Batch.h"

//int dummy_error_handler(int status
//    , char const* func_name
//    , char const* err_msg
//    , char const* file_name
//    , int line
//    , void* userdata)
//{
//    //Do nothing -- will suppress console output
//    return 0;   //Return value is not used
//}
//
//void set_dummy_error_handler()
//{
//    cv::redirectError(dummy_error_handler);
//}
//
//void reset_error_handler()
//{
//    cv::redirectError(nullptr);
//}


void updateHSV(int value, void *data) {
	if (data) {
		((cv::Scalar*) data)->val[0] = value;
	}
}

Application::Application() :
		camera(2),
		calibration("Kalibreren", 500, 500,
				{ { "Roze lowH :", 179, updateHSV, &global::HUE_PINK.low,
						(int) global::HUE_PINK.low[0] }, { "Roze HighH :", 179,
						updateHSV, &global::HUE_PINK.high,
						(int) global::HUE_PINK.high[0] }, { "Groen lowH :", 255,
						updateHSV, &global::HUE_GREEN.low,
						(int) global::HUE_GREEN.low[0] }, { "Groen highH :",
						255, updateHSV, &global::HUE_GREEN.high,
						(int) global::HUE_GREEN.high[0] }, { "Geel lowH :", 255,
						updateHSV, &global::HUE_YELLOW.low,
						(int) global::HUE_YELLOW.low[0] }, { "Geel highH :",
						255, updateHSV, &global::HUE_YELLOW.high,
						(int) global::HUE_YELLOW.high[0] }, { "Oranje lowH :",
						255, updateHSV, &global::HUE_ORANGE.low,
						(int) global::HUE_ORANGE.low[0] }, { "Oranje highH :",
						255, updateHSV, &global::HUE_ORANGE.high,
						(int) global::HUE_ORANGE.high[0] } })
{
	bFinished.store(false);
	shape.store(global::NOSHAPE);
	color.store(global::NOCOLOR);
}

Application::~Application() {
}

void Application::initialize(int argc, char **argv) {
	staticImage = cv::imread("/home/magic/Pictures/Neon_shapes.png");
	calibration.show();
	parseArguments(argc, argv);

	if (bInteractive)
	{
		commandlineThread = std::thread(commandline, &bFinished, &shape, &color, filename);
	}
}

void Application::run() {
	showCamera();

	if (not bInteractive)
	{
		static Batch batch{ filename };
		static utility::SecTimer timer{ std::chrono::seconds{4} };
		std::string command;
		global::Shape shape;
		global::Color color;
		if (timer.Finished()) {
			if (batch.readLine(command, shape, color))
			{
				this->shape.store(shape);
				this->color.store(color);

				if (shape != global::NOSHAPE && color != global::NOCOLOR)
				{
					std::cout << "[+] ";
				}
				else
				{
					std::cout << "[-] ";
				}
				std::cout << command << std::endl;

				showCamera();
				timer.Reset();
			}
			else
			{
				bFinished.store(true);
			}
		}
	}

	if (cv::waitKey(1) == 'q')
	{
		bFinished.store(true);

	}


}

void Application::destroy() {
	if (commandlineThread.joinable())
	{
		commandlineThread.join();
	}

	if (camera.isOpened())
	{
		camera.release();
	}
}

bool Application::isFinished() const {
	return bFinished.load();
}

void Application::helpCommandlineArguments(int argc, char **argv) const {
	std::cout << "Run the program as follows:\n";
	std::cout << "[Interactive]\t" << argv[0] << "\n";
	std::cout << "[Batch]\t\t" << argv[0]
			<< " --batch /path/to/some/batch.txt\n";
}

void Application::parseArguments(int argc, char **argv) {
	if (argc == 1) {
		bInteractive = true;
		return;
	}
	if (argc != 3 or std::string(argv[1]) != std::string("--batch")) {
		helpCommandlineArguments(argc, argv);
		bFinished = true;
		return;
	}

	filename = std::string(argv[2]);
	bInteractive = false;
}

void Application::commandline(
		std::atomic_bool* bFinished,
		std::atomic<global::Shape>* outShape,
		std::atomic<global::Color>* outColor,
		std::string filename)
{
	Batch batch{filename};
	std::string command;
	global::Shape shape;
	global::Color color;

	while (not bFinished->load())
	{
		std::cout << "Enter a command: ";
		std::getline(std::cin, command);
		if (command == "exit")
		{
			break;
		}

		batch.parseCommand(command, shape, color);
		if (shape == global::NOSHAPE and color == global::NOCOLOR)
		{
			batch.helpCommando();
		}
		else
		{
			outShape->store(shape);
			outColor->store(color);
		}
	}

	bFinished->store(true);
}

void Application::showCamera() {
	if (camera.isOpened())
		{
			cv::Mat cameraImage{};
			camera >> cameraImage;

			if (!cameraImage.empty())
			{
				if (shape.load() != global::NOSHAPE and color.load() != global::NOCOLOR)
				{
					cameraImage = recognizer.recognize(cameraImage, shape.load(), color.load());
				}

				cv::imshow("Camera", cameraImage);
			}
		}
}
