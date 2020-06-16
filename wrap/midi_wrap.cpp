#include "midi_wrap.h"
#include "core/log.h"

// order dependent :(
#include <windows.h> // first, for UINT
#include <mmeapi.h>  //second, for midi
#pragma comment(lib,"winmm.lib") // for midi

#include <vector>
#include <mutex>

// Uncomment to sniff the midi messages and print them out directly. Useful for working mappings out for your controller setup.
//#define PRINT_MESSAGES

namespace {

struct Device {
	int config_id;
	const char* name;
	int continuous_mode;
	HMIDIIN handle;
	MIDIState state;
	Device(int _id, const char* _name, int _continuous_mode) : config_id(_id), name(_name), continuous_mode(_continuous_mode), handle(0) {
		memset(&state, 0, sizeof(state));
	}
};

}

/* State */
static std::mutex cb_mutex;
std::vector<Device> devices;

void CALLBACK midicb(HMIDIIN   hMidiIn, UINT wMsg, DWORD_PTR dev_idx, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
	(void)hMidiIn;
	(void)dwParam2;
	if (wMsg == MIM_DATA) {
		Device* device = (Device*)&devices[dev_idx];
		int type = (dwParam1 >> 4) & 0xf;
		if (type == 0xb) { // Continuous Controller		
			std::unique_lock<std::mutex> lock(cb_mutex);
			int idx = (dwParam1 >> 8) & 0xff;
			int val = (dwParam1 >> 16) & 0xff;
			if (device->continuous_mode == MIDI_CONTINUOUS_CONTROLLER_ABSOLUTE)
				device->state.value[idx] = val;
			else
				device->state.value[idx] += val - 64; // relative control is signed and biased around 64
#ifdef PRINT_MESSAGES
			log("dev%d : [0xb = Cont Ctrl] [idx %d] [val %d]\n", dev_idx, idx, val);
#endif
		} else if (type == 0x8) { // Note Off
			std::unique_lock<std::mutex> lock(cb_mutex);
			int idx = (dwParam1 >> 8) & 0xff;	
			device->state.note_offs[idx] += 1;
#ifdef PRINT_MESSAGES
			int vel = (dwParam1 >> 16) & 0xff;
			log("dev%d : [0x8 = Note Off ] [idx %d] [vel %d]\n", dev_idx, idx, vel);
#endif
		} else if (type == 0x9) { // Note On
			std::unique_lock<std::mutex> lock(cb_mutex);
			int idx = (dwParam1 >> 8) & 0xff;	
			device->state.note_ons[idx] += 1;
#ifdef PRINT_MESSAGES
			int vel = (dwParam1 >> 16) & 0xff;
			log("dev%d : [0x9 = Note On  ] [idx %d] [vel %d]\n", dev_idx, idx, vel);
#endif
		} else {
			log("[MIDI] Unknown input: %08x (type=%08x idx=%d val=%d)\n", dwParam1, (dwParam1 >> 4) & 0xf, (dwParam1 >> 8) & 0xff, (dwParam1 >> 16) & 0xff);
		}
	}
}

void midiInit(MIDIDeviceConfig* configs, int configs_count) {
	UINT numdevs = midiInGetNumDevs();
	for (UINT d = 0; d < numdevs; ++d) {
		MIDIINCAPS caps;
		if (midiInGetDevCaps(d, &caps, sizeof(caps)) == MMSYSERR_NOERROR) {
			log("[MIDI] %d - Device '%s'\n", d, caps.szPname);
		}
	}
	for (UINT d = 0; d < numdevs; ++d) {
		MIDIINCAPS caps;
		if (midiInGetDevCaps(d, &caps, sizeof(caps)) == MMSYSERR_NOERROR) {
			for (int c = 0; c < configs_count; ++c) {
				if (strcmp(caps.szPname, configs[c].name) == 0) {
					log("[MIDI] Selecting Device %d: '%s' %x\n", d, caps.szPname, caps.dwSupport);
					devices.push_back(Device(c, configs[c].name, configs[c].continuous_mode));
					if (midiInOpen(&devices.back().handle, d, (DWORD_PTR)midicb, devices.size()-1, CALLBACK_FUNCTION) != MMSYSERR_NOERROR) {
						devices.pop_back();
						log("[MIDI] midiOpen failed!\n");
						continue;
					}
					if (midiInStart(devices.back().handle) != MMSYSERR_NOERROR) {
						midiInClose(devices.back().handle);
						devices.pop_back();
						log("[MIDI] midiStart failed!\n");
						continue;
					}
				}
			}
		}
	}
}
void midiTerm() {
	for (Device& dev : devices) {
		assert(dev.handle);
		midiInClose(dev.handle);
	}
}

bool midiGetState(MIDIState* state_out, int device_id, int num) {
	for (Device& dev : devices) {
		if (dev.config_id == device_id) {
			if (num == 0) {
				std::unique_lock<std::mutex> lock(cb_mutex);
				memcpy(state_out, &dev.state, sizeof(MIDIState));
				return true;
			} else {
				num -= 1; // Keep searching for our device.
			}
		}
	}
	return false;
}