#pragma once

#include "SettingsPage.g.h"

namespace winrt::SIPADPBT::implementation
{
    struct SettingsPage : SettingsPageT<SettingsPage>
    {
        SettingsPage();

        Windows::Foundation::IAsyncAction changeToLightTheme(Windows::Foundation::IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&);
        Windows::Foundation::IAsyncAction changeToDarkTheme(Windows::Foundation::IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&);
        Windows::Foundation::IAsyncAction changeToLeftNavStyle(Windows::Foundation::IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&);
        Windows::Foundation::IAsyncAction changeToTopNavStyle(Windows::Foundation::IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&);
        
        nlohmann::json settings_content;
        Windows::Storage::StorageFile settings_file = nullptr;
    
    };
}

namespace winrt::SIPADPBT::factory_implementation
{
    struct SettingsPage : SettingsPageT<SettingsPage, implementation::SettingsPage>
    {
    };
}
