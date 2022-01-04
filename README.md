# Livid

Livid is a single header C++ wasm frontend library leveraging Emscripten.

## Usage

The code looks something like this:
```cpp
#include <string>
#include "livid/livid.hpp"

using namespace livid;

class AppState {
    static int counter;
    public:
    static void increment() {
        counter += 1;
        auto result = Widget<WidgetType::Div>::from_id("result");
        result.text(std::to_string(counter));
    }
    static void decrement() {
        counter -= 1;
        auto result = Widget<WidgetType::Div>::from_id("result");
        result.text(std::to_string(counter));
    }
};

int AppState::counter = 0;

WASM_EXPORT void inc(void) {
    AppState::increment();
}

WASM_EXPORT void dec(void) {
    AppState::decrement();
}

int main() {
    Document::title("Hello");

    Widget<WidgetType::Div> div("mydiv"); // The constructor takes an id, which needs to be unique and without spaces

    Widget<WidgetType::Button> btn1("btn_inc");
    btn1.text("Increment!"); // This sets the textContent element property
    btn1.style("color", "green");
    btn1.handle("click", "inc"); // This signals that clicks call the inc function
    div.append(btn1); // widgets are automatically appended to body, here we want to append to the div

    Widget<WidgetType::Button> btn2("btn_dec");
    btn2.text("Decrement!");
    btn2.style("color", "red");
    btn2.handle("click", "dec");
    div.append(btn2);

    Widget<WidgetType::Div> result("result");
    result.text("0");

    auto elems = Document::elems_by_tag("DIV");
    
    for (auto &elem: elems) {
        elem.style("color", "black");
    }
}
```
You can also use a builder pattern:
```cpp
#include "livid/livid.hpp"
#include <string>

using namespace livid;

using Div = Widget<WidgetType::Div>;
using Form = Widget<WidgetType::Form>;
using Label = Widget<WidgetType::Label>;
using Input = Widget<WidgetType::Input>;
using Button = Widget<WidgetType::Button>;

int main() {
    Form("box").klass("box").append(
        Div("field1")
            .klass("field") // the class attribute is used by many css libs for styling elements of the same class
            .append(Label("email").klass("label").text("Email"))
            .append(
                Div("control1").klass("control").append(
                    Input("input1")
                        .klass("input")
                        .type("email")
                        .attr("placeholder", "m@gmail.com")
                )
            )
            .append(
                Div("field2")
                    .klass("field")
                    .append(Label("pass").klass("label").text("Password"))
                    .append(
                        Div("control2").klass("control").append(
                            Input("input2")
                                .klass("input")
                                .type("password")
                                .attr("placeholder", "*******")
                        )
                    )
            )
            .append(Button("button").klass("button is-primary").text("Sign in"))
    );
}
```
This uses Bulma for css styling.
![image](https://user-images.githubusercontent.com/37966791/147970535-12542b64-94e2-4660-86d1-43846d9ce92f.png)

## Building

Assuming you have a working installation of Emscripten:

If you clone this repo, from the root you can directly invoke em++ to build any of the examples:
```
$ em++ -std=c++17 -Iinclude examples/counter.cpp -s EXTRA_EXPORTED_RUNTIME_METHODS=['UTF8ToString','lengthBytesUTF8','stringToUTF8'] -o index.html --shell-file my_shell.html
```

With CMake:
You need a CMakeLists.txt file with contents similar to:
```cmake
cmake_minimum_required(VERSION 3.15)
project(cmake_livid_example)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-s WASM=1 -s EXTRA_EXPORTED_RUNTIME_METHODS=['UTF8ToString','lengthBytesUTF8','stringToUTF8']")

include(FetchContent)
FetchContent_Declare(
    LIVID
    GIT_REPOSITORY https://github.com/MoAlyousef/livid
    GIT_TAG main
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(LIVID)

add_executable(index main.cpp)
set_target_properties(index PROPERTIES SUFFIX .html)
target_compile_features(index PRIVATE cxx_std_17)
target_link_options(index PRIVATE --shell-file ${CMAKE_CURRENT_LIST_DIR}/my_shell.html)
target_link_libraries(index PRIVATE livid::livid)
```
Then configure with `emcmake cmake -Bbin`, and build with `cmake --build bin`.
The build usually outputs 3 files, and html, wasm and js files, you need all 3 to run your program.
Building in release mode (`-O3` or `-DCMAKE_BUILD_TYPE=Release`), the total size of a simple project is around 30kb.

## Running

Opening the html file directly in the browser won't work.
To run, you need a server to serve the generated html file:
```
$ python3 -m http.server --directory .
```
Or you can use `emrun` which is provided by emscripten.

emrun opens a browser automatically, if you use another server program, you need to open your wasm-capable browser to the provided url, which is usually something like 127.0.0.1:8000

## Html shell

You'll notice that the repo has a minimal shell which you can use, it's passed as an argument to emscripten. You can replace it with whatever shell you prefer, and include css etc. The login example uses Bulma for CSS:
```html
<!doctype html>
<html lang="en-us">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bulma@0.9.3/css/bulma.min.css">
    <title>My app</title>
  </head>
  {{{ SCRIPT }}}
  </body>
</html>
```

## Todo
- Figure out a way to capture variable in callbacks!