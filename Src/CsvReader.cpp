#include "csvReader.h"
#include <fstream>
#include <assert.h>
#include "dinput.h"

CsvReader::CsvReader(std::string filename)
{
	all.clear();

	std::ifstream ifs(filename);
	if (!ifs) return;

	// BOM Skipする
	unsigned char BOMS[] = { 0xEF, 0xBB, 0xBF };
	bool found = true;
	for (int i = 0; i < 3; i++) {
		if (ifs.get() != BOMS[i]) {
			found = false;
			break;
		}
	}
	if (!found)
		ifs.seekg(std::ios_base::beg);

	// データを読む
	std::string lineStr;
	while (getline(ifs, lineStr)) {
		while (true) {
			int dq = 0;
			for (int i = 0; i < lineStr.size(); i++) {
				if (lineStr[i] == '"')
					dq++;
			}
			if (dq % 2 == 0)
				break;
			std::string s;
			getline(ifs, s);
			lineStr += "\n" + s;
		}
		for (auto it = lineStr.begin(); it != lineStr.end();) {
			if (*it == '"')
				it = lineStr.erase(it);
			if (it != lineStr.end())
				it++;
		}

		// 行内を,で切り分ける
		LINEREC lineRecord;
		int top = 0;
		bool indq = false;
		for (int n = 0; n < lineStr.size(); n++) {
			if (lineStr[n] == ',') {
				if (!indq) {
					lineRecord.record.emplace_back(lineStr.substr(top, (size_t)(n - top)));
					top = n + 1;
				}
			}
			else if (lineStr[n] == '"')
				indq = !indq;
		}
		lineRecord.record.emplace_back(lineStr.substr(top, lineStr.size() - top));
		all.emplace_back(lineRecord);
	}
	ifs.close();
}

CsvReader::~CsvReader()
{
	for (auto rec : all)
		rec.record.clear();
	all.clear();
}

unsigned int CsvReader::GetLines()
{
	return all.size();
}

unsigned int CsvReader::GetColumns(unsigned int line)
{
	assert(line < GetLines());
	return all[line].record.size();
}

std::string CsvReader::GetString(unsigned int line, unsigned int column)
{
	assert(line < GetLines());
	if (column >= GetColumns(line))
		return "";
	return all[line].record[column];
}

int CsvReader::GetInt(unsigned int line, unsigned int column)
{
	std::string str = GetString(line, column);
	return std::stoi(str);
}

float CsvReader::GetFloat(unsigned int line, unsigned int column)
{
	std::string str = GetString(line, column);
	return std::stof(str);
}

unsigned long CsvReader::GetKeyDef(unsigned int line, unsigned int column)
{
	std::string str = GetString(line, column);

	if (str == "ESCAPE")
	{
		return DIK_ESCAPE;
	}
	else if (str == "MINUS")
	{
		return DIK_MINUS;
	}
	else if (str == "EQUALS")
	{
		return DIK_EQUALS;
	}
	else if (str == "BACK")
	{
		return DIK_BACK;
	}
	else if (str == "TAB")
	{
		return DIK_TAB;
	}
	else if (str == "Q")
	{
		return DIK_Q;
	}
	else if (str == "W")
	{
		return DIK_W;
	}
	else if (str == "E")
	{
		return DIK_E;
	}
	else if (str == "R")
	{
		return DIK_R;
	}
	else if (str == "T")
	{
		return DIK_T;
	}
	else if (str == "Y")
	{
		return DIK_Y;
	}
	else if (str == "U")
	{
		return DIK_U;
	}
	else if (str == "I")
	{
		return DIK_I;
	}
	else if (str == "O")
	{
		return DIK_O;
	}
	else if (str == "P")
	{
		return DIK_P;
	}
	else if (str == "LBRACKET")
	{
		return DIK_LBRACKET;
	}
	else if (str == "RBRACKET")
	{
		return DIK_RBRACKET;
	}
	else if (str == "A")
	{
		return DIK_A;
	}
	else if (str == "S")
	{
		return DIK_S;
	}
	else if (str == "D")
	{
		return DIK_D;
	}
	else if (str == "F")
	{
		return DIK_F;
	}
	else if (str == "G")
	{
		return DIK_G;
	}
	else if (str == "H")
	{
		return DIK_H;
	}
	else if (str == "J")
	{
		return DIK_J;
	}
	else if (str == "K")
	{
		return DIK_K;
	}
	else if (str == "L")
	{
		return DIK_L;
	}
	else if (str == "SEMICOLON")
	{
		return DIK_SEMICOLON;
	}
	else if (str == "APOSTROPHE")
	{
		return DIK_APOSTROPHE;
	}
	else if (str == "GRAVE")
	{
		return DIK_GRAVE;
	}
	else if (str == "LSHIFT")
	{
		return DIK_LSHIFT;
	}
	else if (str == "BACKSLASH")
	{
		return DIK_BACKSLASH;
	}
	else if (str == "Z")
	{
		return DIK_Z;
	}
	else if (str == "X")
	{
		return DIK_X;
	}
	else if (str == "C")
	{
		return DIK_C;
	}
	else if (str == "V")
	{
		return DIK_V;
	}
	else if (str == "B")
	{
		return DIK_B;
	}
	else if (str == "N")
	{
		return DIK_N;
	}
	else if (str == "M")
	{
		return DIK_M;
	}
	else if (str == "COMMA")
	{
		return DIK_COMMA;
	}
	else if (str == "PERIOD")
	{
		return DIK_PERIOD;
	}
	else if (str == "SLASH")
	{
		return DIK_SLASH;
	}
	else if (str == "RSHIFT")
	{
		return DIK_RSHIFT;
	}
	else if (str == "MULTIPLY")
	{
		return DIK_MULTIPLY;
	}
	else if (str == "LMENU")
	{
		return DIK_LMENU;
	}
	else if (str == "SPACE")
	{
		return DIK_SPACE;
	}
	else if (str == "CAPITAL")
	{
		return DIK_CAPITAL;
	}
	else if (str == "UP")
	{
		return DIK_UP;
	}
	else if (str == "DOWN")
	{
		return DIK_DOWN;
	}
	else if (str == "LEFT")
	{
		return DIK_LEFT;
	}
	else if (str == "RIGHT")
	{
		return DIK_RIGHT;
	}

}
