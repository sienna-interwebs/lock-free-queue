#ifndef STATS_HPP
#define STATS_HPP

#include <vector>

class Stats {
private:
    std::vector<double> trials;

    void prepare_data();

public:
    Stats() = default;

    void add_trial(double duration);

    int count() const;

    double min() const;
    double max() const;
    double mean() const;
    double variance() const;
    double std_dev() const;
    double percentile(double pct) const;
    double iqr() const;
    int count_outliers() const;

    void print(int capacity);
};

#endif
