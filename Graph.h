#include <JuceHeader.h>
#include <vector>
#include <thread>

class Graph : public juce::Component
{
public:
    Graph();
    Graph(
        juce::Rectangle<int> new_region, 
        std::vector<std::pair<double, double>>* new_data,
        juce::String new_title = "Interpolation",
        juce::String new_x_label = "X",
        juce::String new_y_label = "Y"
    );
    ~Graph() override;

    void setData(std::vector<std::pair<double, double>>* data);
    void paint(juce::Graphics& g) override;
    void resized() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Graph);

private:
    juce::Rectangle<int> region;
    juce::Rectangle<int> region_graph;
    juce::String title;
    juce::String x_label;
    juce::String y_label;
    int x_margin;
    int y_margin;
    std::vector<std::pair<double, double>>* data;
};