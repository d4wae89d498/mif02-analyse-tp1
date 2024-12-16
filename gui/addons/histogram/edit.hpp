#pragma once
#include <opencv2/core.hpp>

struct HistogramLUT : Mif02Plugin
{
    wxTextCtrl* lowerBoundCtrl = nullptr;
    wxTextCtrl* upperBoundCtrl = nullptr;
    wxButton* histButton = nullptr;

    fn getName() const -> string_view override
    {
        return "Histogram / LUT";
    }

    fn setupUi(wxBoxSizer* vbox, wxPanel* panel) -> void override
    {
        wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
        wxStaticText* paramLabel = new wxStaticText(panel, wxID_ANY, "Plage inférieure :");
        lowerBoundCtrl = new wxTextCtrl(panel, wxID_ANY, "0");
        hbox->Add(paramLabel, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
        hbox->Add(lowerBoundCtrl, 1, wxEXPAND);
        vbox->Add(hbox, 0, wxEXPAND | wxALL, 10);

        hbox = new wxBoxSizer(wxHORIZONTAL);
        paramLabel = new wxStaticText(panel, wxID_ANY, "Plage supérieure :");
        upperBoundCtrl = new wxTextCtrl(panel, wxID_ANY, "255");
        hbox->Add(paramLabel, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
        hbox->Add(upperBoundCtrl, 1, wxEXPAND);
        vbox->Add(hbox, 0, wxEXPAND | wxALL, 10);

/*
        histButton = new wxButton(panel, wxID_ANY, "Afficher Histogramme");
        vbox->Add(histButton, 0, wxALL, 10);

        histButton->Bind(wxEVT_BUTTON, &HistogramLUT::onShowHistogram, this);*/
    }


    fn onApply(const cv::Mat& loadedImage, cv::Mat& filteredImg) -> void override
    {
        int lowerBound = std::stoi(lowerBoundCtrl->GetValue().ToStdString());
        int upperBound = std::stoi(upperBoundCtrl->GetValue().ToStdString());

        if (lowerBound >= 0 && upperBound <= 255 && lowerBound < upperBound)
        {

            displayHistogram(filteredImg);
        }
        else
        {
            wxMessageBox("Les valeurs de la plage sont invalides.", "Erreur", wxOK | wxICON_ERROR);
        }
    }
};

REGISTER_PLUGIN(HistogramLUT);
