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
    print_sessions(result, "Протокол использования сети после 8:00:00");
}
