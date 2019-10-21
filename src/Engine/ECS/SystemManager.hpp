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
            refresh();
        }

        // https://en.cppreference.com/w/cpp/algorithm/remove
        void refresh() {
            // This erases from the systems map based on which systems are not active
            // see remove_if docs for more details, but it basically just shifts all
            // inactive elements end of the vector, and returns an iterator which erase uses.
            systems.erase(std::remove_if(systems.begin(), systems.end(), [this](const std::unique_ptr<System> &system) {
                if (!system->isActive()){
                    systemBitSet[system->id];
                    // TODO systemArray?
                }
                return !system->isActive();
            }), systems.end());
        }


        template <typename T> bool hasSystem() const {
            return systemBitSet[getSystemTypeId<T>()];
        }

        template<typename T> T* getSystem() {
            if (hasSystem<T>()) {
                auto ptr(systemArray[getSystemTypeId<T>()]);
                return static_cast<T *>(ptr);
            } else return addSystem<T>();
        }

        template <typename T, typename... TArgs> T* addSystem(TArgs&&... mArgs) {
            if (!hasSystem<T>()){
                T *system(new T(std::forward<TArgs>(mArgs)...));
                SystemId id = getSystemTypeId<T>();
                system->id = id;
                std::unique_ptr<System> uPtr{system};
                systems.emplace_back((std::move(uPtr)));

                systemArray[id] = system;
                systemBitSet[id] = true;

                return system;
            } else return getSystem<T>();
        }

        void reset() {
            systemArray = std::array<System*, maxSystems>{};
            systemBitSet.reset();
            systems.clear();
        }
    };
}

#endif //VAPE_SYSTEMMANAGER_HPP
