#include "devmidi.h"
#include "core/log.h"
#include "imgui/imgui.h"

static MIDIDeviceConfig midi_device_config[] =
{
	MIDIDeviceConfig("Midi Fighter Twister", MIDI_CONTINUOUS_CONTROLLER_RELATIVE),
	MIDIDeviceConfig("Midi Fighter 3D"),
};

// I have my device with it's cord up and away from me, and I want knob 0 to be the lower left hand corner.
static int twisterKnobRemap(int knob) {
	// I don't know how to deal with more than 16 knobs yet.
	if (knob > 15) return 0;
	// First we need to flip the button upside down. So remap it to a 4x4, flip the y, then go back to idx.
	int x = knob % 4;
	int y = knob / 4;
	y = 3 - y;
	return x + y * 4;
}
// I have my device with it's cord up away from me, and I want button 0 to be the lower left hand corner.
static int fighterButtonRemap(int button) {
	// I don't know how to deal with more than 16 buttons yet.
	if (button > 15) return 0;
	return 51 - button; // first set of fighter buttons go from 36 to 51
}


static int write_idx = 0;
static MIDIState twister[2];
static MIDIState fighter[2];

static MIDIState* twister_prev;
static MIDIState* twister_curr;
static MIDIState* fighter_prev;
static MIDIState* fighter_curr;

/* Generic internal functions. */
static bool Press(MIDIState* curr, MIDIState* prev, int button) {
	return (curr->note_ons[button] - prev->note_ons[button]) > 0;
}
static bool Release(MIDIState* curr, MIDIState* prev, int button) {
	return (curr->note_offs[button] - prev->note_offs[button]) > 0;
}
static bool Down(MIDIState* curr, MIDIState* prev, int button) {
	return curr->note_ons[button] > prev->note_offs[button];
}
static bool ButtonPress(const char* id, MIDIState* curr, MIDIState* prev, int button) {
	return gui::Button(id) || Press(curr, prev, button);
}
static bool ButtonRelease(const char* id, MIDIState* curr, MIDIState* prev, int button) {
	return gui::Button(id) || Release(curr, prev, button);
}
static bool SliderFloat(const char* id, MIDIState* curr, MIDIState* prev, int knob, float* v, float v_min, float v_max, const char* format, float power) {
	float knob_scale = 1.0f / 100.0f;
	if (format) {
		const char* dig = format;
		while (*dig && (*dig++ != '.'));
		if (*dig) {
			knob_scale = 1.0f / powf(10.0f, float(atoi(dig)));
		}
	}
	int del = curr->value[knob] - prev->value[knob];
	float linear_v = power == 1.0f ? *v : powf(*v, 1.0f / power);
	linear_v = min(v_max, max(v_min, linear_v + del * knob_scale));
	*v = power == 1.0f ? linear_v : powf(linear_v, power);

	return gui::SliderFloat(id, v, v_min, v_max, format, power) || del != 0;
}
static bool SliderFloatWithDefault(const char* id, MIDIState* curr, MIDIState* prev, int knob, float* v, float v_default, float v_min, float v_max, const char* format, float power) {
	gui::PushID(id);
	bool changed = SliderFloat(id, curr, prev, knob, v, v_min, v_max, format, power);
	gui::SameLine();
	if (ButtonRelease("default", curr, prev, knob)) {
		*v = v_default;
		changed |= true;
	}
	gui::PopID();
	return changed;
}

/* Specific functions tuned to the midi device. */
bool twisterSliderFloat(const char* id, int knob, float* v, float v_min, float v_max, const char* format, float power) {
	return SliderFloat(id, twister_curr, twister_prev, twisterKnobRemap(knob), v, v_min, v_max, format, power);
}
bool twisterSliderFloatWithDefault(const char* id, int knob, float* v, float v_default, float v_min, float v_max, const char* format, float power) {
	return SliderFloatWithDefault(id, twister_curr, twister_prev, twisterKnobRemap(knob), v, v_default, v_min, v_max, format, power);
}
bool twisterKnobButton(const char* id, int knob) {
	return ButtonRelease(id, twister_curr, twister_prev, twisterKnobRemap(knob));
}
int  twisterKnobValue(int knob) {
	return twister_curr->value[twisterKnobRemap(knob)];
}
int  twisterKnobDelta(int knob) {
	return twister_curr->value[twisterKnobRemap(knob)] - twister_prev->value[twisterKnobRemap(knob)];
}
bool twisterKnobPress(int knob) {
	return Press(twister_curr, twister_prev, twisterKnobRemap(knob));
}
bool twisterKnobRelease(int knob) {
	return Release(twister_curr, twister_prev, twisterKnobRemap(knob));
}
bool twisterKnobDown(int knob) {
	return Down(twister_curr, twister_prev, twisterKnobRemap(knob));
}
bool fighterButton(const char* id, int button) {
	return ButtonRelease(id, fighter_curr, fighter_prev, fighterButtonRemap(button));
}
bool fighterPress(int button) {
	return Press(fighter_curr, fighter_prev, fighterButtonRemap(button));
}
bool fighterRelease(int button) {
	return Release(fighter_curr, fighter_prev, fighterButtonRemap(button));
}
bool fighterDown(int button) {
	return Down(fighter_curr, fighter_prev, fighterButtonRemap(button));
}

/* Core init/update/teardown. */
void devmidiInit() {
	midiInit(midi_device_config, sizeof(midi_device_config) / sizeof(midi_device_config[0]));
}
void devmidiTerm() {
	midiTerm();
}
void devmidiUpdate() {
	if (midiGetState(&twister[write_idx], 0)) {
	}
	if (midiGetState(&fighter[write_idx], 1)) {
	}
	twister_curr = &twister[    write_idx];
	twister_prev = &twister[1 - write_idx];
	fighter_curr = &fighter[    write_idx];
	fighter_prev = &fighter[1 - write_idx];
	write_idx = 1-write_idx;
}