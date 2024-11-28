#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif
#include "Model.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::SIPADPBT::implementation
{

    MainWindow::MainWindow() {

        
        this->ExtendsContentIntoTitleBar(true);
        this->InitializeComponent();
        this->TitleBarTextBlock().Text(
            L"SIPADPBT"
        );
        this->SetTitleBar(
            this->AppTitleBar()
        );
        this->AppTitleBar().Loaded([this](
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e
        ) -> Windows::Foundation::IAsyncAction {
            if (this->ExtendsContentIntoTitleBar()) {
                this->SetRegionsForCustomTitleBar();
            }
            
            auto rootElement = this->RootElement();

            // defeault settings
            nlohmann::json settings_data = R"({
                "default_theme": "Dark",
                "navigation_style":  "Left"
            })"_json;

            // check if settings.json file has made
            // if not creating it

            Windows::Storage::StorageFolder localStorageFolder{
                Windows::Storage::ApplicationData::Current().LocalFolder()
            };

            Windows::Storage::IStorageItem settings{ co_await localStorageFolder.TryGetItemAsync(L"settings.json") };
            if (!settings) {
                co_await localStorageFolder.CreateFileAsync(
                    L"settings.json",
                    Windows::Storage::CreationCollisionOption::ReplaceExisting
                );
                Windows::Storage::StorageFile settings_file{ co_await localStorageFolder.GetFileAsync(L"settings.json")};
                co_await Windows::Storage::FileIO::WriteTextAsync(
                    settings_file,
                    winrt::to_hstring(settings_data.dump())
                );
            }
            else {
                Windows::Storage::StorageFile settings_file{ co_await localStorageFolder.GetFileAsync(L"settings.json") };
                winrt::hstring raw_settings_text = co_await Windows::Storage::FileIO::ReadTextAsync(
                    settings_file
                );
                settings_data = nlohmann::json::parse(
                    winrt::to_string(raw_settings_text)
                );
            }

            // set default theme which load from 'settings.json' file
            if (strcmp((settings_data["default_theme"].template get<std::string>()).c_str(), "Dark") == 0) {
                rootElement.RequestedTheme(winrt::Microsoft::UI::Xaml::ElementTheme::Dark);
                this->switch_theme().IsOn(true);
            }
            else if (strcmp((settings_data["default_theme"].template get<std::string>()).c_str(), "Light") == 0) {
                rootElement.RequestedTheme(winrt::Microsoft::UI::Xaml::ElementTheme::Light);
                this->switch_theme().IsOn(false);
            }

            if (strcmp((settings_data["navigation_style"].template get<std::string>()).c_str(), "Left") == 0) {
                this->NavView().PaneDisplayMode(Microsoft::UI::Xaml::Controls::NavigationViewPaneDisplayMode::Left);
            }
            else if (strcmp((settings_data["navigation_style"].template get<std::string>()).c_str(), "Top") == 0) {
                this->NavView().PaneDisplayMode(Microsoft::UI::Xaml::Controls::NavigationViewPaneDisplayMode::Top);
            }

        });
        this->AppTitleBar().SizeChanged([this](
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e
        ) -> void {
            if (this->ExtendsContentIntoTitleBar()) {
                this->SetRegionsForCustomTitleBar();
            }
        });

    }

    void MainWindow::ContentFrame_NavigationFailed(
        winrt::Windows::Foundation::IInspectable const& /*sender*/,
        winrt::Microsoft::UI::Xaml::Navigation::NavigationFailedEventArgs const& args
    ) {
        throw winrt::hresult_error{
            E_FAIL,
            winrt::hstring(L"Failed to load page ") + args.SourcePageType().Name
        };
    };

    void MainWindow::NavView_Loaded(
        winrt::Windows::Foundation::IInspectable const& /*sender*/,
        winrt::Microsoft::UI::Xaml::RoutedEventArgs const& /*args*/
    ) {
        // Add handler for content frame navigation
        this->ContentFrame().Navigated({ this,&MainWindow::On_Navigated });

        //NavView Does not load amy page by default, so load homepage
        this->NavView().SelectedItem(
            this->NavView().MenuItems().GetAt(0)
        );

        this->NavView_Navigate(
            winrt::xaml_typename<SIPADPBT::HomePage>(),
            winrt::Microsoft::UI::Xaml::Media::Animation::EntranceNavigationTransitionInfo()
        );
    };

    void MainWindow::NavView_ItemInvoked(
        winrt::Windows::Foundation::IInspectable const& /*sender*/,
        muxc::NavigationViewItemInvokedEventArgs const& args
    ) {

        if (args.IsSettingsInvoked()) {
            this->NavView_Navigate(
                winrt::xaml_typename<SIPADPBT::SettingsPage>(),
                args.RecommendedNavigationTransitionInfo()
            );
        }
        else if (args.InvokedItemContainer()) {
            winrt::Windows::UI::Xaml::Interop::TypeName pageTypeName;
            pageTypeName.Name = unbox_value<winrt::hstring>(args.InvokedItemContainer().Tag());
            pageTypeName.Kind = winrt::Windows::UI::Xaml::Interop::TypeKind::Primitive;
            this->NavView_Navigate(
                pageTypeName,
                args.RecommendedNavigationTransitionInfo()
            );
        }

    };

    void MainWindow::NavView_Navigate(
        winrt::Windows::UI::Xaml::Interop::TypeName navPageType,
        winrt::Microsoft::UI::Xaml::Media::Animation::NavigationTransitionInfo const& transitionInfo
    ) {
        winrt::Windows::UI::Xaml::Interop::TypeName prevNavTypeName = this->ContentFrame().CurrentSourcePageType();

        if (navPageType.Name != L"" && prevNavTypeName.Name != navPageType.Name) {
            this->ContentFrame().Navigate(
                navPageType,
                nullptr,
                transitionInfo
            );
        }
    };

    void MainWindow::NavView_BackRequested(
        muxc::NavigationView const& /*sender*/,
        muxc::NavigationViewBackRequestedEventArgs const& /*args*/
    ) {

        this->TryGoBack();

    };

    bool MainWindow::TryGoBack() {
        if (!this->ContentFrame().CanGoBack()) {
            return false;
        }
        else if (this->NavView().IsPaneOpen() &&
            (this->NavView().DisplayMode() == muxc::NavigationViewDisplayMode::Compact ||
                this->NavView().DisplayMode() == muxc::NavigationViewDisplayMode::Minimal)) {
            return false;
        }

        this->ContentFrame().GoBack();
        return true;
    };

    void MainWindow::On_Navigated(
        winrt::Windows::Foundation::IInspectable const& /*sender*/,
        winrt::Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& args
    ) {

        this->NavView().IsBackEnabled(
            this->ContentFrame().CanGoBack()
        );

        if (this->ContentFrame().SourcePageType().Name == winrt::xaml_typename<SIPADPBT::SettingsPage>().Name) {
            this->NavView().SelectedItem(
                this->NavView().SettingsItem().as<muxc::NavigationViewItem>()
            );
        }
        else if (
            this->ContentFrame().SourcePageType().Name != L""
            ) {

            for (auto&& eachMenuItem : this->NavView().MenuItems()) {
                auto navigationViewItem = eachMenuItem.try_as<muxc::NavigationViewItem>();
                {
                    if (navigationViewItem) {
                        winrt::hstring hstringValue = winrt::unbox_value_or<winrt::hstring>(
                            navigationViewItem.Tag(),
                            L""
                        );

                        if (hstringValue == this->ContentFrame().SourcePageType().Name) {
                            this->NavView().SelectedItem(navigationViewItem);
                        }
                    }
                }
            }

        }
    };

    winrt::Windows::Graphics::RectInt32 MainWindow::GetRect(winrt::Windows::Foundation::Rect bounds, double scale) {

        return winrt::Windows::Graphics::RectInt32{
            (int32_t)(bounds.X * scale),
            (int32_t)(bounds.Y * scale),
            (int32_t)(bounds.Width * scale),
            (int32_t)(bounds.Height * scale)
        };

    };

    void MainWindow::SetRegionsForCustomTitleBar() {

        double scaleAdjustmen = this->AppTitleBar().XamlRoot().RasterizationScale();
        this->RightPaddingColumn().Width(
            winrt::Microsoft::UI::Xaml::GridLength{
                this->AppWindow().TitleBar().RightInset() / scaleAdjustmen
            }
        );
        this->RightPaddingColumn().Width(
            winrt::Microsoft::UI::Xaml::GridLength{
                this->AppWindow().TitleBar().LeftInset() / scaleAdjustmen
            }
        );

        winrt::Microsoft::UI::Xaml::Media::GeneralTransform transform = this->switch_theme().TransformToVisual(nullptr);
        winrt::Windows::Foundation::Rect bounds = transform.TransformBounds(
            winrt::Windows::Foundation::Rect{
                0,0,
                (float)this->switch_theme().ActualWidth(),
                (float)this->switch_theme().ActualHeight()
            }
        );

        winrt::Windows::Graphics::RectInt32 switchThemeRect = this->GetRect(bounds, scaleAdjustmen);
        winrt::Windows::Graphics::RectInt32 rectArray[] = {
            switchThemeRect
        };

        winrt::Microsoft::UI::Input::InputNonClientPointerSource nonInputClientSource =
            winrt::Microsoft::UI::Input::InputNonClientPointerSource::GetForWindowId(
                this->AppWindow().Id()
            );

        nonInputClientSource.SetRegionRects(winrt::Microsoft::UI::Input::NonClientRegionKind::Passthrough, rectArray);

    }


    void MainWindow::switch_theme_Toggled(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {

        auto rootElement = this->RootElement();

        if (this->switch_theme().IsOn()) {
            rootElement.RequestedTheme(winrt::Microsoft::UI::Xaml::ElementTheme::Dark);
        }
        else {
            rootElement.RequestedTheme(winrt::Microsoft::UI::Xaml::ElementTheme::Light);
        }
    }

}
