/*
 * global.cpp
 *
 *  Created on: Sep 18, 2021
 *      Author: sandra
 */

#include "global.hpp"

namespace global
{
	std::vector<std::string> shapes {
		"driehoek",
		"rechthoek",
		"vierkant",
		"cirkel",
		"halve cirkel",
		"noshape"
	};

	std::vector<std::string> colors {
		"groen",
		"roze",
		"geel",
		"oranje",
		"nocolor"
	};

	ColourRange HUE_PINK = { cv::Scalar(130, 0, 0), cv::Scalar(165, 255, 255) }; // lowH, lowS, lowV
	ColourRange HUE_GREEN = { cv::Scalar(55, 0, 0), cv::Scalar(73, 255, 255) };
	ColourRange HUE_YELLOW = { cv::Scalar(35, 0, 0), cv::Scalar(64, 255, 255) };
	ColourRange HUE_ORANGE = { cv::Scalar(6, 0, 0), cv::Scalar(20, 255, 255) };
} //namespace global
