#include <algorithm>

#include "GameObjectCollection.hpp"
#include "GameObject.hpp"

void GameObjectCollection::add(const std::string& name, std::shared_ptr<GameObject> object) {
    m_objects.emplace(name, object);
    m_objectsDrawOrder.push_back(object);
}

void GameObjectCollection::remove(const std::string &name) {
    auto objectIter = m_objects.find(name);
    if (objectIter == m_objects.end()) {
        // No such object
        return;
    }

    m_objectsDrawOrder.erase(
        std::remove(m_objectsDrawOrder.begin(), m_objectsDrawOrder.end(), objectIter->second),
        m_objectsDrawOrder.end());
}

void GameObjectCollection::sort() noexcept {
    // Insertion sort for draw order (most objects will not move while sorting)
    for (size_t i = 1; i < m_objectsDrawOrder.size(); i++) {
        for (int j = i; j > 0; j--) {
            auto object1 = m_objectsDrawOrder[j - 1];
            auto object2 = m_objectsDrawOrder[j];
            if (object1->getZIndex() <= object2->getZIndex()) {
                break;
            }

            std::swap(m_objectsDrawOrder[j], m_objectsDrawOrder[j - 1]);
        }
    }
}

std::vector<std::shared_ptr<GameObject>>::iterator GameObjectCollection::begin() {
    return m_objectsDrawOrder.begin();
}

std::vector<std::shared_ptr<GameObject>>::iterator GameObjectCollection::end() {
    return m_objectsDrawOrder.end();
}
