#include "livid/livid.hpp"
#include <string>

using namespace livid;

int main() {
    int count = 0;

    // This sets the document title
    Document::title("Hello");

    Widget<WidgetType::Div> div;

    Widget<WidgetType::Button> btn1;
    // This sets the textContent element property
    btn1.text("Increment!");
    btn1.style("color", "green");
    // This signals that clicks call the inc function
    btn1.handle("click", [&](){
        count += 1;
        auto result = Widget<WidgetType::Div>::from_id("result");
        result.text(std::to_string(count));
    });
    // widgets are automatically appended to body, here we want to append to the div
    div.append(btn1);

    Widget<WidgetType::Button> btn2;
    btn2.text("Decrement!");
    btn2.style("color", "red");
    btn2.handle("click", [&](){
        count -= 1;
        auto result = Widget<WidgetType::Div>::from_id("result");
        result.text(std::to_string(count));
    });
    div.append(btn2);

    Widget<WidgetType::Div> result;
    // We set the id to conveniently access the widget by id in the callback
    result.id("result");
    result.text("0");
    result.style("fontSize", "22px");

    // we get all elements by tagName BUTTON
    auto elems = Document::elems_by_tag("BUTTON");
    for (auto &elem : elems) {
        // set their fontSize to 22 pixesl
        elem.style("fontSize", "22px");
    }
}