#include "Game.h"

#include <iostream>
#include <fstream>
#include <random>

float generate_random_float(float lower_bound, float upper_bound);
int generate_random_int(int lower_bound, int upper_bound);

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

    int numberVerticies = generate_random_int(m_enemyConfig.VMIN, m_enemyConfig.VMAX);

    auto entity = m_entities.addEntity("enemy");
    entity->cTransform = std::make_shared<CTransform>(Vec2(generate_random_float(m_enemyConfig.SR, m_window.getSize().x - m_enemyConfig.SR), generate_random_float(m_enemyConfig.SR, m_window.getSize().y - m_enemyConfig.SR)), Vec2(generate_random_float(m_enemyConfig.SMIN, m_enemyConfig.SMAX), generate_random_float(m_enemyConfig.SMIN, m_enemyConfig.SMAX)), 0.0f);
    entity->cShape = std::make_shared<CShape>(m_enemyConfig.SR, numberVerticies, sf::Color(generate_random_int(0,255), generate_random_int(0,255), generate_random_int(0,255)), sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);
    entity->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);
    entity->cScore = std::make_shared<CScore>(numberVerticies*100);

    // record when most recent enemy was spawned
    m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
    // TODO spawn small enemies at the location of the inout enemy
}

// spawns a bullet from a given entity to a target location
void Game::SpawnBullets(std::shared_ptr<Entity> entity, const Vec2 &target)
{
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
}

void Game::sMovement()
{
    // TODO implement all entity movement here
    // read m_player->cInput to check if player is moving

    // sample movement
    m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
    m_player->cTransform->pos.y += m_player->cTransform->velocity.y;
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
    // TODO change the code to draw all of  the entities

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
    // TODO handle user input
    // do ont implement the logic of movement here

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
                std::cout << "W key pressed" << std::endl;
                break;

            // exit game
            case sf::Keyboard::Escape:
                m_window.close();
                exit(0);
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
                std::cout << "W key released" << std::endl;
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
                std::cout << "left mouse clicked at (" << event.mouseButton.x << "," << event.mouseButton.y << ")" << std::endl;
            }

            if (event.mouseButton.button == sf::Mouse::Right)
            {
                std::cout << "right mouse clicked at (" << event.mouseButton.x << "," << event.mouseButton.y << ")" << std::endl;
            }
        }
    }
}


float generate_random_float(float lower_bound, float upper_bound) {

    // Seed with a real random value, if available
    static std::random_device rd;
    // Use a static generator to maintain state across function calls
    static std::mt19937 gen(rd());
    // Define the distribution with the specified range
    std::uniform_real_distribution<float> dis(lower_bound, upper_bound);

    // Generate and return a random float number in the range [lower_bound, upper_bound]
    return dis(gen);
}

int generate_random_int(int lower_bound, int upper_bound) {

    // Seed with a real random value, if available
    static std::random_device rd;
    // Use a static generator to maintain state across function calls
    static std::mt19937 gen(rd());
    // Define the distribution with the specified range
    std::uniform_real_distribution<> dis(lower_bound, upper_bound);

    // Generate and return a random float number in the range [lower_bound, upper_bound]
    return dis(gen);
}