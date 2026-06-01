#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm> // Для std::swap

// Максимальное количество строк в файле по условию задания
const int MAX_SESSIONS = 100;

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

// Вывод статического массива (передается массив и его реальное заполненное количество элементов)
void print_sessions(const InternetSession sessions[], int count, const std::string& title) {
    std::cout << "\n=== " << title << " ===\n";
    for (int i = 0; i < count; ++i) {
        std::cout << sessions[i].start_time << " " << sessions[i].end_time << " " 
                  << sessions[i].received << " " << sessions[i].sent << " " << sessions[i].program << "\n";
    }
}

void filter_skype(const InternetSession sessions[], int count) {
    InternetSession result[MAX_SESSIONS];
    int result_count = 0;
    
    for (int i = 0; i < count; ++i) {
        if (sessions[i].program.find("Skype") != std::string::npos) {
            result[result_count++] = sessions[i];
        }
    }
    print_sessions(result, result_count, "Протокол использования сети программой Skype");
}

void filter_after_0800(const InternetSession sessions[], int count) {
    InternetSession result[MAX_SESSIONS];
    int result_count = 0;
    
    for (int i = 0; i < count; ++i) {
        if (sessions[i].start_time >= "08:00:00") {
            result[result_count++] = sessions[i];
        }
    }
    print_sessions(result, result_count, "Протокол использования сети после 8:00:00");
}

// Компараторы вынесены в глобальную область видимости (исправлено вложение)
bool compare_by_duration(const InternetSession& a, const InternetSession& b) {
    return a.get_duration() > b.get_duration();
}

bool compare_by_program_and_traffic(const InternetSession& a, const InternetSession& b) {
    if (a.program != b.program) {
        return a.program < b.program;
    }
    return (a.received + a.sent) > (b.received + b.sent);
}

// Сортировка вставками для статического массива
void insertion_sort(InternetSession arr[], int count, bool (*compare)(const InternetSession&, const InternetSession&)) {
    for (int i = 1; i < count; i++) {
        InternetSession key = arr[i];
        int j = i - 1;
        while (j >= 0 && compare(key, arr[j])) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Вспомогательная функция разделения для быстрой сортировки
int partition(InternetSession arr[], int low, int high, bool (*compare)(const InternetSession&, const InternetSession&)) {
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

// Быстрая сортировка для статического массива
void quick_sort(InternetSession arr[], int low, int high, bool (*compare)(const InternetSession&, const InternetSession&)) {
    if (low < high) {
        int pi = partition(arr, low, high, compare);
        quick_sort(arr, low, pi - 1, compare);
        quick_sort(arr, pi + 1, high, compare);
    }
}

int main() {
    std::ifstream infile("input.txt");
    if (!infile.is_open()) {
        std::cerr << "Ошибка открытия файла input.txt\n";
        return 1;
    }

    // Замена вектора на статический массив фиксированного размера
    InternetSession sessions[MAX_SESSIONS];
    int session_count = 0;
    
    std::string line;
    while (std::getline(infile, line) && session_count < MAX_SESSIONS) {
        if (!line.empty()) {
            sessions[session_count++] = parse_line(line);
        }
    }
    infile.close();

    // Передаем массив и количество прочитанных строк в фильтры
    filter_skype(sessions, session_count);
    filter_after_0800(sessions, session_count);

    // Копирование массивов для независимой демонстрации сортировок
    InternetSession sort1[MAX_SESSIONS];
    for (int i = 0; i < session_count; ++i) sort1[i] = sessions[i];
    
    insertion_sort(sort1, session_count, compare_by_duration);
    print_sessions(sort1, session_count, "Сортировка ВСТАВКАМИ: По убыванию времени сессии");

    InternetSession sort2[MAX_SESSIONS];
    for (int i = 0; i < session_count; ++i) sort2[i] = sessions[i];
    
    if (session_count > 0) {
        quick_sort(sort2, 0, session_count - 1, compare_by_program_and_traffic);
    }
    print_sessions(sort2, session_count, "БЫСТРАЯ сортировка: По имени программы и трафику");

    return 0;
}
