#pragma once

#include "Explaination.g.h"

namespace winrt::SIPADPBT::implementation
{
    struct Explaination : ExplainationT<Explaination>
    {
        Explaination();
        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);
        winrt::hstring m_dataToExplain;
    };
}

namespace winrt::SIPADPBT::factory_implementation
{
    struct Explaination : ExplainationT<Explaination, implementation::Explaination>
    {
    };
}
