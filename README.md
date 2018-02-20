# pm
very tiny cross-platform game library
```c++
#include "pm.h"

class Main {
private:
    pm::Texture* tex;
    pm::Texture* tex2;
    pm::Sprite sprite;
    pm::Sprite sprite2;
public:
    void create() {
        tex = new pm::Texture("tex.png");
        tex2 = new pm::Texture("tex2.png");
        
        sprite = pm::Sprite(tex);
        sprite.get_texture()->print_info();
        sprite.set_color(0, 0.4f, 0, 1.0f);
        sprite.set_pos(100, 0);
        
        sprite2 = pm::Sprite(tex2);
    }
    
    void render() {
        sprite.draw();
        sprite2.draw();
    }
    
    ~Main() {
        delete tex;
        delete tex2;
    }
};
```
