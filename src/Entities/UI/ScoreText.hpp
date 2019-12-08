//
// Created by matte on 2019-12-05.
//

#ifndef VAPE_SCORETEXT_HPP
#define VAPE_SCORETEXT_HPP

#include "Text.hpp"

class ScoreText : public Text
{
private:
    float m_life;

public:
    bool init(Font *font) override;

    void scroll_up(float ms);

    bool is_alive();
};



#endif //VAPE_SCORETEXT_HPP
