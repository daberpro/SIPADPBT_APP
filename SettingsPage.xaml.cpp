#include "pch.h"
#include "SettingsPage.xaml.h"
#if __has_include("SettingsPage.g.cpp")
#include "SettingsPage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::SIPADPBT::implementation
{
    SettingsPage::SettingsPage() {
        this->InitializeComponent();
        this->Loaded([&](IInspectable const&, RoutedEventArgs const&)-> Windows::Foundation::IAsyncAction {
            Windows::Storage::StorageFolder appInstallationPath = Windows::ApplicationModel::Package::Current().InstalledLocation();
            Windows::Storage::StorageFolder assetsFolder = co_await appInstallationPath.GetFolderAsync(L"Assets");
            Windows::Storage::StorageFolder coreFolder = co_await assetsFolder.GetFolderAsync(L"Core");
            Windows::Storage::StorageFile settingsFile = co_await coreFolder.GetFileAsync(L"settings.json");

            Windows::Storage::Streams::IRandomAccessStream stream = co_await settingsFile.OpenAsync(Windows::Storage::FileAccessMode::Read);
            Windows::Storage::Streams::DataReader reader{ stream.GetInputStreamAt(0) };

            uint32_t size = static_cast<uint32_t>(stream.Size());
            co_await reader.LoadAsync(size);
            this->settings_file = settingsFile;
            nlohmann::json settings_data = nlohmann::json::parse(reader.ReadString(size));
            this->settings_content = settings_data;
            this->theme_style().Content(
                winrt::box_value(
                    winrt::to_hstring(
                        (settings_data["default_theme"].template get<std::string>())
                    )
                )
            );
            this->navigation_style().Content(
                winrt::box_value(
                    winrt::to_hstring(
                        (settings_data["navigation_style"].template get<std::string>())
                    )
                )
            );
        });
    }

    Windows::Foundation::IAsyncAction SettingsPage::changeToLightTheme(Windows::Foundation::IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&) {
        auto& parentOfFrame = this->Frame().Parent().as<Microsoft::UI::Xaml::Controls::NavigationView>();
        auto& rootElement = parentOfFrame.Parent().as<Microsoft::UI::Xaml::Controls::Grid>();
        rootElement.RequestedTheme(winrt::Microsoft::UI::Xaml::ElementTheme::Light);

        this->theme_style().Content(winrt::box_value(L"Light"));
        this->settings_content["default_theme"] = "Light";
        co_await Windows::Storage::FileIO::WriteTextAsync(
            this->settings_file,
            winrt::to_hstring(this->settings_content.dump())
        );

    };
    Windows::Foundation::IAsyncAction SettingsPage::changeToDarkTheme(Windows::Foundation::IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&) {
        auto& parentOfFrame = this->Frame().Parent().as<Microsoft::UI::Xaml::Controls::NavigationView>();
        auto& rootElement = parentOfFrame.Parent().as<Microsoft::UI::Xaml::Controls::Grid>();
        rootElement.RequestedTheme(winrt::Microsoft::UI::Xaml::ElementTheme::Dark);

        this->theme_style().Content(winrt::box_value(L"Dark"));
        this->settings_content["default_theme"] = "Dark";
        co_await Windows::Storage::FileIO::WriteTextAsync(
            this->settings_file,
            winrt::to_hstring(this->settings_content.dump())
        );
    };
    Windows::Foundation::IAsyncAction SettingsPage::changeToLeftNavStyle(Windows::Foundation::IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&) {
        auto& NavView = this->Frame().Parent().as<Microsoft::UI::Xaml::Controls::NavigationView>();
        NavView.PaneDisplayMode(Microsoft::UI::Xaml::Controls::NavigationViewPaneDisplayMode::Left);
        
        this->navigation_style().Content(winrt::box_value(L"Left"));
        this->settings_content["navigation_style"] = "Left";
        co_await Windows::Storage::FileIO::WriteTextAsync(
            this->settings_file,
            winrt::to_hstring(this->settings_content.dump())
        );
    
    };
    Windows::Foundation::IAsyncAction SettingsPage::changeToTopNavStyle(Windows::Foundation::IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&) {
        auto& NavView = this->Frame().Parent().as<Microsoft::UI::Xaml::Controls::NavigationView>();
        NavView.PaneDisplayMode(Microsoft::UI::Xaml::Controls::NavigationViewPaneDisplayMode::Top);
    
        this->navigation_style().Content(winrt::box_value(L"Top"));
        this->settings_content["navigation_style"] = "Top";
        co_await Windows::Storage::FileIO::WriteTextAsync(
            this->settings_file,
            winrt::to_hstring(this->settings_content.dump())
        );
    };
}
