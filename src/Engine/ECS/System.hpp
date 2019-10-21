//
// Created by Cody on 10/16/2019.
//

#ifndef VAPE_SYSTEM_HPP
#define VAPE_SYSTEM_HPP

#include <cstddef>

namespace ECS {
    using SystemId = std::size_t;

    // Returns a unique, incremeneted Id
    inline SystemId getSystemId() {
        static SystemId lastId = 0;
        return lastId++;
    };

    // Returns a unique id for each type T of component;
    template <typename T> inline SystemId getSystemTypeId() noexcept {
        static SystemId typeId = getSystemId();
        return typeId;
    }

    class System {
        friend class SystemManager;
    private:
        SystemId id;
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
