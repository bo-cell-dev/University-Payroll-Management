#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "UIUtils.h"

namespace UserRoles {
    enum Role {
        RECTOR,
        VICE_RECTOR,
        DIRECTOR,
        DEAN,
        DEPARTMENT_HEAD,
        PROFESSOR,
        ASSOCIATE_PROFESSOR,
        SENIOR_LECTURER,
        ASSISTANT,
        ACCOUNTANT,
        UNKNOWN
    };

    inline std::string roleToString(Role role) {
        switch (role) {
        case RECTOR: return "Ректор";
        case VICE_RECTOR: return "Проректор";
        case DIRECTOR: return "Директор";
        case DEAN: return "Декан";
        case DEPARTMENT_HEAD: return "Заведующий кафедрой";
        case PROFESSOR: return "Профессор";
        case ASSOCIATE_PROFESSOR: return "Доцент";
        case SENIOR_LECTURER: return "Старший преподаватель";
        case ASSISTANT: return "Ассистент";
        case ACCOUNTANT: return "Бухгалтер";
        default: return "Неизвестная должность";
        }
    }

    inline std::string roleToString(Role role, bool withDescription) {
        std::string base = roleToString(role);
        if (withDescription) {
            switch (role) {
            case RECTOR: return base + " - Высшая должность в университете";
            case VICE_RECTOR: return base + " - Заместитель ректора";
            case DEAN: return base + " - Руководитель факультета";
            case DEPARTMENT_HEAD: return base + " - Заведующий кафедрой";
            case PROFESSOR: return base + " - Преподаватель высшего уровня";
            case ASSOCIATE_PROFESSOR: return base + " - Доцент";
            case SENIOR_LECTURER: return base + " - Преподаватель";
            case ASSISTANT: return base + " - Ассистент преподавателя";
            case ACCOUNTANT: return base + " - Бухгалтер";
            default: return base + " - Описание недоступно";
            }
        }
        return base;
    }

    inline Role selectRole() {
        std::cout << "Выберите должность:\n";
        std::cout << "1. Ректор\n2. Проректор\n3. Директор\n4. Декан\n"
            << "5. Заведующий кафедрой\n6. Профессор\n7. Доцент\n"
            << "8. Старший преподаватель\n9. Ассистент\n"
            << "10. Бухгалтер\n";
        int choice = getValidInput<int>("Введите номер должности: ");
        switch (choice) {
        case 1: return RECTOR;
        case 2: return VICE_RECTOR;
        case 3: return DIRECTOR;
        case 4: return DEAN;
        case 5: return DEPARTMENT_HEAD;
        case 6: return PROFESSOR;
        case 7: return ASSOCIATE_PROFESSOR;
        case 8: return SENIOR_LECTURER;
        case 9: return ASSISTANT;
        case 10: return ACCOUNTANT;
        default: return UNKNOWN;
        }
    }

    inline Role roleFromString(const std::string& roleStr) {
        if (roleStr == "Ректор") return RECTOR;
        if (roleStr == "Проректор") return VICE_RECTOR;
        if (roleStr == "Директор") return DIRECTOR;
        if (roleStr == "Декан") return DEAN;
        if (roleStr == "Заведующий кафедрой") return DEPARTMENT_HEAD;
        if (roleStr == "Профессор") return PROFESSOR;
        if (roleStr == "Доцент") return ASSOCIATE_PROFESSOR;
        if (roleStr == "Старший преподаватель") return SENIOR_LECTURER;
        if (roleStr == "Ассистент") return ASSISTANT;
        if (roleStr == "Бухгалтер") return ACCOUNTANT;
        return UNKNOWN;
    }

    const std::vector<std::string> roleFiles = {
        "rector.txt",
        "vice_rector.txt",
        "director.txt",
        "dean.txt",
        "department_head.txt",
        "professor.txt",
        "associate_professor.txt",
        "senior_lecturer.txt",
        "assistant.txt"
    };
}
