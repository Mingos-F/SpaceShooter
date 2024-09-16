#include "EntityManager.h"

#include <iostream>

EntityManager::EntityManager()
{
    m_entitiesMap["enemy"] = EntityVec{};
    m_entitiesMap["smallenemy"] = EntityVec{};
    m_entitiesMap["bullet"] = EntityVec{};
}

void EntityManager::update()
{
    // add entities from m_entitiesToAdd to the proper locations (vector of all entities and to the map)
    for (auto &entity : m_entitiesToAdd)
    {
        m_entities.push_back(entity);

        // find returns an iterator either to the element with tag "tag" or to the end of map if tag not found
        auto it = m_entitiesMap.find(entity->tag());

        // tag found?
        if (it != m_entitiesMap.cend())
        {
            // insert in the vector of entities
            it->second.push_back(entity);
        }
        else
        {
            // create new key value pair
            m_entitiesMap[entity->tag()] = EntityVec{entity};
        }
    }

    // clear the entities to add vector
    m_entitiesToAdd.clear();

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
    // remove all dead entities from the input vector

    vec.erase(std::remove_if(vec.begin(), vec.end(), [](const std::shared_ptr<Entity> &entity)
                             { return !entity->isActive(); }),
              vec.end());
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
    auto it = m_entitiesMap.find(tag);

    // tag found not found
    if (it == m_entitiesMap.cend())
    {
        std::cerr << "Tag not found, (EntityManager get entities)" << std::endl;
        exit(-1);
    }

    return m_entitiesMap.at(tag);
}

const EntityMap &EntityManager::getEntityMap()
{
    return m_entitiesMap;
}