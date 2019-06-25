
struct GuiWidget {
	enum { INPUT, SLIDER, DRAG, BINARY, COLOR, WINDOW, HEADER };
	u8 type;
	union {
		float float_min;
		int int_min;
	};
	union {
		float float_max;
		int int_max;
	};
	const char* fmt;
	float power;
};

inline GuiWidget SliderFloat(float v_min = 0.f, float v_max = 1.f, const char* fmt = "%3.2f", float power = 1.f) {  GuiWidget w; w.type=GuiWidget::SLIDER; w.float_min = v_min; w.float_max = v_max; w.fmt = fmt; w.power = power; return w; }
inline GuiWidget InputFloat() { return {GuiWidget::INPUT}; }
inline GuiWidget SliderInt(int v_min = 0, int v_max = 100) { GuiWidget w; w.type=GuiWidget::SLIDER; w.int_min = v_min; w.int_max = v_max; return w; }
inline GuiWidget InputInt() { return {GuiWidget::INPUT}; }
inline GuiWidget Checkbox() { return GuiWidget(); }
inline GuiWidget Binary() { return {GuiWidget::BINARY}; }
inline GuiWidget Color() { return {GuiWidget::COLOR}; }
inline GuiWidget Header() { GuiWidget w; w.type = GuiWidget::HEADER; return w; }
inline GuiWidget Window() { GuiWidget w; w.type = GuiWidget::WINDOW; return w; }
inline GuiWidget Struct() { return Header(); }

inline void printBits(const char* name, u64 bits, int num_bits) {
	static char str[65];
	num_bits = min(num_bits,64);
	for (int b = 0; b < num_bits; ++b) {
		str[num_bits-1-b] = bits&0x1 ? '1' : '0';
		bits >>= 1;
	}
	str[num_bits] = 0;
	gui::Text("%s %s", str, name);
}

inline void Gui(const char* name, float& v, GuiWidget w) {
	if (w.type == GuiWidget::INPUT)
		ImGui::InputFloat(name, &v);
	if (w.type == GuiWidget::SLIDER)
		ImGui::SliderFloat(name, &v, w.float_min, w.float_max, w.fmt, w.power);
}
inline void Gui(const char* name, s32& v, GuiWidget w) {
	if (w.type == GuiWidget::INPUT)
		ImGui::InputInt(name, &v);
	if (w.type == GuiWidget::SLIDER)
		ImGui::SliderInt(name, &v, w.int_min, w.int_max);
}
inline void Gui(const char* name, u64& v, GuiWidget w) {
	int vi = v;
	if (w.type == GuiWidget::INPUT)
		ImGui::InputInt(name, &vi);
	if (w.type == GuiWidget::SLIDER)
		ImGui::SliderInt(name, &vi, w.int_min, w.int_max);
	if (w.type == GuiWidget::BINARY)
		printBits(name, v, 64);
	v = vi;
}
inline void Gui(const char* name, u32& v, GuiWidget w) {
	int vi = v;
	if (w.type == GuiWidget::INPUT)
		ImGui::InputInt(name, &vi);
	if (w.type == GuiWidget::SLIDER)
		ImGui::SliderInt(name, &vi, w.int_min, w.int_max);
	if (w.type == GuiWidget::BINARY)
		printBits(name, v, 32);
	v = vi;
}
inline void Gui(const char* name, u16& v, GuiWidget w) {
	int vi = v;
	if (w.type == GuiWidget::INPUT)
		ImGui::InputInt(name, &vi);
	if (w.type == GuiWidget::SLIDER)
		ImGui::SliderInt(name, &vi, w.int_min, w.int_max);
	if (w.type == GuiWidget::BINARY)
		printBits(name, v, 16);
	v = vi;
}
inline void Gui(const char* name, u8& v, GuiWidget w) {
	int  vi = v;
	if (w.type == GuiWidget::INPUT)
		ImGui::InputInt(name, &vi);
	if (w.type == GuiWidget::SLIDER)
		ImGui::SliderInt(name, &vi, w.int_min, w.int_max);
	if (w.type == GuiWidget::BINARY)
		printBits(name, v, 8);
	v = (u8)max(0,min(255,vi));
}
inline void Gui(const char* name, bool& v, GuiWidget w) {
	(void)w;
	ImGui::Checkbox(name, &v);
}
inline void Gui(const char* name, vec2& v, GuiWidget w) {
	if (w.type == GuiWidget::INPUT)
		ImGui::InputFloat2(name, (float*)&v);
	if (w.type == GuiWidget::SLIDER)
		ImGui::SliderFloat2(name, (float*)&v, w.float_min, w.float_max, w.fmt, w.power);
}
inline void Gui(const char* name, vec3& v, GuiWidget w) {
	if (w.type == GuiWidget::INPUT)
		ImGui::InputFloat3(name, (float*)&v);
	if (w.type == GuiWidget::SLIDER)
		ImGui::SliderFloat3(name, (float*)&v, w.float_min, w.float_max, w.fmt, w.power);
	if (w.type == GuiWidget::COLOR)
		ImGui::ColorEdit3(name, (float*)&v);
}
inline void Gui(const char* name, vec4& v, GuiWidget w) {
	if (w.type == GuiWidget::INPUT)
		ImGui::InputFloat4(name, (float*)&v);
	if (w.type == GuiWidget::SLIDER)
		ImGui::SliderFloat4(name, (float*)&v, w.float_min, w.float_max, w.fmt, w.power);
	if (w.type == GuiWidget::COLOR)
		ImGui::ColorEdit4(name, (float*)&v);
}
#define POD(T)

#define STRUCT(T) \
void Gui(const char* name, T& v, GuiWidget w = Window()) { \
	bool open = false; \
	if (w.type == GuiWidget::HEADER) open = ImGui::CollapsingHeader(name, ImGuiTreeNodeFlags_DefaultOpen); \
	if (w.type == GuiWidget::WINDOW) open = ImGui::Begin(name); \
	if (open) { \
		if (w.type == GuiWidget::HEADER) { ImGui::PushID(name); ImGui::Indent(); } 

#define END \
		if (w.type == GuiWidget::HEADER) {ImGui::PopID(); ImGui::Unindent(); } \
	} \
	if (w.type == GuiWidget::WINDOW) ImGui::End(); \
}

#define VAR(VT,VN,INIT,WIDGET) Gui(#VN,v.VN,WIDGET);

#define BUNCH(VT,VN,WIDGET) \
	if (ImGui::CollapsingHeader(#VN)) \
		for (int i = 0; i < v.VN.count; ++i) { \
			ImGui::PushID(i); \
			Gui("",v.VN[i],WIDGET); \
			ImGui::PopID(); \
		}

#define BIN(VT,VN,COUNT,WIDGET) \
	if (ImGui::CollapsingHeader(#VN)) \
		for (int i = 0; i < v.VN.count; ++i) { \
			ImGui::PushID(i); \
			Gui("",v.VN[i],WIDGET); \
			ImGui::PopID(); \
		}