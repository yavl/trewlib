#include "pm.h"

class Main {
private:
	pm::Texture* tex;
	pm::Sprite sprite;
	pm::Camera cam;
public:
	void create() {
		tex = new pm::Texture("tex.png");
		tex->print_info();
		
		sprite = pm::Sprite(tex);
		sprite.get_texture()->print_info();
	}

	void render() {
		sprite.draw();
	}
	
	~Main() {
		delete tex;
	}
};