/*
	==============================================================================

		This file was auto-generated!

		It contains the basic framework code for a JUCE plugin editor.

	==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
MisstortionAudioProcessorEditor::MisstortionAudioProcessorEditor(MisstortionAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p), m_tooltip(this, 100)
{
	m_backgroundImage = ImageCache::getFromMemory(BinaryData::Misstortion_png, BinaryData::Misstortion_pngSize);

	setSize(m_backgroundImage.getWidth() * 2, m_backgroundImage.getHeight() * 2);

	InitializeSlider(m_sliderMix, Slider::LinearBar, processor.m_paramMix, "%");
	InitializeSlider(m_sliderGainIn, Slider::LinearBar, processor.m_paramGainIn, " dB");
	InitializeSlider(m_sliderGainOut, Slider::LinearBar, processor.m_paramGainOut, " dB");

	InitializeSlider(m_sliderDriveHard, Slider::LinearBar, processor.m_paramDriveHard, " dB");
	InitializeSlider(m_sliderDriveSoft, Slider::LinearBar, processor.m_paramDriveSoft, " dB");
	InitializeSlider(m_sliderToneHP, Slider::LinearBar, processor.m_paramToneHP, 0.25, " Hz");
	InitializeSlider(m_sliderToneLP, Slider::LinearBar, processor.m_paramToneLP, 0.25, " Hz");
	InitializeSlider(m_sliderSymmetry, Slider::LinearBar, processor.m_paramSymmetry, "%");

	m_buttonFilterModeLegacy.setRadioGroupId(1, dontSendNotification);
	m_buttonFilterModeLegacy.onClick = [this]() { setFilterMode(0); };
	m_buttonFilterModeLegacy.setAlpha(0.0f);
	m_buttonFilterModeLegacy.setTooltip("Legacy filter");
	addAndMakeVisible(m_buttonFilterModeLegacy);

	m_buttonFilterMode6db.setRadioGroupId(1, dontSendNotification);
	m_buttonFilterMode6db.onClick = [this]() { setFilterMode(1); };
	m_buttonFilterMode6db.setAlpha(0.0f);
	m_buttonFilterMode6db.setTooltip("6dB/octave");
	addAndMakeVisible(m_buttonFilterMode6db);

	m_buttonFilterMode12db.setRadioGroupId(1, dontSendNotification);
	m_buttonFilterMode12db.onClick = [this]() { setFilterMode(2); };
	m_buttonFilterMode12db.setAlpha(0.0f);
	m_buttonFilterMode12db.setTooltip("12dB/octave");
	addAndMakeVisible(m_buttonFilterMode12db);

#if DEBUG
	m_labelDebug.setColour(Label::backgroundColourId, Colour(0x7FFF00FF));
	m_labelDebug.setColour(Label::textColourId, Colour(0xFFFFFFFF));
	addAndMakeVisible(m_labelDebug);
#endif

	startTimer(100);
}

MisstortionAudioProcessorEditor::~MisstortionAudioProcessorEditor()
{
}

void MisstortionAudioProcessorEditor::InitializeSlider(Slider &slider, Slider::SliderStyle style, AudioParameterFloat* param, String suffix)
{
	NormalisableRange<float> &range = param->range;

	slider.setSliderStyle(style);
	slider.setRange(range.start, range.end, 0.01);
	slider.setSkewFactor(range.skew, range.symmetricSkew);
	slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	if (suffix != "") {
		slider.setPopupDisplayEnabled(true, true, this);
		slider.setTextValueSuffix(suffix);
	}
	slider.setValue(*param, dontSendNotification);
	slider.addListener(this);

	slider.setLookAndFeel(&m_lookAndFeel);

	addAndMakeVisible(slider);
}

void MisstortionAudioProcessorEditor::InitializeSlider(Slider &slider, Slider::SliderStyle style, AudioParameterInt* param, double skew, String suffix)
{
	Range<int> range = param->getRange();

	slider.setSliderStyle(style);
	slider.setRange(range.getStart(), range.getEnd(), 1);
	slider.setSkewFactor(skew);
	slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	if (suffix != "") {
		slider.setPopupDisplayEnabled(true, true, this);
		slider.setTextValueSuffix(suffix);
	}
	slider.setValue(*param, dontSendNotification);
	slider.addListener(this);

	slider.setLookAndFeel(&m_lookAndFeel);

	addAndMakeVisible(slider);
}

//==============================================================================
void MisstortionAudioProcessorEditor::paintFilterButton(ToggleButton &button, Graphics &g)
{
	auto pos = button.getPosition();
	int width = button.getWidth();
	int height = button.getHeight();

	if (button.getToggleState()) {
		g.setColour(Colour(0xFF472E2E));
	} else {
		g.setColour(Colour(0xFF1E2526));
	}

	g.fillRect(pos.x, pos.y, width, height);
}

void MisstortionAudioProcessorEditor::paint(Graphics& g)
{
	g.setImageResamplingQuality(Graphics::lowResamplingQuality);

	int width = m_backgroundImage.getWidth();
	int height = m_backgroundImage.getHeight();

	paintFilterButton(m_buttonFilterModeLegacy, g);
	paintFilterButton(m_buttonFilterMode6db, g);
	paintFilterButton(m_buttonFilterMode12db, g);

	g.drawImage(m_backgroundImage, 0, 0, width * 2, height * 2, 0, 0, width, height);
}

void MisstortionAudioProcessorEditor::resized()
{
	setControlBounds(m_sliderMix, 2, 28, 111, 8);
	setControlBounds(m_sliderGainIn, 13, 45, 100, 8);
	setControlBounds(m_sliderGainOut, 13, 56, 100, 8);

	setControlBounds(m_sliderDriveHard, 13, 80, 100, 8);
	setControlBounds(m_sliderDriveSoft, 13, 97, 100, 8);
	setControlBounds(m_sliderToneHP, 13, 114, 86, 8);
	setControlBounds(m_sliderToneLP, 13, 131, 86, 8);
	setControlBounds(m_sliderSymmetry, 13, 148, 100, 8);

	setControlBounds(m_buttonFilterModeLegacy, 102, 110, 11, 9);
	setControlBounds(m_buttonFilterMode6db, 102, 120, 11, 9);
	setControlBounds(m_buttonFilterMode12db, 102, 130, 11, 9);

#if DEBUG
	m_labelDebug.setBounds(0, 0, 101 * 2, 50);
#endif
}

void MisstortionAudioProcessorEditor::setControlBounds(Component &component, int x, int y, int w, int h)
{
	component.setBounds(x * 2, y * 2, w * 2, h * 2);
}

void MisstortionAudioProcessorEditor::setSliderParam(Slider* sliderChanged, Slider &actualSlider, AudioParameterFloat* param)
{
	if (sliderChanged == &actualSlider) {
		*param = (float)sliderChanged->getValue();
	}
}

void MisstortionAudioProcessorEditor::setSliderParam(Slider* sliderChanged, Slider &actualSlider, AudioParameterInt* param)
{
	if (sliderChanged == &actualSlider) {
		*param = (int)sliderChanged->getValue();
	}
}

void MisstortionAudioProcessorEditor::setFilterMode(int newMode)
{
	*processor.m_paramFilterMode = newMode;
}

void MisstortionAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	setSliderParam(slider, m_sliderMix, processor.m_paramMix);
	setSliderParam(slider, m_sliderGainIn, processor.m_paramGainIn);
	setSliderParam(slider, m_sliderGainOut, processor.m_paramGainOut);

	setSliderParam(slider, m_sliderDriveHard, processor.m_paramDriveHard);
	setSliderParam(slider, m_sliderDriveSoft, processor.m_paramDriveSoft);
	setSliderParam(slider, m_sliderToneHP, processor.m_paramToneHP);
	setSliderParam(slider, m_sliderToneLP, processor.m_paramToneLP);
	setSliderParam(slider, m_sliderSymmetry, processor.m_paramSymmetry);
}

void MisstortionAudioProcessorEditor::timerCallback()
{
	m_sliderMix.setValue(*processor.m_paramMix, dontSendNotification);
	m_sliderGainIn.setValue(*processor.m_paramGainIn, dontSendNotification);
	m_sliderGainOut.setValue(*processor.m_paramGainOut, dontSendNotification);

	m_sliderDriveHard.setValue(*processor.m_paramDriveHard, dontSendNotification);
	m_sliderDriveSoft.setValue(*processor.m_paramDriveSoft, dontSendNotification);
	m_sliderToneHP.setValue(*processor.m_paramToneHP, dontSendNotification);
	m_sliderToneLP.setValue(*processor.m_paramToneLP, dontSendNotification);
	m_sliderSymmetry.setValue(*processor.m_paramSymmetry, dontSendNotification);

	int filterMode = (int)*processor.m_paramFilterMode;
	m_buttonFilterModeLegacy.setToggleState(filterMode == 0, dontSendNotification);
	m_buttonFilterMode6db.setToggleState(filterMode == 1, dontSendNotification);
	m_buttonFilterMode12db.setToggleState(filterMode == 2, dontSendNotification);

#if DEBUG
	m_labelDebug.setText("[DEBUG] " + processor.m_debugText, dontSendNotification);
#endif
}
