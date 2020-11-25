#pragma once
#include "GuiCode/pch.hpp"


class Theme
{
public:

	enum ID
	{
		WINDOW_BACKGROUND = 0,
		TITLEBAR_BACKGROUND = 1,
		TITLEBAR_TEXT = 2,
        MENU_BACKGROUND = 10,
        MENU_BORDER = 11,
        MENU_BUTTON_TEXT = 12,
        MENU_BUTTON_DARKER_TEXT = 13,
        MENU_BUTTON_BACKGROUND = 14,
        MENU_BUTTON_BACKGROUND_HOVER = 15,
        MENU_BUTTON_BACKGROUND_PRESS = 16,
        MENU_DISABLED_BUTTON_TEXT = 17,
        MENU_DISABLED_BUTTON_DARKER_TEXT = 18,
        MENU_DISABLED_BUTTON_BACKGROUND = 19,
        MENU_DISABLED_BUTTON_BACKGROUND_HOVER = 20,
        MENU_DISABLED_BUTTON_BACKGROUND_PRESS = 21,

        VIEW_BACKGROUND = 100,
        ARRANGEMENT_DARKER = 1000,
        ARRANGEMENT_LIGHTER = 1001,
        ARRANGEMENT_LINE_1 = 1002,
        ARRANGEMENT_LINE_2 = 1003,
        ARRANGEMENT_LINE_3 = 1004,
    };

    static Color Get(int c) { return m_Colors.find(c) == m_Colors.end() ? Color{ 0,0,0,0 } : m_Colors[c]; }

	static void Load(const std::string& path) 
	{
        // Clear the current colors
        m_Colors.clear();

		// Open the file
		std::ifstream file; 
		file.open(path, std::ios::in);

		// Check if it's open
		if (!file.is_open())
		{
			LOG("Could not open file at: " << path);
			return;
		}

		// Go through all lines of the file
		std::string line;
        while (std::getline(file, line))
        {
            // Skip empty lines
            while (line.length() == 0)
                std::getline(file, line);

            // interpret the line
            InterpretLine(line);
        }

		// Close the file
		file.close();
	}

private:

    static void InterpretLine(const std::string& line)
	{
        // Take out the comment
        std::string_view _nc(line.c_str(), line.find_first_of('#'));
        
        // Single out the color and the id
        std::string_view _idstring(line.c_str(), line.find_first_of(':'));
        std::string_view _color((line.begin() + line.find_first_of(':') + 1).operator->(), line.find_first_of('#') - line.find_first_of(':') - 1);
        
        std::string_view _rstring(_color.data(), _color.find_first_of(','));
        _color.remove_prefix(_color.find_first_of(',') + 1);
        std::string_view _gstring(_color.data(), _color.find_first_of(','));
        _color.remove_prefix(_color.find_first_of(',') + 1);
        std::string_view _bstring(_color.data(), _color.find_first_of(','));
        _color.remove_prefix(_color.find_first_of(',') + 1);
        std::string_view _astring(_color.data(), _color.size());

        int _id = atoi(_idstring.data());
        float _r = atoi(_rstring.data());
        float _g = atoi(_gstring.data());
        float _b = atoi(_bstring.data());
        float _a = atoi(_astring.data());
        
        // Insert the color
        m_Colors.insert({ _id, Color{ _r, _g, _b, _a } });
	}


	static inline std::unordered_map<int, Color> m_Colors;
};