#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

struct InternetSession {
    std::string start_time;
    std::string end_time;
    long long received;
    long long sent;
    std::string program;

    int get_duration() const {
        auto to_seconds = [](const std::string& time_str) {
            int h, m, s;
            char colon;
            std::stringstream ss(time_str);
            ss >> h >> colon >> m >> colon >> s;
            return h * 3600 + m * 60 + s;
        };
        return to_seconds(end_time) - to_seconds(start_time);
    }
};

InternetSession parse_line(const std::string& line) {
    std::stringstream ss(line);
    InternetSession session;
    ss >> session.start_time >> session.end_time >> session.received >> session.sent;
    ss >> std::ws;
    std::getline(ss, session.program);
    return session;
}
void print_sessions(const std::vector<InternetSession>& sessions, const std::string& title) {
    std::cout << "\n=== " << title << " ===\n";
    for (const auto& s : sessions) {
        std::cout << s.start_time << " " << s.end_time << " " 
                  << s.received << " " << s.sent << " " << s.program << "\n";
    }
}

void filter_skype(const std::vector<InternetSession>& sessions) {
    std::vector<InternetSession> result;
    for (const auto& s : sessions) {
        if (s.program.find("Skype") != std::string::npos) {
            result.push_back(s);
        }
    }
    print_sessions(result, "Протокол использования сети программой Skype");
}

void filter_after_0800(const std::vector<InternetSession>& sessions) {
    std::vector<InternetSession> result;
    for (const auto& s : sessions) {
        if (s.start_time >= "08:00:00") {
            result.push_back(s);
        }
    }

    bool compare_by_duration(const InternetSession& a, const InternetSession& b) {
    return a.get_duration() > b.get_duration();
}

bool compare_by_program_and_traffic(const InternetSession& a, const InternetSession& b) {
    if (a.program != b.program) {
        return a.program < b.program;
    }
    return (a.received + a.sent) > (b.received + b.sent);
}

void insertion_sort(std::vector<InternetSession>& arr, bool (*compare)(const InternetSession&, const InternetSession&)) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        InternetSession key = arr[i];
        int j = i - 1;
        while (j >= 0 && compare(key, arr[j])) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

int partition(std::vector<InternetSession>& arr, int low, int high, bool (*compare)(const InternetSession&, const InternetSession&)) {
    InternetSession pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (compare(arr[j], pivot)) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quick_sort(std::vector<InternetSession>& arr, int low, int high, bool (*compare)(const InternetSession&, const InternetSession&)) {
    if (low < high) {
        int pi = partition(arr, low, high, compare);
        quick_sort(arr, low, pi - 1, compare);
        quick_sort(arr, pi + 1, high, compare);
    }
}

    print_sessions(result, "Протокол использования сети после 8:00:00");
}
