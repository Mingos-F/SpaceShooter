#pragma once

#include "Entity.h"
#include "EntityManager.h"

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

struct PlayerConfig
{
    int SR, CR, FR, FG, FB, OR, OG, OB, OT, V;
    float S;
};

struct EnemyConfig
{
    int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI;
    float SMIN, SMAX;
};

struct BulletConfig
{
    int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L;
    float S;
};

class Game
{
    sf::RenderWindow m_window; // the window we will draw to
    EntityManager m_entities;  // vector of entities to maintain
    sf::Font m_font;
    sf::Text m_text; // the score text
    PlayerConfig m_playerConfig;
    EnemyConfig m_enemyConfig;
    BulletConfig m_bulletConfig;
    sf::Clock m_deltaClock;
    int m_score = 0;
    int m_currentFrame = 0;
    int m_lastEnemySpawnTime = 0;
    bool m_paused = false; // wether we update the game logic
    bool m_running = true; // wether the game is running
    std::shared_ptr<Entity> m_player;

public:
    Game(const std::string &config);
    void init(const std::string &path);

    void run();
    void setPaused(bool paused);
    void spawnPlayer();
    void spawnEnemy();
    void spawnSmallEnemies(std::shared_ptr<Entity> e);
    void SpawnBullets(std::shared_ptr<Entity> entity, const Vec2 &target);
    void spawnSpecialWeapon(std::shared_ptr<Entity> entity);
    void sMovement();
    void sLifespan();
    void sCollision();
    void sEnemySpawner();
    void sGUI();
    void sRender();
    void sUserInput();
};