#include <vector>
#include <math_and_geometry/largest_triangle_area.hpp>
#include <cmath> //for std::abs
#include <algorithm> // for std::max



// Implement your largest_triangle_area logic here.
static double cross(const Point2D & u, const Point2D& v){
    // think about u and v as vector u=(u_x,u_y) and v=(v_x,v_y) becasue we call cross as A-B that is vector thatnks to definition of - operator
    return u.x*v.y-u.y*v.x;
}


static double triangleArea(const Point2D & A, const Point2D & B, const Point2D & C){
    Point2D vectU=B-A;
    Point2D vectV=C-A;
    return 0.5*std::abs(cross(vectU, vectV));
}


double largestTriangleArea(std::vector<std::vector<int>> &points){
    double maxArea=0;
    std::vector<Point2D> vectOfPoints;
    for (const auto & p:points){
        vectOfPoints.push_back(Point2D{static_cast<double>(p[0]),static_cast<double>(p[1])});
         
    }
    int n=vectOfPoints.size();
    for (int i=0;i<n;++i){
        for (int j=i+1;j<n;++j){
            for (int k=j+1;k<n;++k){
                maxArea=std::max(maxArea,triangleArea(vectOfPoints[i], vectOfPoints[j], vectOfPoints[k]));
            }
        }
    }
    return maxArea;
        
}
