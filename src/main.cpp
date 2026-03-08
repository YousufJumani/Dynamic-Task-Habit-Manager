#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

struct HabitTask {
    int id;
    std::string title;
    std::vector<std::string> completionDates;
};

static bool isValidDate(const std::string &date) {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') return false;
    for (int i = 0; i < static_cast<int>(date.size()); ++i) {
        if (i == 4 || i == 7) continue;
        if (!std::isdigit(static_cast<unsigned char>(date[i]))) return false;
    }
    return true;
}

static int toDayNumber(const std::string &date) {
    std::tm tm = {};
    tm.tm_year = std::stoi(date.substr(0, 4)) - 1900;
    tm.tm_mon = std::stoi(date.substr(5, 2)) - 1;
    tm.tm_mday = std::stoi(date.substr(8, 2));
    std::time_t t = std::mktime(&tm);
    return static_cast<int>(t / (60 * 60 * 24));
}

static int calculateStreak(std::vector<std::string> dates) {
    if (dates.empty()) return 0;
    std::sort(dates.begin(), dates.end());

    int streak = 1;
    int best = 1;
    for (size_t i = 1; i < dates.size(); ++i) {
        int prev = toDayNumber(dates[i - 1]);
        int curr = toDayNumber(dates[i]);
        if (curr == prev) continue;
        if (curr == prev + 1) ++streak;
        else streak = 1;
        best = std::max(best, streak);
    }
    return best;
}

static HabitTask* findTaskById(std::vector<std::unique_ptr<HabitTask>> &tasks, int id) {
    for (auto &task : tasks) {
        if (task->id == id) return task.get();
    }
    return nullptr;
}

int main() {
    std::vector<std::unique_ptr<HabitTask>> tasks;
    int nextId = 1;

    std::cout << "Dynamic Habit Manager\n";
    std::cout << "Commands: add, complete, delete, list, quit\n";

    while (true) {
        std::cout << "\n> ";
        std::string cmd;
        std::getline(std::cin, cmd);

        if (cmd == "quit") break;

        if (cmd == "add") {
            std::cout << "Title: ";
            std::string title;
            std::getline(std::cin, title);
            if (title.empty()) {
                std::cout << "Title cannot be empty.\n";
                continue;
            }
            auto task = std::make_unique<HabitTask>();
            task->id = nextId++;
            task->title = title;
            tasks.push_back(std::move(task));
            std::cout << "Task added.\n";
        } else if (cmd == "complete") {
            std::cout << "Task ID: ";
            std::string rawId;
            std::getline(std::cin, rawId);
            int id = 0;
            try {
                id = std::stoi(rawId);
            } catch (...) {
                std::cout << "Invalid task id.\n";
                continue;
            }

            HabitTask *task = findTaskById(tasks, id);
            if (!task) {
                std::cout << "Task not found.\n";
                continue;
            }

            std::cout << "Date (YYYY-MM-DD): ";
            std::string date;
            std::getline(std::cin, date);
            if (!isValidDate(date)) {
                std::cout << "Invalid date format.\n";
                continue;
            }

            task->completionDates.push_back(date);
            std::cout << "Marked complete. Current best streak: "
                      << calculateStreak(task->completionDates) << "\n";
        } else if (cmd == "delete") {
            std::cout << "Task ID: ";
            std::string rawId;
            std::getline(std::cin, rawId);
            int id = 0;
            try {
                id = std::stoi(rawId);
            } catch (...) {
                std::cout << "Invalid task id.\n";
                continue;
            }

            auto it = std::remove_if(tasks.begin(), tasks.end(), [id](const std::unique_ptr<HabitTask> &task) {
                return task->id == id;
            });
            if (it == tasks.end()) {
                std::cout << "Task not found.\n";
                continue;
            }
            tasks.erase(it, tasks.end());
            std::cout << "Task deleted.\n";
        } else if (cmd == "list") {
            if (tasks.empty()) {
                std::cout << "No tasks yet.\n";
                continue;
            }
            for (const auto &task : tasks) {
                std::cout << "[" << task->id << "] " << task->title
                          << " | completions: " << task->completionDates.size()
                          << " | best streak: " << calculateStreak(task->completionDates)
                          << "\n";
            }
        } else {
            std::cout << "Unknown command.\n";
        }
    }

    std::cout << "Goodbye.\n";
    return 0;
}
