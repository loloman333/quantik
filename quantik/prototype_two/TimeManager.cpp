#include "TimeManager.hpp"


std::chrono::steady_clock::time_point TimeManager::tn;
std::chrono::duration<double> TimeManager::total_time;

void TimeManager::start(bool keep_total)
{
    tn = std::chrono::steady_clock::now();
    if (!keep_total) total_time = std::chrono::duration<double>{};
}

std::chrono::duration<double> TimeManager::step()
{
    auto tn_plus_1 = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = tn_plus_1 - tn;
    tn = tn_plus_1;
    total_time += duration;

    return duration;
}

std::chrono::duration<double> TimeManager::get_total_time()
{
    return total_time;
}

string TimeManager::to_string(std::chrono::duration<double> duration, string separator)
{
    int hours = static_cast<int>(duration.count()) / 3600;
    double remaining_seconds = static_cast<double>(duration.count()) - hours * 3600;
    int minutes = remaining_seconds / 60;
    double seconds = remaining_seconds - minutes * 60;

    std::string hour_info = hours != 0 ? STR(hours) + " hours " + separator : "";
    std::string minute_info = minutes != 0 ? STR(minutes) + " minutes " + separator : "";
    std::string time_info = hour_info + minute_info + STR(seconds) + " seconds";

    return time_info;
}
