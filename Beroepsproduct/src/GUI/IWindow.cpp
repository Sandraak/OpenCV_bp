#include "IWindow.h"

namespace gui
{
	IWindow::IWindow(cv::String windowName, int width, int height)
		: windowName(std::move(windowName)), width(width), height(height)
	{
	}

	IWindow::~IWindow() {
	}

	int IWindow::getHeight() const {
		return height;
	}

	void IWindow::setHeight(int height) {
		this->height = height;
	}

	int IWindow::getWidth() const {
		return width;
	}

	void IWindow::setWidth(int width) {
		this->width = width;
	}

	const cv::String& IWindow::getWindowName() const {
		return windowName;
	}

	void IWindow::setWindowName(const cv::String &windowName) {
		this->windowName = windowName;
	}
} // namespace gui
