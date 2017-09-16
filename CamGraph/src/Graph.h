#pragma once

#include "ofMain.h"

#include <limits>

class Plot {
public:
    Plot(float _x, float _y) : x(_x), y(_y) {
    }
    
    float x;
    float y;
};

class Graph {
public:
    
    void setColor(const ofColor &_color) {
        color = _color;
    }
    
    void setMarginY(const float &_minMargin, const float &_maxMargin) {
        yminMargin = _minMargin;
        ymaxMargin = _maxMargin;
    }
    
    void clear() {
        data.clear();
        
        float minValue = numeric_limits<float>::max();
        float maxValue = numeric_limits<float>::min();
        
        xmin = minValue;
        xmax = maxValue;
        ymin = minValue;
        ymax = maxValue;
    }
    
    void addValue(const float &x, const float &y, bool calcRange = true) {
        
        Plot value(x, y);
        data.push_back(value);
        
        if (calcRange) {
            xmin = min(xmin, x);
            xmax = max(xmax, x);
            ymin = min(ymin, y);
            ymax = max(ymax, y);
        }
        
    }
    
    void setRangeX(const float &_xmin, const float &_xmax) {
        xmin = _xmin;
        xmax = _xmax;
    }
    
    void setRangeY(const float &_ymin, const float &_ymax) {
        ymin = _ymin;
        ymax = _ymax;
    }
    
    float getMaxY() {
        return ymax;
    }
    
    void draw(const ofRectangle &rect) {
        
        ofSetColor(color);
        
        shape.clear();
        
        ymin -= yminMargin;
        ymax += ymaxMargin;
        
        for (auto plot : data) {
            float x = rect.x + rect.width  * getRatio(plot.x, xmin, xmax);
            float y = rect.y + rect.height * getRatio(plot.y, ymax, ymin);
            shape.addVertex(x, y);
            ofDrawCircle(x, y, 3);
        }
        
        shape.draw();
    }
    
    
    
private:
                                       
   float getRatio(float t, float a, float b) {
       return (t - a) / (b - a);
   }
                                       
    
    ofPolyline shape;
    
    vector<Plot> data;
    
    ofColor color;
    
    float xmin, xmax, ymin, ymax;
    float yminMargin = 0, ymaxMargin = 0;
    
};
