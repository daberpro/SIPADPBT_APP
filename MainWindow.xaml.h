#pragma once

#include "MainWindow.g.h"

namespace muxc {
    using namespace winrt::Microsoft::UI::Xaml::Controls;
}

namespace winrt::SIPADPBT::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();

        void ContentFrame_NavigationFailed(
            winrt::Windows::Foundation::IInspectable const& /*sender*/,
            winrt::Microsoft::UI::Xaml::Navigation::NavigationFailedEventArgs const& args
        );

        void NavView_Loaded(
            winrt::Windows::Foundation::IInspectable const& /*sender*/,
            winrt::Microsoft::UI::Xaml::RoutedEventArgs const& /*args*/
        );

        void NavView_ItemInvoked(
            winrt::Windows::Foundation::IInspectable const& /*sender*/,
            muxc::NavigationViewItemInvokedEventArgs const& args
        );

        void NavView_Navigate(
            winrt::Windows::UI::Xaml::Interop::TypeName navPageType,
            winrt::Microsoft::UI::Xaml::Media::Animation::NavigationTransitionInfo const& transitionInfo
        );

        void NavView_BackRequested(
            muxc::NavigationView const& /*sender*/,
            muxc::NavigationViewBackRequestedEventArgs const& /*args*/
        );

        void On_Navigated(
            winrt::Windows::Foundation::IInspectable const& /*sender*/,
            winrt::Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& args
        );

        bool TryGoBack();

        winrt::Windows::Graphics::RectInt32 GetRect(winrt::Windows::Foundation::Rect bounds, double scale);
        void SetRegionsForCustomTitleBar();

        void switch_theme_Toggled(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::SIPADPBT::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
