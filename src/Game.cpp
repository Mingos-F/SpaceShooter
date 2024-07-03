#include "Game.h"

#include <iostream>

Game::Game(const std::string &config)
{
    init(config);
}

void Game::init(const std::string &path)
{
    // TODO read the config file here

    // set up default window parameters
    m_window.create(sf::VideoMode(1280, 720), "test");
    m_window.setFramerateLimit(60);

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

// respawn the player in the middle of the screen
void Game::spawnPlayer()
{
    // TODO finish adding all properties of the player with the correct values from the config

    // create every entity by calling EntityManager.addEntity(tag)
    auto entity = m_entities.addEntity("player");

    // give the entity a transform so it spawn in 200 200 with velocity 1 1 and angle
    entity->cTransform = std::make_shared<CTransform>(Vec2(200.0f, 200.0f), Vec2(1.0f, 1.0f), 0.0f);

    // the shape will have radius 32 8 sides dark grey fill and red outline of thickness 4
    entity->cShape = std::make_shared<CShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);

    // add input component to the player
    entity->cInput = std::make_shared<CInput>();

    // since we want this entity to be our player set the games player var to this entity
    m_player = entity;
}

void Game::spawnEnemy()
{
    // TODO make sure the enemy is spawned with the config values and withing bounds of the widow

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

    // set the position of the shape based on the entities transform pos
    m_player->cShape->circle.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);

    // set rotation of the shape based on the entities angle
    m_player->cTransform->angle += 1.0f;
    m_player->cShape->circle.setRotation(m_player->cTransform->angle);

    // draw the entities shape
    m_window.draw(m_player->cShape->circle);

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