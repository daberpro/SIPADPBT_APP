#pragma once

#include "About.g.h"

namespace winrt::SIPADPBT::implementation
{
    struct About : AboutT<About>
    {
        About()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }


       
    };
}

namespace winrt::SIPADPBT::factory_implementation
{
    struct About : AboutT<About, implementation::About>
    {
    };
}
