#ifndef ANIMATEDSPRITE_HPP
#define ANIMATEDSPRITE_HPP

#include <unordered_map>
#include <tuple>
#include <SFML/Graphics.hpp>

class AnimatedSprite : public sf::Sprite {
public:
    AnimatedSprite();

    /* setGridSize sets the number of columns and rows the
     * associated sprite sheet has. Must be called after
     * setting the texture.*/
    void setGridSize(int columns, int rows) noexcept;

    /* addAnimation will add a new named animation to the
     * internal dictionary. It requires the starting column
     * and row, the number of frames before repeating, and
     * the time in seconds that each frame will be displayed
     * for. The parameters are bounded by the grid size so
     * that sprite rectangles aren't created outside of the
     * given texture. */
    void addAnimation(
            const std::string& name,
            int startColumn,
            int startRow,
            int frameCount,
            float frameTime) noexcept;

    /* play switches to the given named animation */
    void play(const std::string& name) noexcept;

    void update(float deltaTime) noexcept;

private:
    int m_columns;
    int m_rows;

    int m_currentColumn;
    int m_currentRow;
    float m_currentFrameTime;
    std::string m_currentAnimationName;
    std::tuple<int, int, int, float> m_currentAnimation;
    sf::IntRect m_spriteRectangle; // Determines which part of texture to draw

    // Maps animation name -> (column #, row #, frame count, frame time (in seconds))
    std::unordered_map<std::string, std::tuple<int, int, int, float>> m_animations;

    void nextFrame() noexcept;

    void updateSpriteRectangle() noexcept;
};

#endif // ANIMATEDSPRITE_HPP
