#if !defined(CHAINPOWERUP)
#define CHAINPOWERUP

#include "Power_Up.h"
#include "Data.h"

class Chainpowerup : public Power_Up
{
public:
    Chainpowerup(float const, float const, Data const&, Data_desc const&, float const&);
    ~Chainpowerup() = default;

    virtual std::string get_type_name() override;
    std::shared_ptr<Weapon> get_weapon(std::shared_ptr<Game_Object> const&) override;
protected:

};

#endif