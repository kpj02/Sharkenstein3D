#if !defined(WEAPON)
#define WEAPON

#include <vector>
#include <stack>
#include <memory>
#include <string>

#include "Game_Object.h"
#include "Dynamic.h"
#include "Data.h"

class Weapon : public Dynamic
{
public:
    Weapon(std::shared_ptr<Game_Object> const&, Data const&, Data_desc const&);
    virtual ~Weapon() = default;
    virtual void set_expired();
    void render(sf::RenderWindow &, sf::Vector2f, float, std::vector<float>) override;
    virtual std::string get_status() = 0;
    void render_hud(sf::RenderWindow&);
    void load_text();
protected:
    std::weak_ptr<Game_Object> player_ptr;
    float weapon_time;
    sf::Font font;
    sf::Text text;
};

#endif