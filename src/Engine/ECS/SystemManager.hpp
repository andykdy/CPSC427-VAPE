//
// Created by Cody on 10/16/2019.
//

#ifndef VAPE_SYSTEMMANAGER_HPP
#define VAPE_SYSTEMMANAGER_HPP

namespace ECS {
    constexpr std::size_t maxSystems = 32;

    using SystemBitSet = std::bitset<maxSystems>;
    using SystemArray = std::array<System*, maxSystems>;



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

            systemArray[getSystemTypeId<T>()] = system;
            systemBitSet[getSystemTypeId<T>()] = true;

            return *system;
        }

        template<typename T> T* getSystem() const {
            auto ptr(systemArray[getSystemTypeId<T>()]);
            return static_cast<T*>(ptr);
        }
    };
}

#endif //VAPE_SYSTEMMANAGER_HPP
