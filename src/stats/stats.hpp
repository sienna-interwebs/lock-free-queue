#ifndef STATS_HPP
#define STATS_HPP

#include <vector>

class Stats {
private:
    std::vector<double> trials;
    bool is_sorted;

    void ensure_sorted();

public:
    Stats() {
        is_sorted = false;
    }

    void add_trial(double duration_ms);
    int count() const;
    double min();
    double max();
    double mean() const;
    double variance() const;
    double std_dev() const;
    double percentile(double pct);
    double iqr();
    int count_outliers();

    void print(int capacity);
};

#endif
