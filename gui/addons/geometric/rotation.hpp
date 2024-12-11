#pragma once

struct Rotate : Mif02Plugin
{
    wxTextCtrl* degreeCtrl = nullptr;
    wxCheckBox* opencvCheckbox = nullptr;

    fn getName() const -> string_view override
    {
        return "Rotate";
    }

    fn setupUi(wxBoxSizer* vbox, wxPanel* panel) -> void override
    {
        wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
        wxStaticText* paramLabel = new wxStaticText(panel, wxID_ANY, "Angle de rotation (°) :");
        degreeCtrl = new wxTextCtrl(panel, wxID_ANY, "0");
        hbox->Add(paramLabel, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
        hbox->Add(degreeCtrl, 1, wxEXPAND);
        vbox->Add(hbox, 0, wxEXPAND | wxALL, 10);


		opencvCheckbox = new wxCheckBox(panel, wxID_ANY, "Utiliser fonction de OpenCV");
		opencvCheckbox->SetValue(true);
		vbox->Add(opencvCheckbox, 0, wxALL, 10);
    }

    fn onApply(const cv::Mat& loadedImage, cv::Mat &filteredImg) -> void override
    {
        double angle = stod(degreeCtrl->GetValue().ToStdString());

        cv::Point2f center(loadedImage.cols / 2.0, loadedImage.rows / 2.0);

		if (opencvCheckbox->IsChecked()) {


			cv::Mat rotationMatrix = cv::getRotationMatrix2D(center, angle, 1.0);
			cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), loadedImage.size(), angle).boundingRect2f();
			rotationMatrix.at<double>(0, 2) += bbox.width / 2.0 - center.x;
			rotationMatrix.at<double>(1, 2) += bbox.height / 2.0 - center.y;
			cv::warpAffine(loadedImage, filteredImg, rotationMatrix, bbox.size());

		} else {

			filteredImg = applyRotation(loadedImage, angle);

		}
    }
};

REGISTER_PLUGIN(Rotate);
