#pragma once

#include "midi_wrap.h"

/* Simple API to address my particular dev midi setup. */
/* Allows binding values to a particular midi controller and ImGui at the same time. */

bool twisterSliderFloat(const char* id, int knob, float* v, float v_min = 0.0f, float v_max = 1.0f, const char* format = "%.2f", float power = 1.0f);
bool twisterSliderFloatWithDefault(const char* id, int knob, float* v, float v_default, float v_min = 0.0f, float v_max = 1.0f, const char* format = "%.2f", float power = 1.0f);
bool twisterKnobButton(const char* id, int knob);
int  twisterKnobValue(int knob);
int  twisterKnobDelta(int knob);
bool twisterKnobPress(int knob);
bool twisterKnobRelease(int knob);
bool twisterKnobDown(int knob);

bool fighterButton(const char* id, int button);
bool fighterPress(int button);
bool fighterRelease(int button);
bool fighterDown(int button);

void devmidiInit();
void devmidiUpdate();
void devmidiTerm();

