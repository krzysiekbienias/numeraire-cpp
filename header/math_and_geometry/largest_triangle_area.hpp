#pragma once
#include<vector>



struct Point2D{
    double x,y;
    Point2D operator - (const Point2D &other) const {
        return {x-other.x,y-other.y}; //it creates new Point that we interpret as a vectro we just not introduce new class vecctor to not overcomplicate.
        
    }
};


double largestTriangleArea(std::vector<std::vector<int>> & points);
