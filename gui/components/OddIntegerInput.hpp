#include <wx/wx.h>
#include <string_view>

using namespace std;

struct OddIntegerInput : IntegerInput {

	using IntegerInput::IntegerInput;

	void setupUi(
					wxBoxSizer* vbox,
					wxPanel* panel,
					std::optional<string_view> label_opt = std::nullopt,
					std::optional<int> default_value_opt = std::nullopt,
					std::optional<int> min_value_opt = std::nullopt,
					std::optional<int> max_value_opt = std::nullopt)
	{
		IntegerInput::setupUi(vbox, panel, label_opt, default_value_opt, min_value_opt, max_value_opt);
		slider->Bind(wxEVT_SLIDER, &OddIntegerInput::onSliderChange, this);
	}

	void onSliderChange(wxCommandEvent& event) {
		int value = slider->GetValue();
		if (value % 2 == 0) {
			value += (value < max_value) ? 1 : -1;
			slider->SetValue(value);
		}
	}
};
