#pragma once
#include "Model.g.h"

namespace winrt::SIPADPBT::implementation
{

    struct Model : ModelT<Model>
    {
        
        Model() = default;
        Model(hstring const& question, bool isRight, int32_t index);

        winrt::SIPADPBT::Option selectedOption();
        void selectedOption(winrt::SIPADPBT::Option const& value);
        hstring question();
        void question(hstring const& value);
        bool isRight();
        void isRight(bool value);
        int32_t index();
        void index(int32_t value);
        winrt::event_token PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

    private:
        winrt::hstring m_question;
        bool m_isRight;
        int32_t m_index;
        winrt::SIPADPBT::Option m_selectedOption = winrt::SIPADPBT::Option::No;
        winrt::event<winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_PropertyChanged;

    };
}
namespace winrt::SIPADPBT::factory_implementation
{
    struct Model : ModelT<Model, implementation::Model>
    {
    };
}
