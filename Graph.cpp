#include "Graph.h"

Graph::Graph() {

}

Graph::Graph(
	juce::Rectangle<int> new_region,
	std::vector<std::pair<double, double>>* new_data,
	juce::String new_title,
	juce::String new_x_label,
	juce::String new_y_label
):	
	region(new_region), 
	region_graph(new_region), 
	title(new_title), 
	x_label(new_x_label),
	y_label(new_y_label),
	x_margin(-50), 
	y_margin(-40),
	data(new_data)
{
	region_graph.expand(x_margin, y_margin);
}

Graph::~Graph() {

}

void Graph::setData(std::vector<std::pair<double, double>>* new_data) {
	data = new_data;
}

void Graph::paint(juce::Graphics& g) {
	g.fillAll(juce::Colours::black);

	float max_x = FLT_MIN;
	float min_x = FLT_MAX;
	float max_y = FLT_MIN;
	float min_y = FLT_MAX;
	int lineH = 1;

	for (auto iter : *data) {
		if (iter.first > max_x)
			max_x = iter.first;
		if (iter.second > max_y)
			max_y = iter.second;
		if (iter.first < min_x)
			min_x = iter.first;
		if (iter.second < min_y)
			min_y = iter.second;
	}

	int dx = (max_x - min_x) / 10;
	int dy = (max_y - min_y) / 10;
	float scale_x = region_graph.getWidth() / ((max_x == min_x ? 0.0001 : max_x - min_x) * 1.10);
	float scale_y = region_graph.getHeight() / ((max_y == min_y ? 0.0001 : max_y - min_y) * 1.10);

	// Draw graph lines
	g.setColour(juce::Colours::grey);
	g.drawRect(region_graph, 2);

	g.setFont(juce::Font(20.0));
	g.setColour(juce::Colours::grey);
	g.drawText(title, region, juce::Justification::centredTop, true);

	g.setFont(juce::Font(16.0));
	g.setColour(juce::Colours::grey);
	g.drawText(x_label, region, juce::Justification::centredBottom, true);
	g.drawText(y_label, region, juce::Justification::centredLeft, true);

	// Drawing x-axis
	g.setFont(juce::Font(16.0));
	g.setColour(juce::Colours::grey);
	for (int i = 0; i < 11; i++)
	{
		int x = (scale_x * i * dx) + region_graph.getBottomLeft().getX();
		int y = region_graph.getBottomLeft().getY();
		int y0 = region_graph.getTopLeft().getY();
		int value = min_x + (dx * i);
		juce::Line<float> line(x, y, x, y0);
		float len[] = { 4, 2 };
		g.drawDashedLine(line, len, 2);
		g.drawLine(x, y - 5, x, y + 5, 2);
		g.drawSingleLineText(juce::String(value), x, y + 20, juce::Justification::left);
	}

	// Drawing y-axis
	for (int i = 0; i < 11; i++)
	{
		int x = region_graph.getTopLeft().getX();
		int y = region_graph.getHeight() - (scale_y * i * dy) + region_graph.getTopLeft().getY();
		int x0 = region_graph.getTopRight().getX();
		int value = min_y + (dy * i);
		juce::Line<float> line(x, y, x0, y);
		float len[] = { 4, 2 };
		g.drawDashedLine(line, len, 2);
		g.drawLine(x - 5, y, x + 5, y, 2);
		g.drawSingleLineText(juce::String(value), x - 5, y, juce::Justification::right);
	}

	{
		// Setting scale
		g.setColour(juce::Colours::white);
		int x = (scale_x * ((*data)[0].first - min_x)) + region_graph.getX();
		int y = region_graph.getHeight() - (scale_y * ((*data)[0].second - min_y)) + region_graph.getY();
		int prev_x = x;
		int prev_y = y;

		// Drawing lines
		g.setColour(juce::Colours::white);
		for (auto iter : *data) {
			x = (scale_x * (iter.first - min_x)) + region_graph.getX();
			y = region_graph.getHeight() - (scale_y * (iter.second - min_y)) + region_graph.getY();
			g.drawLine(prev_x, prev_y, x, y, 2);
			g.fillRect(x - 2, y - 2, 5, 5);
			prev_x = x;
			prev_y = y;
		}
	}
}

void Graph::resized() {

}