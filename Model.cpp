#include "pch.h"
#include "Model.h"
#include "Model.g.cpp"

namespace winrt::SIPADPBT::implementation
{


    Model::Model(hstring const& question, bool isRight, int32_t index) : m_question(question), m_isRight(isRight), m_index(index) {};

    hstring Model::question()
    {
        return this->m_question;
    }
    void Model::question(hstring const& value)
    {
        if (this->m_question != value) {
            this->m_question = value;
            this->m_PropertyChanged(*this,Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"question"));
        }
    }
    int32_t Model::index()
    {
        return this->m_index;
    }
    void Model::index(int32_t value)
    {
        if (this->m_index != value) {
            this->m_index = value;
            this->m_PropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"index"));
        }
    }
    bool Model::isRight()
    {
        return this->m_isRight;
    }
    void Model::isRight(bool value)
    {
        if (this->m_isRight != value) {
            this->m_isRight = value;
            this->m_PropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"isRight"));
        }
    }
    winrt::event_token Model::PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        return this->m_PropertyChanged.add(handler);
    }
    void Model::PropertyChanged(winrt::event_token const& token) noexcept
    {
        this->m_PropertyChanged.remove(token);
    }
}
