//
// Created by Cody on 11/17/2019.
//

#ifndef VAPE_VIDEO_HPP
#define VAPE_VIDEO_HPP

#include <common.hpp>
#include <Engine/Graphics/VideoUtil.hpp>

class Video : public EntityOld {
private:
    VideoUtil m_video_reader;
    GLuint m_tex_id;
public:
    bool init(const char* filename);
    void destroy();
    void update(float ms);
    void draw(const mat3& projection) override;
};


#endif //VAPE_VIDEO_HPP
