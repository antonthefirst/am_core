#include "texture.h"

bool TexDesc::operator==(const TexDesc& rhs) const {
	return memcmp(this, &rhs, sizeof(TexDesc)) == 0;
}
bool TexDesc::operator!=(const TexDesc& rhs) const {
	return !operator==(rhs);
}