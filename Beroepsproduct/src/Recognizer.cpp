#include "Recognizer.h"
#include "global.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <cmath>
#include <opencv2/highgui/highgui_c.h>

#include <iostream>
#include "Utility/Timer.h"

Recognizer::Recognizer() {
}

Recognizer::~Recognizer() {
}

cv::Mat Recognizer::recognize(cv::Mat original, global::Shape shape,
		global::Color color) {
	auto copy = original.clone();
	utility::MilliTimer timer;
	auto filtered = findColor(copy, color);
	cv::Mat filtered2;
	cv::resize(filtered, filtered2, cv::Size(), 0.2, 0.2);
	cv::imshow("kleur",filtered2);

	auto contours = findShape(filtered, shape);
	auto duration = timer();
	return draw(copy, contours, duration);
}

cv::Mat Recognizer::findColor(cv::Mat original, global::Color color) {
	cv::Mat output;
	cv::Mat hsv;
	cv::cvtColor(original, hsv, cv::COLOR_BGR2HSV);

	switch (color) {
	case global::PINK:
		cv::inRange(hsv, global::HUE_PINK.low, global::HUE_PINK.high, output);
		break;
	case global::GREEN:
		cv::inRange(hsv, global::HUE_GREEN.low, global::HUE_GREEN.high, output);
		break;
	case global::YELLOW:
		cv::inRange(hsv, global::HUE_YELLOW.low, global::HUE_YELLOW.high,
				output);
		break;
	case global::ORANGE:
		cv::inRange(hsv, global::HUE_ORANGE.low, global::HUE_ORANGE.high,
				output);
		break;
	default:
		break;
	}

	cv::erode(output, output,
			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
	cv::dilate(output, output,
			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
	cv::dilate(output, output,
			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
	cv::erode(output, output,
			cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
	return output;
}

float Recognizer::calculateDistance(cv::Point point1, cv::Point point2) {
	return cv::norm(point1 - point2);
}

bool Recognizer::checkSquare(std::vector<cv::Point> approximation) {
	std::vector<float> lengths;
	lengths.push_back(calculateDistance(approximation[0], approximation[1]));
	lengths.push_back(calculateDistance(approximation[1], approximation[2]));
	lengths.push_back(calculateDistance(approximation[2], approximation[3]));
	lengths.push_back(calculateDistance(approximation[3], approximation[0]));
	std::sort(lengths.begin(), lengths.end(), std::greater<float>());

	return ((lengths[0] / lengths[3]) - 1 <= 0.2);
}

bool Recognizer::checkHalfCircle(std::vector<cv::Point> approximation) {
	cv::RotatedRect rectangle = cv::minAreaRect(approximation);
	cv::Point2f corners[4];
	rectangle.points(corners);
	double height = cv::norm(corners[0] - corners[1]);
	double width = cv::norm(corners[1] - corners[2]);
	double area = height * width;
	double contourArea = cv::contourArea(approximation);
	return (contourArea / area > M_PI / 4 - 0.1 && contourArea / area < M_PI / 4
			&& !checkSquare(approximation));
}

std::vector<std::vector<cv::Point> > Recognizer::findShape(cv::Mat filtered,
		global::Shape shape) {
	std::vector<std::vector<cv::Point>> contours;
	std::vector<std::vector<cv::Point>> foundContours;
	cv::Canny(filtered, filtered, 0, 50, 5);
	cv::findContours(filtered.clone(), foundContours, CV_RETR_EXTERNAL,
			CV_CHAIN_APPROX_SIMPLE);

	for (const auto &contour : foundContours) {
		std::vector<cv::Point> approximation;
		cv::approxPolyDP( { contour }, approximation,
				cv::arcLength( { contour }, true) * 0.02, true);

		if (std::fabs(cv::contourArea(contour)) < 100
				or not cv::isContourConvex(approximation)) {
			continue;
		}
		switch (shape) {
		case global::TRIANGLE:
			if (approximation.size() == 3) {
				contours.push_back(contour);
			}
			break;
		case global::SQUARE:
			if (approximation.size() == 4 && checkSquare(approximation)) {
				contours.push_back(contour);
			}
			break;
		case global::RECTANGLE:
			if (approximation.size() == 4 && !checkSquare(approximation)) {
				contours.push_back(contour);
			}
			break;
		case global::CIRCLE:
			if (approximation.size() >= 6 && !checkHalfCircle(approximation)) {
				contours.push_back(contour);
			}

			break;
		case global::HALFCIRCLE:
			if (approximation.size() >= 6 && checkHalfCircle(approximation)) {
				contours.push_back(contour);
			}
			break;
		default:
			break;
		}
	}
	cv::cvtColor(filtered, filtered, cv::COLOR_GRAY2BGR);
	return contours;
}

cv::Mat Recognizer::draw(cv::Mat original,
		std::vector<std::vector<cv::Point>> const &contours, float duration) {
	if (!contours.empty()) {
		cv::drawContours(original, contours, -1, cv::Scalar(255, 255, 0), 5);
		for (auto const &contour : contours) {
			const auto m = cv::moments(contour);
			original = text(original,
					cv::Point { (int) (m.m10 / m.m00), (int) (m.m01 / m.m00) },
					cv::contourArea(contour), duration);
		}
	} else {
		cv::putText(original,
				std::string(
						"Could not find the shape in "
								+ std::to_string(duration) + "ms"), { 10, 10 },
				cv::FONT_HERSHEY_TRIPLEX, 1, cv::Scalar { });
	}
	return original;
}

cv::Mat Recognizer::text(cv::Mat original, const cv::Point &center, double area,
		float duration) {
	constexpr int font = cv::FONT_HERSHEY_TRIPLEX;
	constexpr double scale = 0.5;
	constexpr int thickness = 1;
	int baseline = 0;

	std::string areaText = "Area: " + std::to_string(area);
	std::string centerText = "(" + std::to_string(center.x) + ", "
			+ std::to_string(center.y) + ")";
	std::string durationText = "Duration: " + std::to_string(duration) + "ms";

	auto areaTextSize = cv::getTextSize(areaText, font, scale, thickness,
			&baseline);
	auto centerTextSize = cv::getTextSize(centerText, font, scale, thickness,
			&baseline);
	auto durationTextSize = cv::getTextSize(durationText, font, scale,
			thickness, &baseline);

	cv::putText(original, areaText,
			{ center.x - (int) (areaTextSize.width / 2.), center.y
					- centerTextSize.height }, font, scale, cv::Scalar { });
	cv::putText(original, centerText,
			{ center.x - (int) (centerTextSize.width / 2.), center.y }, font,
			scale, cv::Scalar { });
	cv::putText(original, durationText,
			{ center.x - (int) (durationTextSize.width / 2.), center.y
					+ centerTextSize.height }, font, scale, cv::Scalar { });

	return original;
}
