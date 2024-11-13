#include "pch.h"
#include "JengkolAnalysis.xaml.h"
#if __has_include("JengkolAnalysis.g.cpp")
#include "JengkolAnalysis.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::SIPADPBT::implementation
{
    JengkolAnalysis::JengkolAnalysis() {
        this->m_questionnaire = winrt::single_threaded_observable_vector<winrt::SIPADPBT::Model>();
        this->m_disease = winrt::single_threaded_observable_vector<winrt::SIPADPBT::ModelDisease>();
        this->InitializeComponent();
        this->Loaded([&](IInspectable const&, RoutedEventArgs const&)-> Windows::Foundation::IAsyncAction {
            winrt::apartment_context ui_thread;
            winrt::hstring indexContent;
            co_await this->ReadFileFromAssets(L"index data.json", indexContent);
            std::vector<winrt::SIPADPBT::Model> inputData;
            std::vector<winrt::SIPADPBT::ModelDisease> diseases;
            int32_t index = 0;

            co_await winrt::resume_background();
            nlohmann::json indexData = nlohmann::json::parse(winrt::to_string(indexContent));
            for (auto& data : indexData["jengkol"]["gejala"]) {
                inputData.push_back({
                    winrt::to_hstring(data.template get<std::string>()),
                    false,
                    index
                });
                index++;
            }
            index = 0;
            for (auto& data : indexData["jengkol"]["penyakit"]) {
                diseases.push_back({
                    winrt::to_hstring(data["nama"].template get<std::string>()),
                    winrt::to_hstring(data["deskripsi"].template get<std::string>()),
                    index,
                    0.0
                });
                index++;
            }

            co_await ui_thread;

            if (this->m_disease.Size() == 0) {
                this->m_disease.ReplaceAll(diseases);
            }
            if (this->m_questionnaire.Size() == 0) {
                this->m_questionnaire.ReplaceAll(inputData);
            }

        });
    }

    void JengkolAnalysis::ChangeSelection(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const&) {
        auto& element = sender.as<Microsoft::UI::Xaml::Controls::RadioButtons>();
        auto& dataContext = element.DataContext().as<winrt::SIPADPBT::Model>();
        int32_t selectedIndex = element.SelectedIndex();
        if (selectedIndex != -1) {
            auto& selectedChildElement = element.ContainerFromIndex(selectedIndex).try_as<Microsoft::UI::Xaml::Controls::RadioButton>();
            uint32_t indexData;
            this->m_questionnaire.IndexOf(dataContext, indexData);

            if (selectedChildElement) {
                if (selectedChildElement.Content().as<winrt::hstring>() == L"Ya") {
                    this->m_questionnaire.GetAt(indexData).isRight(true);
                }
                else {
                    this->m_questionnaire.GetAt(indexData).isRight(false);
                }
            }
        }

    };

    void JengkolAnalysis::Show_Info(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const&) {
        
        auto& button = sender.as<Microsoft::UI::Xaml::Controls::Button>();
        auto& parent = button.Parent().as<Microsoft::UI::Xaml::Controls::Grid>();
        auto& info = parent.Children().GetAt(1).as<Microsoft::UI::Xaml::Controls::TeachingTip>();
        info.Target(button);
        info.IsOpen(true);
    
    };

    void JengkolAnalysis::Explain(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const&) {

        Windows::UI::Xaml::Interop::TypeName typenameTarget;
        typenameTarget.Name = L"SIPADPBT.Explaination";
        typenameTarget.Kind = winrt::Windows::UI::Xaml::Interop::TypeKind::Primitive;
        this->Frame().Navigate(
            typenameTarget,
            winrt::box_value(winrt::to_hstring(this->m_dataToExplain.dump(4)))
        );

    };

    winrt::Windows::Foundation::Collections::IObservableVector<winrt::SIPADPBT::Model> JengkolAnalysis::questionnaire()
    {
        return this->m_questionnaire;
    }

    winrt::Windows::Foundation::Collections::IObservableVector<winrt::SIPADPBT::ModelDisease> JengkolAnalysis::disease() {
        return this->m_disease;
    }

    Windows::Foundation::IAsyncAction JengkolAnalysis::ReadFileFromAssets(winrt::hstring path, winrt::hstring& result) {
        winrt::hstring output;
        try {
            Windows::Storage::StorageFolder applicationInstallationPath = Windows::ApplicationModel::Package::Current().InstalledLocation();
            Windows::Storage::StorageFolder assetsPath = co_await applicationInstallationPath.GetFolderAsync(L"Assets");
            Windows::Storage::StorageFolder corePath = co_await assetsPath.GetFolderAsync(L"Core");
            Windows::Storage::StorageFile file = co_await corePath.GetFileAsync(path);

            Windows::Storage::Streams::IRandomAccessStream stream = co_await file.OpenAsync(Windows::Storage::FileAccessMode::Read);
            Windows::Storage::Streams::DataReader reader{ stream.GetInputStreamAt(0) };

            uint32_t size = static_cast<uint32_t>(stream.Size());
            co_await reader.LoadAsync(size);
            output = reader.ReadString(size);
        }
        catch (std::exception& err) {
            OutputDebugString(
                (L"Error, " + winrt::to_hstring(err.what())).c_str()
            );
        }
        result = output;
    };

    void JengkolAnalysis::reset(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const&) {
        
        for (auto& predict_label : this->m_disease) {
            predict_label.precentage(0);
        }

        for (auto& question : this->m_questionnaire) {
            question.isRight(false);
        }
    
    };

    Windows::Foundation::IAsyncAction JengkolAnalysis::Analyze(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const&) {

        std::vector<winrt::SIPADPBT::ModelDisease> _output_disease;
        Windows::Storage::StorageFolder applicationInstallationPath = Windows::ApplicationModel::Package::Current().InstalledLocation();
        Windows::Storage::StorageFolder assetsPath = co_await applicationInstallationPath.GetFolderAsync(L"Assets");
        Windows::Storage::StorageFolder corePath = co_await assetsPath.GetFolderAsync(L"Core");
        OutputDebugString((L"\n Model path : "+corePath.Path()+L"\n").c_str());
        std::string corePathStdStr = winrt::to_string(corePath.Path());

        winrt::apartment_context ui_thread;

        co_await winrt::resume_background();
        std::thread get_analyze([&](std::string&& core_path)-> void{
            arma::mat input;
            arma::mat output;
            std::vector<std::string> questions;
            input.reshape(1, this->m_questionnaire.Size());
            uint32_t index = 0;
            for (auto& inputData : this->m_questionnaire) {
                input(0, index) = inputData.isRight();
                questions.push_back(winrt::to_string(inputData.question()));
                index++;
            }
            mlpack::FFN<mlpack::CrossEntropyError> model;
            mlpack::data::Load(
                (core_path +"\\model-jengkol.bin").c_str(),
                "model",
                model,
                false
            );
            input = input.t();
            model.Predict(input, output);
            for (uint32_t i = 0; i < this->m_disease.Size(); i++) {
                auto& _disease = this->m_disease.GetAt(i);
                _output_disease.push_back({
                    _disease.name(),
                    _disease.description(),
                    _disease.index(),
                    round(output(i,0) * 100)
                });
            }

            // Creating a tabular data with pertrubation
            // using random normal distribution

            uint32_t input_col_size = this->m_questionnaire.Size(), output_row_size = this->m_disease.Size();
            arma::mat training_data;
            const std::string data_path ="\\data jengkol.csv";
            mlpack::data::Load((core_path + data_path).c_str(), training_data);

            arma::mat input_training_data = training_data.rows(0, input_col_size + 1).t();
            arma::vec input_columns;        // vector of features
            std::vector<double> means;      // list means of features
            std::vector<double> stddevs;    // list standard deviations of features
            double mean_input_columns = 0;  // get current mean from feature 
            double stddev = 0;              // get current standard deviation from feature
            double Z = 0;                   // get sum square of (X_i - mean_input_columns) where the X_i is feature in index "i"

            for (uint32_t i = 0; i < input_col_size; i++) {
                input_columns = input_training_data.col(i);
                mean_input_columns = arma::sum(input_columns) / input_columns.n_rows;

                Z = 0;
                for (auto& x : input_columns) {
                    Z += pow(x - mean_input_columns, 2);
                }
                stddev = sqrt(Z / input_columns.n_rows);
                means.push_back(mean_input_columns);
                stddevs.push_back(stddev);

            }

            arma::mat pertrubate_data;
            uint32_t total_pertrubate_data = 2000;

            pertrubate_data.reshape(total_pertrubate_data, input_col_size);
            arma::mat transpose_input = input.t();
            for (uint32_t i = 0; i < means.size(); i++) {
                std::default_random_engine r_engine;
                std::normal_distribution<double> n_distribution(means[i], stddevs[i]);

                for (uint32_t j = 0; j < total_pertrubate_data; j++) {
                    pertrubate_data(j, i) = transpose_input(0, i) + n_distribution(r_engine);
                }
            }

            arma::mat pertrubate_result;
            model.Predict(pertrubate_data.t(), pertrubate_result);
            pertrubate_result = pertrubate_result.t(); // size (total_pertrubate_data X output_row_size)

            // get the index class using argmax
            arma::mat selection_pertrubate_result = pertrubate_result;
            double pi_x = 0, distance = 0, sigma = 1, index_has_delete = 0;
            for (uint32_t i = 0; i < total_pertrubate_data; i++) {
                distance = arma::norm(output.t() - pertrubate_result.rows(i, i), 2);
                pi_x = exp(-pow(distance, 2) / sigma);
                // check if pi_x distance are greater or equals to 0.5
                if (!(pi_x >= 0.9)) {
                    pertrubate_data.shed_row(i - index_has_delete);
                    selection_pertrubate_result.shed_row(i - index_has_delete);
                    index_has_delete++;
                }
            }
            pertrubate_result.reset();

            // get single max output for each class from
            arma::mat vector_selection_pertrubate_result;
            arma::mat new_pertrubate_data, new_pertrubate_result;
            vector_selection_pertrubate_result.reshape(1, selection_pertrubate_result.n_rows);
            for (uint32_t i = 0; i < selection_pertrubate_result.n_rows; i++) {
                vector_selection_pertrubate_result(0, i) = selection_pertrubate_result.row(i).max();
            }
            selection_pertrubate_result.reset();

            // create g model, in this capstone project selection_pertrubate_result
            // i will use Logistic Regression

            pertrubate_data = pertrubate_data.t();
            mlpack::LinearRegression LR;
            LR.Train(pertrubate_data, vector_selection_pertrubate_result);

            arma::mat validation;
            LR.Predict(pertrubate_data, validation);

            double INTERPRETABLE_VALUE = arma::norm(vector_selection_pertrubate_result - validation, 2);
            std::vector<double> data_to_explain;
            for (auto& d : LR.Parameters()) {
                data_to_explain.push_back(d);
            }


            this->m_dataToExplain["data"] = data_to_explain;
            this->m_dataToExplain["labels"] = questions;
            this->m_dataToExplain["interpretable_value"] = INTERPRETABLE_VALUE;


        },std::move(corePathStdStr));

        get_analyze.join();

        co_await ui_thread;
        this->m_disease.ReplaceAll(_output_disease);

    }
}
