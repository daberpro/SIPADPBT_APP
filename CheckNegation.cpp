#include "pch.h"
#include "CheckNegation.h"
#include "CheckNegation.g.cpp"

namespace winrt::SIPADPBT::implementation
{
    winrt::Windows::Foundation::IInspectable CheckNegation::Convert(
        winrt::Windows::Foundation::IInspectable const& value,
        winrt::Windows::UI::Xaml::Interop::TypeName const& targetType,
        winrt::Windows::Foundation::IInspectable const& parameter, hstring const& language
    ) {
        winrt::SIPADPBT::Option new_value = winrt::unbox_value<winrt::SIPADPBT::Option>(value);
        return winrt::box_value(new_value == winrt::SIPADPBT::Option::No);
    }
    winrt::Windows::Foundation::IInspectable CheckNegation::ConvertBack(winrt::Windows::Foundation::IInspectable const& value, winrt::Windows::UI::Xaml::Interop::TypeName const& targetType, winrt::Windows::Foundation::IInspectable const& parameter, hstring const& language)
    {
        bool new_value = winrt::unbox_value<bool>(value);
        if (new_value) {
            return winrt::box_value(winrt::SIPADPBT::Option::No);
        }
        return winrt::box_value(winrt::SIPADPBT::Option::Yes);
    }
}
