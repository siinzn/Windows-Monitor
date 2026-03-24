#include "Monitor.h"
#include <thread>
#include <chrono>

int main()
{
    BOOL running = TRUE;
    Monitor monitor;
    while (running)
    {
        monitor.update();
        monitor.render();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

