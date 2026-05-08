#include "app/ConsoleApplication.hpp"
#include "di/AppModule.hpp"

int main() {
    const auto injector = createAppInjector();
    const auto app = injector.create<ConsoleApplication>();
    app.run();

    return 0;
}
