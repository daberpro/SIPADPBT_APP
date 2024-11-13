#include "pch.h"
#include "CheckBoolean.h"
#include "CheckBoolean.g.cpp"

namespace winrt::SIPADPBT::implementation
{
    winrt::Windows::Foundation::IInspectable CheckBoolean::Convert(
        winrt::Windows::Foundation::IInspectable const& value, 
        winrt::Windows::UI::Xaml::Interop::TypeName const& targetType, 
        winrt::Windows::Foundation::IInspectable const& parameter, hstring const& language
    ){
        bool new_value = winrt::unbox_value<bool>(value);
        return winrt::box_value(!new_value);
    }
    winrt::Windows::Foundation::IInspectable CheckBoolean::ConvertBack(winrt::Windows::Foundation::IInspectable const& value, winrt::Windows::UI::Xaml::Interop::TypeName const& targetType, winrt::Windows::Foundation::IInspectable const& parameter, hstring const& language)
    {
        bool new_value = winrt::unbox_value<bool>(value);
        return winrt::box_value(!new_value);
    }
}
