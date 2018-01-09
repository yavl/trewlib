#include "pm.h"

class Main {
private:
public:
	pm::Texture tex;

	void create() {
		tex = pm::init_texture("tex.png");
		tex.print_info();
	}

	void render() {
		pm::draw_texture(tex, 0, 0, 800, 600);
	}
};