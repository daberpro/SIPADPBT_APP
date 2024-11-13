#pragma once
#include "CheckBoolean.g.h"

namespace winrt::SIPADPBT::implementation
{
    struct CheckBoolean : CheckBooleanT<CheckBoolean>
    {
        CheckBoolean() = default;

        winrt::Windows::Foundation::IInspectable Convert(winrt::Windows::Foundation::IInspectable const& value, winrt::Windows::UI::Xaml::Interop::TypeName const& targetType, winrt::Windows::Foundation::IInspectable const& parameter, hstring const& language);
        winrt::Windows::Foundation::IInspectable ConvertBack(winrt::Windows::Foundation::IInspectable const& value, winrt::Windows::UI::Xaml::Interop::TypeName const& targetType, winrt::Windows::Foundation::IInspectable const& parameter, hstring const& language);
    };
}
namespace winrt::SIPADPBT::factory_implementation
{
    struct CheckBoolean : CheckBooleanT<CheckBoolean, implementation::CheckBoolean>
    {
    };
}
