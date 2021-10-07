#include "SliderWindow.h"

namespace gui {
SliderData::SliderData(cv::String label, int maximum,
		void (*callback)(int, void*), void *data, int value) :
		label(std::move(label)), maximum(maximum), value(value), callback(
				callback), data(data) {
}

SliderWindow::SliderWindow(cv::String windowName, int width, int height,
		std::vector<SliderData> sliders) :
		IWindow(windowName, width, height), sliders(std::move(sliders)) {
}

SliderWindow::~SliderWindow() {
}

void SliderWindow::show() {
	cv::namedWindow(getWindowName());
	cv::resizeWindow(getWindowName(), { getWidth(), getHeight() });
	for (auto &slider : sliders) {
		cv::createTrackbar(slider.label, getWindowName(), &slider.value,
				slider.maximum, slider.callback, slider.data);
	}
	cv::imshow(getWindowName(), 0);
}

void SliderWindow::destroy() {
	cv::destroyWindow(getWindowName());
}

cv::String SliderWindow::getLabel(int index) const {
	if (isValidSlider(index)) {
		return sliders.at(index).label;
	}
	return "";
}

bool SliderWindow::setLabel(cv::String label, int index) {
	if (isValidSlider(index)) {
		sliders.at(index).label = label;
		return true;
	}
	return false;
}

int SliderWindow::getValue(int index) const {
	if (isValidSlider(index)) {
		return sliders.at(index).value;
	}
	return -1;
}

bool SliderWindow::setValue(int value, int index) {
	if (isValidSlider(index)) {
		sliders.at(index).value = value;
		return true;
	}
	return false;
}

int SliderWindow::getMaximum(int index) const {
	if (isValidSlider(index)) {
		return sliders.at(index).maximum;
	}
	return -1;
}

bool SliderWindow::setMaximum(int maximum, int index) {
	if (isValidSlider(index)) {
		sliders.at(index).maximum = maximum;
		return true;
	}
	return false;
}

bool SliderWindow::isValidSlider(int index) const {
	return not sliders.empty() and index >= 0 and index < (int) sliders.size();
}
} // namespace gui
