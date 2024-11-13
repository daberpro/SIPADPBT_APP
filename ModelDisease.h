#pragma once
#include "ModelDisease.g.h"

namespace winrt::SIPADPBT::implementation
{
    struct ModelDisease : ModelDiseaseT<ModelDisease>
    {
        ModelDisease() = default;

        ModelDisease(hstring const& name, hstring const& description, int32_t index, double precentage);
        hstring name();
        void name(hstring const& value);
        hstring description();
        void description(hstring const& value);
        double precentage();
        void precentage(double value);
        int32_t index();
        void index(int32_t value);
        winrt::event_token PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

    private:
        winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_PropertyChanged;
        winrt::hstring m_name;
        winrt::hstring m_description;
        int32_t m_index;
        double m_precentage;
    };
}
namespace winrt::SIPADPBT::factory_implementation
{
    struct ModelDisease : ModelDiseaseT<ModelDisease, implementation::ModelDisease>
    {
    };
}
