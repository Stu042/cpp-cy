//
// Created by stu on 21/02/2021.
//
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <vector>
#include <map>
#include "console.h"
#include "utils.h"

const char* ConsoleAqua = "\033aqua\033";
const char* ConsoleBlack = "\033black\033";
const char* ConsoleBlue = "\033blue\033";
const char* ConsoleGreen = "\033green\033";
const char* ConsoleGrey = "\033grey\033";
const char* ConsoleLtAqua = "\033ltaqua\033";
const char* ConsoleLtBlue = "\033ltblue\033";
const char* ConsoleLtGreen = "\033ltgreen\033";
const char* ConsoleLtPurple = "\033ltpurple\033";
const char* ConsoleLtRed = "\033ltred\033";
const char* ConsoleLtWhite = "\033ltwhite\033";
const char* ConsoleLtYellow = "\033ltyellow\033;
const char* ConsolePurple = "\033purple\033";
const char* ConsoleRed = "\033red\033";
const char* ConsoleReset = "\033reset\033";
const char* ConsoleWhite = "\033white\033";
const char* ConsoleYellow = "\033yellow\033";

const std::map<std::string, const char *> ConsoleCodes = {
	{"aqua",			"\033[38;2;0;255;255m"},
	{"black",		"\033[38;2;0;0;0m"},
	{"blue",			"\033[38;2;0;0;255m"},
	{"green",		"\033[38;2;0;255;0m"},
	{"grey",			"\033[38;2;128;128;128m"},
	{"ltaqua",		"\033[38;2;64;255;255m"},
	{"ltblue",		"\033[38;2;64;64;255m"},
	{"ltgreen",		"\033[38;2;64;255;64m"},
	{"ltpurple",		"\033[38;2;255;64;255m"},
	{"ltred",		"\033[38;2;255;64;64m"},
	{"ltwhite",		"\033[38;2;255;255;255m"},
	{"ltyellow",		"\033[38;2;64;255;255m"},
	{"purple",		"\033[38;2;255;0;255m"},
	{"red",			"\033[38;2;255;0;0m"},
	{"reset",		"\033[0m"},
	{"white",		"\033[38;2;248;248;248m"},
	{"yellow",		"\033[38;2;0;255;255m"},
};


void AddToBuf(const char* text, std::vector<char>& ConsoleColourBuf) {
	while (*text != '\0')
		ConsoleColourBuf.push_back(*text++);
}

char *ConsoleColour(const char *text, std::vector<char>& ConsoleColourBuf) {
	char buf[10];
	char bufIdx;
	bool reset=false;
	while (*text != '\0') {
		if (*text == '\033') {
			bufIdx = 0;
			text++;
			while (*text != '\033')
				buf[bufIdx++] = *text++;
			buf[bufIdx] = '\0';
			text++;
			if (ConsoleCodes.find(buf) != ConsoleCodes.end()) {
				if (reset) {
					AddToBuf("\033[0m", ConsoleColourBuf);
					reset = false;
				}
				AddToBuf(ConsoleCodes.at(buf), ConsoleColourBuf);
				reset = true;
			} else {
				PrintError("ConsoleColour(), colour doesnt exist: %s", buf);
			}
		} else {
			ConsoleColourBuf.push_back(*text++);
		}
	}
	if (reset) {
		AddToBuf("\033[0m", ConsoleColourBuf);
	}
	ConsoleColourBuf.push_back('\0');
	return ConsoleColourBuf.data();
}


void ConsolePrint(const char *fmt, ...) {
	std::vector<char> ConsoleColourBuf;
	va_list args;
	va_start(args, fmt);
	char *buf = GetStringMem(fmt, args);
	vsprintf(buf, fmt, args);
	va_end(args);
	char *output = ConsoleColour(buf, ConsoleColourBuf);
	fputs(output, stdout);
	delete buf;
	ConsoleColourBuf.clear();
}


void ConsoleTest() {
	ConsolePrint("\033red\033Testing output\n");
	ConsolePrint("\033ltblue\033Testing output\n");
	ConsolePrint("Testing output\n");
}

