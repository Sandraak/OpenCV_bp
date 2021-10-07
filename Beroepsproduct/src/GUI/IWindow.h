/*
 * IWindow.h
 *
 *  Created on: Sep 10, 2021
 *      Author: magic
 */

#ifndef IWINDOW_H_
#define IWINDOW_H_

#include "opencv2/opencv.hpp"

namespace gui
{
	class IWindow
	{
	public:
		IWindow(cv::String windowName, int width, int height);
		virtual ~IWindow();

		virtual void show() = 0;
		virtual void destroy() = 0;

		int getHeight() const;
		void setHeight(int height);
		int getWidth() const;
		void setWidth(int width);
		const cv::String& getWindowName() const;
		void setWindowName(const cv::String &windowName);
	private:
		cv::String windowName;
		int width;
		int height;
	};
} // namespace gui

#endif /* IWINDOW_H_ */

