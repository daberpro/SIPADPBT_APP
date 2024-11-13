#pragma once

#include "DurianAnalysis.g.h"
#include "ModelDisease.h"
#include "Model.h"
#include "CheckBoolean.h"

namespace winrt::SIPADPBT::implementation
{
    struct DurianAnalysis : DurianAnalysisT<DurianAnalysis>
    {
        DurianAnalysis();
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::SIPADPBT::Model> questionnaire();
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::SIPADPBT::ModelDisease> disease();
        Windows::Foundation::Collections::IObservableVector<SIPADPBT::Model> m_questionnaire;
        Windows::Foundation::Collections::IObservableVector<SIPADPBT::ModelDisease> m_disease;
        Windows::Foundation::IAsyncAction ReadFileFromAssets(winrt::hstring path, winrt::hstring& result);
        Windows::Foundation::IAsyncAction Analyze(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&);
        void ChangeSelection(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&);
        void Show_Info(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&);
        void Explain(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&);
        void reset(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&);

        nlohmann::json m_dataToExplain = R"({
            "data": [],
            "interpretable_value": 1,
            "labels": []
        })"_json;
    };
}

namespace winrt::SIPADPBT::factory_implementation
{
    struct DurianAnalysis : DurianAnalysisT<DurianAnalysis, implementation::DurianAnalysis>
    {
    };
}
