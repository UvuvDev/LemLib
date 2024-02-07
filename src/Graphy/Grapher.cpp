#include "Graphy/Grapher.hpp"
#include "pros/colors.h"
#include <memory>

namespace graphy {

AsyncGrapher::AsyncGrapher(const std::string &title, const uint refreshRate) {
    this->title = title;
    this->refreshRate = refreshRate;
    cnt = 0;
}

void AsyncGrapher::addDataType(const std::string &name, const uint32_t color) {
    if (cnt > MAX_DATA) {
        std::runtime_error("Error: max number of data is 14");
    } else {
        cnt++;
        container.insert({name, {0.0}});
        colors.insert({name, color});
    }
}

void AsyncGrapher::update(const std::string &name, double val) {
    container[name].push_back(val);
    if (container[name].size() > MAX_CACHE_SIZE) {
        container[name].erase(container[name].begin());
    }
}

void AsyncGrapher::setRefreshRate(const uint refreshRate) {
    this->refreshRate = refreshRate;
}

uint AsyncGrapher::getRefreshRate() {
    return this->refreshRate;
}

void AsyncGrapher::loop() {
    while (true) {
        // Reset screen
        pros::screen::erase();

        // Draw graph
        pros::screen::set_pen(pros::c::COLOR_WHITE);
        pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, 1, title.c_str());
        pros::screen::draw_line(GRAPH_LEFT, GRAPH_TOP, GRAPH_LEFT, GRAPH_BOTTOM);
        pros::screen::draw_line(
          GRAPH_LEFT, GRAPH_BOTTOM, GRAPH_LEFT + MAX_CACHE_SIZE, GRAPH_BOTTOM);

        int indexLine = 0;
        for (const auto &item : container) {
            pros::screen::set_pen(colors[item.first]);
            pros::screen::print(pros::text_format_e_t::E_TEXT_SMALL,
                                GRAPH_LEFT + MAX_CACHE_SIZE,
                                (++indexLine) * 14 + 30,
                                item.first.c_str());
            for (int i = 0; i < item.second.size()-1; i++) {
                double val1 = item.second[i] * (GRAPH_BOTTOM - GRAPH_TOP);
                double val2 = item.second[i + 1] * (GRAPH_BOTTOM - GRAPH_TOP);
                pros::screen::draw_line(
                  GRAPH_LEFT + i, GRAPH_BOTTOM - val1, GRAPH_LEFT + i + 1, GRAPH_BOTTOM - val2);
            }
        }

        pros::delay(this->refreshRate);
    }
}

std::map<std::string, std::vector<double>>* AsyncGrapher::getContainer() {

    return &container;
    
}

void AsyncGrapher::insertNewGraph(std::shared_ptr<std::pair<std::string, std::vector<double>>> newGraph) {    

    if (cnt > MAX_DATA) {
        std::runtime_error("Error: max number of data is 14");
    } else {
        cnt++;
        container.insert({newGraph->first, newGraph->second});
        colors.insert({newGraph->first, pros::c::COLOR_CORNFLOWER_BLUE});
    }

}

}  // namespace graph