#include "Source.h"

Interpolator::Interpolator(const std::vector<std::pair<double, double>>& new_data) : data(new_data) {
    std::sort(data.begin(), data.end());
}

std::vector<std::pair<double, double>>& Interpolator::get_data() {
    return data;
}

/* ---------- */

LinearInterpolator::LinearInterpolator(const std::vector<std::pair<double, double>>& new_data) : Interpolator(new_data) {}

double LinearInterpolator::f(double x) {
    /* Check if given X is less than X int data */
    auto lessThan = [](const std::pair<double, double>& point, double x) {
        return point.first < x;
    };

    /* Find first element that is lower than X */
    auto iter = std::lower_bound(data.begin(), data.end(), x, lessThan);

    /* Check if X is greater than the greatest in data */
    if (iter == data.end())
        return (data.end() - 1)->second;

    /* Check if X is lower than the lowest in data */
    if (iter == data.begin() && x <= data.begin()->first)
        return data.begin()->second;

    double x0 = (iter - 1)->first;
    double y0 = (iter - 1)->second;
    double x1 = iter->first;
    double y1 = iter->second;

    double dx = x1 - x0;
    double dy = y1 - y0;

    return y0 + ((x - x0) / dx) * dy;
}

/* For chosen number of intervals function finds values for each interval */
void LinearInterpolator::interpolate(int intervals) {
    std::vector<std::pair<double, double>> result;
    for (auto iter = data.begin(); iter != data.end() - 1; iter++) {
        double step = ((iter + 1)->first - iter->first) / intervals;
        for (double x = iter->first; x != (iter + 1)->first; x += step) {
            double y = f(x);
            result.push_back(std::pair<double, double>(x, y));
        }
    }
    result.push_back(std::pair<double, double>((data.end() - 1)->first, (data.end() - 1)->second));

    data.clear();
    data.insert(data.end(), result.begin(), result.end());
}

void LinearInterpolator::draw(juce::Graphics& g) {

    /* -------------------- */
    /* LINEAR INTERPOLATION */
    /* -------------------- */

    std::vector<juce::Line<float>> lines_linear;
    for (auto iter = data.begin(); iter != data.end() - 1; iter++) {
        juce::Point<float> a(iter->first, iter->second);
        juce::Point<float> b((iter + 1)->first, (iter + 1)->second);
        lines_linear.push_back(juce::Line<float>(a, b));
        g.drawRoundedRectangle(a.getX(), a.getY(), 5, 5, 5, 5);
        g.drawRoundedRectangle(b.getX(), b.getY(), 5, 5, 5, 5);
    }

    for (auto iter = lines_linear.begin(); iter != lines_linear.end(); iter++) {
        g.drawLine(*iter, 2.0f);
    }
}

/* ---------- */

HermiteInterpolator::HermiteInterpolator(const std::vector<std::pair<double, double>>& new_data, std::vector<double>& new_derivatives) : Interpolator(new_data), derivatives(new_derivatives) {}

double HermiteInterpolator::f(double x, double x0, double x1, double y0, double dy0, double y1, double dy1) {
    double lval = ((1 - (2 * (x - x0) / (x0 - x1))) * y0 + (x - x0) * dy0) * pow(((x - x1) / (x0 - x1)), 2);
    double rval = ((1 - (2 * (x - x1) / (x1 - x0))) * y1 + (x - x0) * dy1) * pow(((x - x0) / (x1 - x0)), 2);

    return lval + rval;
}

/* For chosen number of intervals function finds values for each interval */
void HermiteInterpolator::interpolate(int intervals) {
    std::vector<std::pair<double, double>> data_result;
    std::vector<double> derivatives_result;

    for (int i = 0; i < derivatives.size() - 1; i++) {
        double step = (data[i + 1].first - data[i].first) / intervals;

        for (double z = data[i].first; z != data[i + 1].first; z += step) {
            double x0 = data[i].first;
            double x1 = data[i + 1].first;
            double y0 = data[i].second;
            double dy0 = derivatives[i];
            double y1 = data[i + 1].second;
            double dy1 = derivatives[i + 1];

            double y = f(z, x0, x1, y0, dy0, y1, dy1);

            if (z == data[i].first) {
                data_result.push_back(std::pair<double, double>(x0, y0));
                derivatives_result.push_back(dy0);
            }
            else {
                data_result.push_back(std::pair<double, double>(z, y));
                derivatives_result.push_back((dy1 + dy0) / 2);
            }
        }
    }

    data_result.push_back(std::pair<double, double>((data.end() - 1)->first, (data.end() - 1)->second));
    derivatives_result.push_back(*(derivatives.end() - 1));

    data.clear();
    derivatives.clear();
    data.insert(data.end(), data_result.begin(), data_result.end());
    derivatives.insert(derivatives.end(), derivatives_result.begin(), derivatives_result.end());

    //std::reverse(data.begin(), data.end());
}

void HermiteInterpolator::draw(juce::Graphics& g) {

    /* --------------------- */
    /* HERMITE INTERPOLATION */
    /* --------------------- */

    int i = 0;
    std::vector<juce::Line<float>> lines_hermite;
    for (auto iter = data.begin(); iter != data.end() - 1; iter++) {
        juce::Point<float> a(iter->first * 100 + 200, iter->second * 100);
        juce::Point<float> b((iter + 1)->first * 100 + 200, (iter + 1)->second * 100);
        lines_hermite.push_back(juce::Line<float>(a, b));
        g.drawRoundedRectangle(a.getX(), a.getY(), 5, 5, 5, 5);
        g.drawRoundedRectangle(b.getX(), b.getY(), 5, 5, 5, 5);
    }

    for (auto iter = lines_hermite.begin(); iter != lines_hermite.end(); iter++) {
        g.drawLine(*iter, 2.0f);
    }
}