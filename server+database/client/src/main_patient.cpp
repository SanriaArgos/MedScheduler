#include "client_patient.hpp"
#include <iostream>

void display_menu() {
    std::cout << "\n=== Patient Menu ===\n";
    std::cout << "1. Register\n";
    std::cout << "2. Login\n";
    std::cout << "3. View doctor's schedule\n";
    std::cout << "4. Exit\n";
    std::cout << "Choose an option: ";
}

int main() {
    int choice = 0;
    std::string token; // Токен авторизации

    while (true) {
        display_menu();
        std::cin >> choice;

        switch (choice) {
            case 1: {
                registration();
                break;
            }

            case 2: { 
                token = login();
                if (!token.empty()) {
                    std::cout << "Login successful!\n";
                } else {
                    std::cout << "Login failed.\n";
                }
                break;
            }

            case 3: { 
                if (token.empty()) {
                    std::cout << "Please login first.\n";
                } else {
                    view_doctor_schedule();
                }
                break;
            }

            case 4: { 
                std::cout << "Exiting the program.\n";
                return 0;
            }

            default: {
                std::cout << "Invalid choice. Please try again.\n";
                break;
            }
        }
    }

    return 0;
}

//здесь токен будет существовать, пока мы в мейне, то есть пока программа запущена
//если мы выходим из приложения, то есть программа завершается, то надо логиниться снова