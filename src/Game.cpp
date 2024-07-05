#include "Game.h"

#include <iostream>
#include <fstream>
#include <random>
#include <math.h>

float generateRandom(float lower_bound, float upper_bound);

Game::Game(const std::string &config)
{
    init(config);
}

void Game::init(const std::string &path)
{
    // read the config file here

    std::fstream fin(path);

    std::string id;

    if (!fin)
    {
        std::cerr << "Failed to open the configuration file" << std::endl;
        exit(-1);
    }

    while (fin >> id)
    {
        if (id == "Window")
        {
            fin >> m_windowConfig.W >> m_windowConfig.H >> m_windowConfig.FR >> m_windowConfig.FS;
        }
        else if (id == "Fonts")
        {
            fin >> m_fontConfig.P >> m_fontConfig.S >> m_fontConfig.R >> m_fontConfig.G >> m_fontConfig.B;
        }
        else if (id == "Player")
        {
            fin >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S >> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB >> m_playerConfig.OT >> m_playerConfig.V;
        }
        else if (id == "Enemy")
        {
            fin >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX >> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT >> m_enemyConfig.VMIN >> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SI;
        }
        else if (id == "Bullet")
        {
            fin >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB >> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB >> m_playerConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;
        }
    }

    // set up default window parameters
    if (m_windowConfig.FS == 0)
    {
        m_window.create(sf::VideoMode(m_windowConfig.W, m_windowConfig.H), "SpaceShooter");
    }
    else
    {
        m_window.create(sf::VideoMode::getFullscreenModes()[0], "SpaceShooter", sf::Style::Fullscreen);
    }

    m_window.setFramerateLimit(m_windowConfig.FR);
    m_window.setVerticalSyncEnabled(true);

    // set up the font
    if (!m_font.loadFromFile(m_fontConfig.P))
    {
        std::cerr << "Failed to load the font" << std::endl;
        exit(-1);
    }

    bool imGuiInit = ImGui::SFML::Init(m_window);

    if (!imGuiInit)
    {
        std::cerr << "Failed to initialize ImGui" << std::endl;
        exit(-1);
    }

    spawnPlayer();
}

void Game::run()
{

    // TODO add pause functionality in here
    //  some systems should run while paused rendering

    while (m_running)
    {
        // update the entity manager
        m_entities.update();

        // required update call to imgui
        ImGui::SFML::Update(m_window, m_deltaClock.restart());

        sEnemySpawner();
        sMovement();
        sCollision();
        sUserInput();
        sGUI();
        sRender();

        // increase the current frame
        //  may need to be moved when pause is implemented
        m_currentFrame++;
    }
}

void Game::setPaused(bool paused)
{
    // TODO
}

void Game::spawnPlayer()
{
    // create the player with the values from the config
    // create every entity by calling EntityManager.addEntity(tag)
    auto entity = m_entities.addEntity("player");
    entity->cTransform = std::make_shared<CTransform>(Vec2(m_window.getSize().x / 2, m_window.getSize().y / 2), Vec2(m_playerConfig.S, m_playerConfig.S), 0.0f);
    entity->cShape = std::make_shared<CShape>(m_playerConfig.CR, m_playerConfig.V, sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB), sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);
    entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);
    entity->cInput = std::make_shared<CInput>();

    // since we want this entity to be our player set the games player var to this entity
    m_player = entity;
}

void Game::spawnEnemy()
{
    // the enemy is spawned with the config values and withing bounds of the window

    int numberVertices = std::floor(generateRandom(m_enemyConfig.VMIN, m_enemyConfig.VMAX));

    auto entity = m_entities.addEntity("enemy");
    entity->cTransform = std::make_shared<CTransform>(Vec2(generateRandom(m_enemyConfig.SR, m_window.getSize().x - m_enemyConfig.SR), generateRandom(m_enemyConfig.SR, m_window.getSize().y - m_enemyConfig.SR)), Vec2(generateRandom(m_enemyConfig.SMIN, m_enemyConfig.SMAX), generateRandom(m_enemyConfig.SMIN, m_enemyConfig.SMAX)), 0.0f);
    entity->cShape = std::make_shared<CShape>(m_enemyConfig.SR, numberVertices, sf::Color(std::floor(generateRandom(0, 255)), std::floor(generateRandom(0, 255)), std::floor(generateRandom(0, 255))), sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);
    entity->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);
    entity->cScore = std::make_shared<CScore>(numberVertices * 100);

    // record when most recent enemy was spawned
    m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
    // TODO spawn small enemies at the location of the inout enemy
}

void Game::SpawnBullets(std::shared_ptr<Entity> entity, const Vec2 &target)
{
    auto bullet = m_entities.addEntity("bullet");
    bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, (entity->cTransform->pos.unitVector(target) * m_bulletConfig.S), 0.0f);
    bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V, sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB), sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);
    bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
    bullet->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
}

void Game::sMovement()
{

    for (auto &entity : m_entities.getEntities())
    {
        if (entity->cInput)
        {
            if (m_player->cInput->up && (m_player->cTransform->pos.y - m_player->cShape->circle.getRadius()) > 5.0f)
                m_player->cTransform->pos.y -= m_player->cTransform->velocity.y;

            if (m_player->cInput->down && (m_player->cTransform->pos.y + m_player->cShape->circle.getRadius()) < (m_window.getSize().y - 5.0f))
                m_player->cTransform->pos.y += m_player->cTransform->velocity.y;

            if (m_player->cInput->left && (m_player->cTransform->pos.x - m_player->cShape->circle.getRadius()) > 5.0f)
                m_player->cTransform->pos.x -= m_player->cTransform->velocity.x;

            if (m_player->cInput->right && (m_player->cTransform->pos.x + m_player->cShape->circle.getRadius()) < (m_window.getSize().x - 5.0f))
                m_player->cTransform->pos.x += m_player->cTransform->velocity.x;

            continue;
        }

        if ((entity->cTransform->pos.x - entity->cShape->circle.getRadius()) < 5.0f || (entity->cTransform->pos.x + entity->cShape->circle.getRadius()) > (m_window.getSize().x - 5.0f))
            entity->cTransform->velocity.x *= -1;

        if ((entity->cTransform->pos.y - entity->cShape->circle.getRadius()) < 5.0f || (entity->cTransform->pos.y + entity->cShape->circle.getRadius()) > (m_window.getSize().y - 5.0f))
            entity->cTransform->velocity.y *= -1;

        entity->cTransform->pos.x += entity->cTransform->velocity.x;
        entity->cTransform->pos.y += entity->cTransform->velocity.y;
    }
}

void Game::sLifespan()
{
    // for all entities
    //      if entity has no lifespan component skip it
    //      if entity has > 0 lifespan , subtract 1
    //      if it has lifespan and is alive
    //          scale alpha chanel
    //      if it has lifespan and its time is up
    //          destroy entity
}

void Game::sCollision()
{
}

void Game::sEnemySpawner()
{
    if (m_currentFrame == 0 || m_currentFrame > m_lastEnemySpawnTime + m_enemyConfig.SI)
    {
        spawnEnemy();
    }
}

void Game::sGUI()
{
    ImGui::Begin("SpaceShooter");
    ImGui::Text("Stuff goes here");
    ImGui::End();
}

void Game::sRender()
{
    // change the code to draw all of  the entities

    m_window.clear();

    for (auto &entity : m_entities.getEntities())
    {
        // entity->cShape->circle.setPosition(entity->cTransform->pos.x, entity->cTransform->pos.y);
        entity->cTransform->angle += 1.0f;
        entity->cShape->circle.setRotation(entity->cTransform->angle);
        entity->cShape->circle.setPosition(entity->cTransform->pos.x, entity->cTransform->pos.y);
        m_window.draw(entity->cShape->circle);
    }

    // draw the ui last
    ImGui::SFML::Render(m_window);

    m_window.display();
}

void Game::sUserInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        // pass the event to imgui to be parsed
        ImGui::SFML::ProcessEvent(m_window, event);

        // this event triggers when the window is closed
        if (event.type == sf::Event::Closed)
        {
            m_running = false;
        }

        // key pressed
        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::W:
                m_player->cInput->up = true;
                break;

            case sf::Keyboard::S:
                m_player->cInput->down = true;
                break;

            case sf::Keyboard::A:
                m_player->cInput->left = true;
                break;

            case sf::Keyboard::D:
                m_player->cInput->right = true;
                break;

            // exit game
            case sf::Keyboard::Escape:
                m_running = false;
                break;

            default:
                break;
            }
        }

        // key released
        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::W:
                m_player->cInput->up = false;
                break;

            case sf::Keyboard::S:
                m_player->cInput->down = false;
                break;

            case sf::Keyboard::A:
                m_player->cInput->left = false;
                break;

            case sf::Keyboard::D:
                m_player->cInput->right = false;
                break;

            default:
                break;
            }
        }

        //  mouse button pressed
        if (event.type == sf::Event::MouseButtonPressed)
        {

            // this line ignores mouse events if ImGui is the thin being clicked
            if (ImGui::GetIO().WantCaptureMouse)
            {
                continue;
            }

            if (event.mouseButton.button == sf::Mouse::Left)
            {
                SpawnBullets(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
            }

            if (event.mouseButton.button == sf::Mouse::Right)
            {
                spawnSpecialWeapon(m_player);
            }
        }
    }
}

float generateRandom(float lower_bound, float upper_bound)
{

    // Seed with a real random value, if available
    static std::random_device rd;
    // Use a static generator to maintain state across function calls
    static std::mt19937 gen(rd());
    // Define the distribution with the specified range
    std::uniform_real_distribution<float> dis(lower_bound, upper_bound);

    // Generate and return a random float number in the range [lower_bound, upper_bound]
    return dis(gen);
}
