#pragma once
#include <istream>
#include <string>

// Reads a line until \r\n is found
std::istream& httpGetline(std::istream& is, std::string& t)
{
	//start the string
	t.clear();
	t.push_back(is.get());

	char c;
	while ((t.back() != '\r' && (c = is.get()) != '\n') || is.bad())
	{
		t.push_back(c);
	}

	if (!is.bad())
	{
		// We found the end of line sequence. Pop off the second character of the sequence we added to the result
		t.pop_back();
	}
	return is;
}