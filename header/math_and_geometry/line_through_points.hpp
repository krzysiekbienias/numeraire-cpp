#pragma once
#include<vector>

// Declare your line_through_points interface here.
struct Point{
    int x,y;
    
    Point(int x_, int y_):x(x_),y(y_){};
    
};

std::vector<Point> toPoint(const std::vector<std::vector<int>>& points);


int lineThroughPoints(const std::vector<std::vector<int>>& points);


