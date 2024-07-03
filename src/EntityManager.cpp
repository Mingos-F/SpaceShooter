#include "EntityManager.h"

EntityManager::EntityManager()
{
}

void EntityManager::update()
{
    // TODO add entities from m_entitiesToAdd to the proper locations (vector of all entities and to the map)

    // remove dead entities from the vector off all entities
    removeDeadEntities(m_entities);

    // remove dead entities from the map
    for (auto &[tag, entityVec] : m_entitiesMap)
    {
        removeDeadEntities(entityVec);
    }
}

void EntityManager::removeDeadEntities(EntityVec &vec)
{
    // TODO remove all dead entities from the input vector
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag)
{
    auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
    m_entitiesToAdd.push_back(entity);
    return entity;
}

const EntityVec &EntityManager::getEntities()
{
    return m_entities;
}

const EntityVec &EntityManager::getEntities(const std::string &tag)
{
    // TODO this is incorrect return the correct vector from the map
    return m_entities;
}

const EntityMap &EntityManager::getEntityMap()
{
    return m_entitiesMap;
}