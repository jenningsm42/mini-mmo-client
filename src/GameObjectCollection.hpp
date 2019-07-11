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

    std::shared_ptr<GameObject> get(const std::string& name) const noexcept;

    // Sort should be called once per frame only
    void sort() noexcept;

    std::vector<std::shared_ptr<GameObject>>::iterator begin();
    std::vector<std::shared_ptr<GameObject>>::iterator end();

private:
    std::unordered_map<std::string, std::shared_ptr<GameObject>> m_objects;
    std::vector<std::shared_ptr<GameObject>> m_objectsDrawOrder;
};

#endif // GAMEOBJECTCOLLECTION_HPP
