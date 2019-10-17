//
// Created by Cody on 10/16/2019.
//

#ifndef VAPE_SYSTEMMANAGER_HPP
#define VAPE_SYSTEMMANAGER_HPP

namespace ECS {
    constexpr std::size_t maxSystems = 32;

    using SystemId = std::size_t;
    using SystemBitSet = std::bitset<maxSystems>;
    using SystemArray = std::array<System*, maxSystems>;

    // Returns a unique, incremeneted Id
    inline SystemId getSystemId() {
        static SystemId lastId = 0;
        return lastId++;
    };

    // Returns a unique id for each type T of component;
    template <typename T> inline SystemId getSystemId() noexcept {
        static SystemId typeId = getSystemId();
        return typeId;
    }

    class SystemManager {
    private:
        std::vector<std::unique_ptr<System>> systems;

        SystemArray systemArray; // Systems indexed by typeId
        SystemBitSet systemBitSet; // Quick True/False lookup for if has system;

    public:
        void update(float ms) {
            for (auto & s : systems) s->update(ms);
        }

        // https://en.cppreference.com/w/cpp/algorithm/remove
        void refresh() {
            // This erases from the entities vector based on which entities are not active
            // see remove_if docs for more details, but it basically just shifts all
            // inactive elements end of the vector, and returns an iterator which erase uses.
            systems.erase(std::remove_if(systems.begin(), systems.end(), [](const std::unique_ptr<System> &system) {
                return !system->isActive();
            }), systems.end());
        }

        template <typename T, typename... TArgs> T& addSystem(TArgs&&... mArgs) {
            T *system(new T(std::forward<TArgs>(mArgs)...));
            std::unique_ptr<System> uPtr{system};
            systems.emplace_back((std::move(uPtr)));

            systemArray[getComponentTypeId<T>()] = system;
            systemBitSet[getComponentTypeId<T>()] = true;

            system->init();
            return *system;
        }
    };
}

#endif //VAPE_SYSTEMMANAGER_HPP
