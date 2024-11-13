#include "pch.h"
#include "ModelDisease.h"
#include "ModelDisease.g.cpp"

namespace winrt::SIPADPBT::implementation
{
    ModelDisease::ModelDisease(hstring const& name, hstring const& description, int32_t index, double precentage) :
        m_name(name), m_description(description), m_index(index), m_precentage(precentage)
    {
    }
    hstring ModelDisease::name()
    {
        return this->m_name;
    }
    void ModelDisease::name(hstring const& value)
    {
        if (this->m_name != value) {
            this->m_name = value;
            this->m_PropertyChanged(*this,Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"name"));
        }
    }
    hstring ModelDisease::description()
    {
        return this->m_description;
    }
    void ModelDisease::description(hstring const& value)
    {
        if (this->m_description != value) {
            this->m_description = value;
            this->m_PropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"description"));
        }
    }
    double ModelDisease::precentage()
    {
        return this->m_precentage;
    }
    void ModelDisease::precentage(double value)
    {
        if (this->m_precentage != value) {
            this->m_precentage = value;
            this->m_PropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"precentage"));
        }
    }
    int32_t ModelDisease::index()
    {
        return this->m_index;
    }
    void ModelDisease::index(int32_t value)
    {
        if (this->m_index != value) {
            this->m_index = value;
            this->m_PropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"index"));
        }
    }
    winrt::event_token ModelDisease::PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        return this->m_PropertyChanged.add(handler);
    }
    void ModelDisease::PropertyChanged(winrt::event_token const& token) noexcept
    {
        this->m_PropertyChanged.remove(token);
    }
}
