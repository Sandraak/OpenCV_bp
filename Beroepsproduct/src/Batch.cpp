#include "Batch.h"
#include <iostream>

Batch::Batch(std::string filename)
	: filename(filename), file(filename)
{
}

Batch::~Batch() {
}

bool Batch::parseCommand(
		std::string command,
		global::Shape &outShape,
		global::Color &outColor,
		bool bRemoveComment) const
{
	if (bRemoveComment)
	{
		command = removeComment(command);
	}

	outShape = global::stringToEnum<global::Shape>(command, global::shapes);
	if (outShape == global::NOSHAPE or command.empty() or command.at(0) != ' ')
	{
		outColor = global::NOCOLOR;
		return false;
	}

	command = command.substr(1);
	outColor = global::stringToEnum<global::Color>(command, global::colors);

	return outColor != global::NOCOLOR;
}

void Batch::helpCommando() const {
	std::cout << "Enter 'exit' to quit. Commands should be entered as follows:\n";
	std::cout << "\t[shape] [color]\n";
	std::cout << "\tShapes: 'driehoek', 'vierkant', 'rechthoek', 'halve cirkel', 'cirkel'\n";
	std::cout << "\tColors: 'roze', 'groen', 'geel', 'oranje'\n";
}

bool Batch::readLine(
		std::string& outCommand,
		global::Shape& outShape,
		global::Color& outColor)
{
	if (std::getline(file, outCommand))
	{
		parseCommand(outCommand, outShape, outColor, true);
		return true;
	}
	return false;
}

std::string Batch::removeComment(std::string command) const {
	auto index = command.find('#');
	if (index != std::string::npos)
	{
		command = command.substr(0, index);
	}
	return command;
}

