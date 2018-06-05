#ifndef _IRenderer_h_
#define _IRenderer_h_

#include "../util.h"

class IRenderer {
public:
	virtual void draw(Attrib *attrib);
	virtual void gen_buffer();
private:
};

#endif