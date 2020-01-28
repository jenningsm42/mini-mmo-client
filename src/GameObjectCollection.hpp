#ifndef GAMEOBJECTCOLLECTION_HPP
#define GAMEOBJECTCOLLECTION_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class GameObject;

class GameObjectCollection {
public:
    void add(const std::string& name, std::shared_ptr<GameObject>);
    void remove(const std::string& name);

    template <class T>
    std::shared_ptr<T> get(const std::string& name) const {
        if (m_objects.find(name) != m_objects.end()) {
            return std::dynamic_pointer_cast<T>(m_objects.at(name));
        }

        return nullptr;
    }

    // Sort should be called once per frame only
    void sort() noexcept;

    std::vector<std::shared_ptr<GameObject>>::iterator begin();
    std::vector<std::shared_ptr<GameObject>>::iterator end();

private:
    std::unordered_map<std::string, std::shared_ptr<GameObject>> m_objects;
    std::vector<std::shared_ptr<GameObject>> m_objectsDrawOrder;
};

#endif // GAMEOBJECTCOLLECTION_HPP
