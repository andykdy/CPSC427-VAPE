//
// Created by Cody on 12/6/2019.
//

#ifndef VAPE_ENTITYGRID_HPP
#define VAPE_ENTITYGRID_HPP


#include <vector>
#include <Entities/Enemies/Enemy.hpp>
#include <Entities/Pickups/Pickup.hpp>
#include "common.hpp"

class Pickup;

enum EType {
    empty,
    player,
    vamp,
    enemy,
    goal,
    projectile_friendly,
    projectile_hostile,
};

struct pair {
    int x;
    int y;
};

// Search nodes
struct Node {
    pair position;
    pair parent;
    float f;
    float g;
    float h;
    bool operator < (const Node& r) const
    {
        return f < r.f;
    }
};

// Function pointer types for search destination/heuristc
typedef bool isDestinationFn(const std::vector<std::vector<EType>>& grid, pair pos, pair dest);
typedef float heuristicFn(const std::vector<std::vector<EType>>& grid, pair pos, pair dest);

// For rendering square colors based on type
class EntityGridSquare : public EntityOld {
public:
    bool init(float size);
    void destroy();
    void draw(const mat3 &projection) override; // Unused
    void draw(const mat3 &projection, vec2 pos, EType type);
};

// A grid for the screen tracking ETypes in each square
class EntityGrid : public EntityOld{
public:
    // Generate grid based on (screen) width and height, using squares of size size
    bool init(int width, int height, int size);

    // Cleanup
    void destroy();

    // Set all squares as empty
    void clear();

    // Add entities to grid, by calculating which squares they're in and setting them as the appropriate EType
    void addToGrid(const Player& player);
    void addToGrid(const Vamp& vamp);
    void addToGrid(const Enemy& enemy);
    void addToGrid(const Boss& boss);
    void addToGrid(const Pickup& pickup);
    void addToGrid(const Projectile& projectile);
    void addBoxToGrid(vec2 tl, vec2 br, EType type);

    // Draw the grid
    void draw(const mat3 &projection) override;

    // Find path for fish to edge of screen
    //std::vector<vec2> getPath(const Fish& fish);
    // Find path for turtle to player
    //std::vector<vec2> getPath(const Turtle& turtle, const Salmon& salmon);
    // Find path from salmon to fish
    //std::vector<vec2> getPath(const Salmon& salmon, const std::vector<Fish>& fishes);
private:
    std::vector<std::vector<EType>> grid;
    // How many squares wide
    int gridW;
    // How many squares tall
    int gridH;
    // Side length of squares
    int size;

    // Grid vertices for drawing
    std::vector<Vertex> m_vertices;
    std::vector<uint16_t> m_indices;
    // Draws square colors
    EntityGridSquare square;
    // Debugging function for printing node information
    static void printNode(Node node);
    // Check node within grid
    bool isValid(int x, int y);
    // A* Search function
    std::vector<vec2> search(vec2 position, vec2 bbox, std::vector<EType> avoid, isDestinationFn destFn, heuristicFn h,  pair dest={-1,-1}, bool DEBUG_LOG=false);
};


#endif //VAPE_ENTITYGRID_HPP
