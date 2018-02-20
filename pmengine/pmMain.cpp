#include "pm/pm.hpp"
#include <vector>

using namespace std;
using namespace pm;

class Main {
private:
    Texture* tex;
    Texture* tex2;
    Sprite sprite, sprite2, sprite3;
    vector<pm::Sprite> sprites;
public:
    void create() {
        tex = new Texture("tex.png");
        tex2 = new Texture("tex2.png");
        
        sprite = Sprite(tex);
        sprite.get_texture()->print_info();
        sprite.set_color(0, 0.4f, 0, 1.0f);
        sprite.set_pos(0, 0);
        
        sprite2 = Sprite(tex);
        sprite2.set_pos(200, 100);
        
        sprite3 = Sprite(tex2);
        sprite3.set_pos(300, 200);
        
        sprites.push_back(sprite);
        sprites.push_back(sprite2);
        sprites.push_back(sprite3);
    }
    
    void render() {
        for (Sprite& x : sprites) {
            x.draw();
        }
    }
    
    ~Main() {
        delete tex;
        delete tex2;
    }
};


