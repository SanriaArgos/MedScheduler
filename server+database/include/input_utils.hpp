#ifndef INPUT_UTILS_HPP_
#define INPUT_UTILS_HPP_

#include <string>

std::string get_Input(const std::string &prompt);

std::string get_Validated_Input(const std::string &prompt, bool required = true);

bool isCyrillic(const std::string &s);

std::string get_Validated_Name(const std::string &prompt, bool required = true);

std::string get_Validated_Phone();

bool validate_Password_Characters(const std::string &password);

std::string get_Validated_Password();

int get_Menu_Choice(const std::string &menu);

#endif // INPUT_UTILS_HPP_
