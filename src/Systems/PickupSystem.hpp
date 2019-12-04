//
// Created by Andy on 12/03/2019.
//

#ifndef VAPE_PICKUPSYSTEM_HPP
#define VAPE_PICKUPSYSTEM_HPP


#include <Engine/ECS/System.hpp>
#include <Entities/Pickups/Pickup.hpp>

class Pickup;

class PickupSystem : public ECS::System {
public:
	std::vector<Pickup*> pickups;
	void update(float ms) override;
	std::vector<Pickup*> *getPickups();
	void clear();
};


#endif //VAPE_PICKSYSTEM_HPP
