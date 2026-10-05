#include "stats.hpp"
#include <numeric>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>

void Stats::add_trial(double duration_ns) {
    trials.push_back(duration_ns);
}

int Stats::count() const {
    return static_cast<int>(trials.size());
}

void Stats::prepare_data() {
    std::sort(trials.begin(), trials.end());
}

double Stats::min() const {
    return trials.empty() ? 0.0 : trials.front();
}

double Stats::max() const {
    return trials.empty() ? 0.0 : trials.back();
}

double Stats::mean() const {
    if (trials.empty()) return 0.0;
    double sum = std::accumulate(trials.begin(), trials.end(), 0.0);
    return sum / trials.size();
}

double Stats::variance() const {
    if (trials.size() < 2) return 0.0;
    double avg = mean();
    double sq_sum = 0.0;
    for (double val : trials) {
        sq_sum += (val - avg) * (val - avg);
    }
    return sq_sum / trials.size();
}

double Stats::std_dev() const {
    return std::sqrt(variance());
}

double Stats::percentile(double pct) const {
    if (trials.empty()) return 0.0;
    pct = std::clamp(pct, 0.0, 100.0);
    int idx = static_cast<int>((pct / 100.0) * (trials.size() - 1));
    return trials[idx];
}

double Stats::iqr() const {
    if (trials.size() < 4) return 0.0;
    return percentile(75.0) - percentile(25.0);
}

int Stats::count_outliers() const {
    if (trials.size() < 4) return 0;

    double q1 = percentile(25.0);
    double q3 = percentile(75.0);
    double current_iqr = q3 - q1;

    double lower_bound = q1 - (1.5 * current_iqr);
    double upper_bound = q3 + (1.5 * current_iqr);

    int outlier_count = 0;
    for (double val : trials) {
        if (val < lower_bound || val > upper_bound) {
            outlier_count++;
        }
    }
    return outlier_count;
}

void Stats::print(int capacity) {
    if (trials.empty()) return;

    prepare_data();

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "min: " << min() << "\n";
    std::cout << "max: " << max() << "\n";
    std::cout << "mean:     " << mean() << "\n";
    std::cout << "median:   " << percentile(50.0) << "\n";
    std::cout << "stdev:    " << std_dev() << "\n";
    std::cout << "p25:      " << percentile(25.0) << " ; p75: " << percentile(75.0) << "\n";
    std::cout << "p90:      " << percentile(90.0) << " ; p95: " << percentile(95.0) << " ; p99: " << percentile(99.0) << "\n";
    std::cout << "IQR:      " << iqr() << " ; outliers: " << count_outliers() << "\n";
}
