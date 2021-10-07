/*
 * SliderWindow.h
 *
 *  Created on: Sep 10, 2021
 *      Author: magic
 */

#ifndef SLIDERWINDOW_H_
#define SLIDERWINDOW_H_

#include "IWindow.h"
#include <vector>

namespace gui {
struct SliderData {
	SliderData(cv::String label, int maximum,
			void (*callback)(int, void*) = nullptr, void *data = nullptr,
			int value = 0);

	cv::String label;
	int maximum;
	int value;
	void (*callback)(int, void*);
	void *data;
};

class SliderWindow: public IWindow {
public:
	SliderWindow(cv::String windowName, int width, int height,
			std::vector<SliderData> slider);
	virtual ~SliderWindow();
	/*
	 * @brief Maakt een window met sliders en laat deze zien.
	 */
	virtual void show() override;
	virtual void destroy() override;
	cv::String getLabel(int index) const;
	bool setLabel(cv::String label, int index);
	int getValue(int index) const;
	bool setValue(int value, int index);
	int getMaximum(int index) const;
	bool setMaximum(int maximum, int index);
protected:
	bool isValidSlider(int index) const;
private:
	std::vector<SliderData> sliders;
};
} // namespace gui

#endif /* SLIDERWINDOW_H_ */
