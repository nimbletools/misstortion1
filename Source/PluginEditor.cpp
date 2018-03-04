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
	: AudioProcessorEditor(&p), processor(p)
{
	setSize(101 * 2, 157 * 2);

	m_backgroundImage = ImageCache::getFromMemory(BinaryData::Misstortion_png, BinaryData::Misstortion_pngSize);

	InitializeSlider(m_sliderMix, Slider::LinearBar, processor.m_paramMix, "%");
	InitializeSlider(m_sliderGain, Slider::LinearBar, processor.m_paramGain, " dB");

	InitializeSlider(m_sliderDriveHard, Slider::LinearBar, processor.m_paramDriveHard, " dB");
	InitializeSlider(m_sliderDriveSoft, Slider::LinearBar, processor.m_paramDriveSoft, " dB");
	InitializeSlider(m_sliderDriveSoftPre, Slider::LinearBar, processor.m_paramDriveSoftPre, "%");
	InitializeSlider(m_sliderToneHP, Slider::LinearBar, processor.m_paramToneHP, 0.25, " Hz");
	InitializeSlider(m_sliderToneLP, Slider::LinearBar, processor.m_paramToneLP, 0.25, " Hz");
	InitializeSlider(m_sliderSymmetry, Slider::LinearBar, processor.m_paramSymmetry, "%");

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
		slider.setPopupDisplayEnabled(true, true, false);
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
		slider.setPopupDisplayEnabled(true, true, false);
		slider.setTextValueSuffix(suffix);
	}
	slider.setValue(*param, dontSendNotification);
	slider.addListener(this);

	slider.setLookAndFeel(&m_lookAndFeel);

	addAndMakeVisible(slider);
}

//==============================================================================
void MisstortionAudioProcessorEditor::paint(Graphics& g)
{
	g.setImageResamplingQuality(Graphics::lowResamplingQuality);

	int width = m_backgroundImage.getWidth();
	int height = m_backgroundImage.getHeight();
	g.drawImage(m_backgroundImage, 0, 0, width * 2, height * 2, 0, 0, width, height);
}

void MisstortionAudioProcessorEditor::resized()
{
	setControlBounds(m_sliderMix, 2, 28, 97, 8);
	setControlBounds(m_sliderGain, 2, 45, 97, 8);

	setControlBounds(m_sliderDriveHard, 13, 62, 86, 8);
	setControlBounds(m_sliderDriveSoft, 13, 79, 86, 8);
	setControlBounds(m_sliderDriveSoftPre, 13, 96, 86, 8);
	setControlBounds(m_sliderToneHP, 13, 113, 86, 8);
	setControlBounds(m_sliderToneLP, 13, 130, 86, 8);
	setControlBounds(m_sliderSymmetry, 13, 147, 86, 8);

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
	if (sliderChanged != &actualSlider) {
		return;
	}
	*param = (float)sliderChanged->getValue();
}

void MisstortionAudioProcessorEditor::setSliderParam(Slider* sliderChanged, Slider &actualSlider, AudioParameterInt* param)
{
	if (sliderChanged != &actualSlider) {
		return;
	}
	*param = (int)sliderChanged->getValue();
}

void MisstortionAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	setSliderParam(slider, m_sliderMix, processor.m_paramMix);
	setSliderParam(slider, m_sliderGain, processor.m_paramGain);

	setSliderParam(slider, m_sliderDriveHard, processor.m_paramDriveHard);
	setSliderParam(slider, m_sliderDriveSoft, processor.m_paramDriveSoft);
	setSliderParam(slider, m_sliderDriveSoftPre, processor.m_paramDriveSoftPre);
	setSliderParam(slider, m_sliderToneHP, processor.m_paramToneHP);
	setSliderParam(slider, m_sliderToneLP, processor.m_paramToneLP);
	setSliderParam(slider, m_sliderSymmetry, processor.m_paramSymmetry);
}

void MisstortionAudioProcessorEditor::timerCallback()
{
	m_sliderMix.setValue(*processor.m_paramMix, dontSendNotification);
	m_sliderGain.setValue(*processor.m_paramGain, dontSendNotification);

	m_sliderDriveHard.setValue(*processor.m_paramDriveHard, dontSendNotification);
	m_sliderDriveSoft.setValue(*processor.m_paramDriveSoft, dontSendNotification);
	m_sliderDriveSoftPre.setValue(*processor.m_paramDriveSoftPre, dontSendNotification);
	m_sliderToneHP.setValue(*processor.m_paramToneHP, dontSendNotification);
	m_sliderToneLP.setValue(*processor.m_paramToneLP, dontSendNotification);
	m_sliderSymmetry.setValue(*processor.m_paramSymmetry, dontSendNotification);

#if DEBUG
	m_labelDebug.setText("[DEBUG] " + processor.m_debugText, dontSendNotification);
#endif
}
