#include "pch.h"
#include "Explaination.xaml.h"
#if __has_include("Explaination.g.cpp")
#include "Explaination.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::SIPADPBT::implementation
{
    Explaination::Explaination() {
        this->InitializeComponent();
        this->web().NavigationCompleted([&](Microsoft::UI::Xaml::Controls::WebView2 const& sender, Microsoft::Web::WebView2::Core::CoreWebView2NavigationCompletedEventArgs const&)-> Windows::Foundation::IAsyncAction{
            co_await sender.ExecuteScriptAsync(L"explain("+this->m_dataToExplain+ L")");
        });
       
        this->Loaded([&](IInspectable const&, RoutedEventArgs const&)-> Windows::Foundation::IAsyncAction {
            Windows::Storage::StorageFolder applicationInstallationPath = Windows::ApplicationModel::Package::Current().InstalledLocation();
            Windows::Storage::StorageFolder assetsPath = co_await applicationInstallationPath.GetFolderAsync(L"Assets");
            Windows::Storage::StorageFolder explainPath = co_await assetsPath.GetFolderAsync(L"Explain");
            
            co_await this->web().EnsureCoreWebView2Async();
            this->web().DefaultBackgroundColor(Microsoft::UI::Colors::Transparent());
            this->web().Source(Windows::Foundation::Uri(explainPath.Path().c_str()+winrt::to_hstring("/index.html")));
        });
    }

    void Explaination::OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e) {
        
        auto propertyValue = e.Parameter().as<Windows::Foundation::IPropertyValue>();
        if (propertyValue.Type() == Windows::Foundation::PropertyType::String) {
            auto data = winrt::unbox_value<winrt::hstring>(e.Parameter());
            if (!data.empty()) {
                this->m_dataToExplain = data;
                __super::OnNavigatedTo(e);
            }
        }
        __super::OnNavigatedTo(e);
    
    };
    
}
