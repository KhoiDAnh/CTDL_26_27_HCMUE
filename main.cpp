#include "ui/App.h"

#ifndef DATA_DIR
#define DATA_DIR "data/"
#endif

int main() {
    App app(DATA_DIR);
    return app.run();
}