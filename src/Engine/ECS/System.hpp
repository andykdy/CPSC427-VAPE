//
// Created by Cody on 10/16/2019.
//

#ifndef VAPE_SYSTEM_HPP
#define VAPE_SYSTEM_HPP

namespace ECS {
    class System {
    private:
        bool active = true;
    public:
        virtual void update(float ms) {}

        bool isActive() { return active; };
        void destroy() { active = false; };

        // Deconstructor
        virtual ~System() {}
    };
}

#endif //VAPE_SYSTEM_HPP
