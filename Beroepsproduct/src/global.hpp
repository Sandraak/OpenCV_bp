/*
 * global.hpp
 *
 *  Created on: Sep 18, 2021
 *      Author: sandra
 */

#ifndef GLOBAL_HPP_
#define GLOBAL_HPP_

#include <string>
#include <vector>
#include "opencv2/core.hpp"

namespace global
{
 /// enum met alle mogelijke kleuren
enum Color {
	GREEN = 0, PINK, YELLOW, ORANGE, NOCOLOR
};

	extern std::vector<std::string> colors;

	/// enum met alle mogelijke vormen
	enum Shape{
		TRIANGLE = 0,
		RECTANGLE,
		SQUARE,
		CIRCLE,
		HALFCIRCLE,
		NOSHAPE
	};

	extern std::vector<std::string> shapes;

	/// Struct met lage en hoge threshold waarden van een kleur
	struct ColourRange {
		cv::Scalar low;
		cv::Scalar high;
	};


	/*
	 * @brief koppelt kleur of vorm uit vector aan de bijbehorende kleur of vorm uit enum.
	 *
	 * @p line input stringg
	 * @p object vector met kleuren of vormen.
	 *
	 * @return enum met kleur of vorm
	 */
	template<typename T>
	T stringToEnum(std::string& line, std::vector<std::string> const& strings)
	{
	    for (int i = 0 ; i < (int)strings.size(); ++i)
	    {
	    	auto pos = line.find(strings[i]);

	        if (pos == 0)
	        {
	            line = line.substr(strings[i].size());
	            return (T)i;
	        }
	    }
	    return (T)(strings.size() - 1);
	}

	extern ColourRange HUE_PINK;
	extern ColourRange HUE_GREEN;
	extern ColourRange HUE_YELLOW;
	extern ColourRange HUE_ORANGE;
} // namespace global

#endif /* GLOBAL_HPP_ */
