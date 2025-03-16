#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <string>

std::string read_line_with_backspace(const std::string &prompt);
std::string get_input_backspace(const std::string &prompt);

std::string get_input(const std::string &prompt);
std::string get_validated_input(const std::string &prompt, bool required = true);
bool is_latin_or_dash(const std::string &s);
std::string get_validated_name(const std::string &prompt, bool required = true);
std::string get_validated_phone();
bool validate_password_characters(const std::string &password);
std::string get_validated_password();
int get_menu_choice(const std::string &menu);

#endif  // UTILS_HPP_
