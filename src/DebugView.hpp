#ifndef DEBUGVIEW_HPP
#define DEBUGVIEW_HPP

#include <vector>

#include "GameObject.hpp"


class DebugView : public GameObject {
public:
    virtual void update(Game&, GameObjectCollection&, float deltaTime) noexcept override;

    virtual float getZIndex() const noexcept override;

private:
    std::vector<sf::FloatRect> m_boundingBoxes;

    virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;
};


#endif // DEBUGVIEW_HPP
