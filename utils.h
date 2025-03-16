#ifndef UTILS_H
#define UTILS_H
#include <string>

std::string read_line_with_backspace(const std::string &prompt);
std::string get_input_backspace(const std::string &prompt);
bool is_latin_for_mail(const std::string &s);
std::string get_input(const std::string &prompt);
std::string get_validated_input(const std::string &prompt, bool required = true);
bool is_latin_or_dash(const std::string &s);
std::string get_validated_name(const std::string &prompt, bool required = true);
bool is_validated_phone(const std::string &s);
bool validate_password_characters(const std::string &password);
std::string get_validated_password();
int get_menu_choice(const std::string &menu);
#endif // UTILS_H
