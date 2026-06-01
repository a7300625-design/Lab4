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
