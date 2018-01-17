#include "pm.h"

class Main {
private:
	pm::Texture* tex;
	pm::Sprite sprite;
public:
	void create() {
		tex = new pm::Texture("tex.png");
		tex->print_info();
		
		sprite = pm::Sprite(tex);
	}

	void render() {
		sprite.draw();
	}
	
	~Main() {
		delete tex;
	}
};
