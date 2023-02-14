#pragma once

typedef enum language_t
{
	language_default = -1,
	language_english = 0,
	language_french = 1,
	language_german = 2,
	language_italian = 3,
	language_spanish = 4,
	language_num_states = 5,
}language_t;

struct LocalizationHeader
{
	enum language_t language;
	int numXAfiles;
	int XATableOffset;
	int numStrings;
};