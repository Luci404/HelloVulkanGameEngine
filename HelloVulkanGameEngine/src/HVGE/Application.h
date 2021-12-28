#pragma once

#include "HVGE/Window.h"

namespace HVGE
{
    class Application
    {
    public:
        Application();
        ~Application();

        void Run();

    private:
        Window m_Window;
    };
}