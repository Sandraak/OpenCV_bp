#ifndef BATCH_H_
#define BATCH_H_

#include <fstream>
#include <string>
#include "global.hpp"

class Batch {
public:
	Batch(std::string filename);
	virtual ~Batch();

	bool parseCommand(
			std::string command,
			global::Shape& outShape,
			global::Color& outColor,
			bool bRemoveComment = false) const;
	bool readLine(
			std::string& outCommand,
			global::Shape& outShape,
			global::Color& outColor);
	void helpCommando() const;
private:
	std::string removeComment(std::string command) const;
private:
	std::string filename;
	std::fstream file;
};

#endif /* BATCH_H_ */
