//
// Created by Cody on 11/17/2019.
//

#ifndef VAPE_VIDEOUTIL_HPP
#define VAPE_VIDEOUTIL_HPP

extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libavfilter/avfilter.h>
    #include <libavdevice/avdevice.h>
    #include <libswresample/swresample.h>
    #include <libswscale/swscale.h>
    #include <libavutil/avutil.h>
}

class VideoUtil {
private:
    AVFormatContext * m_format_ctx;
    AVCodecContext * m_codec_ctx;
    SwsContext* m_sws_ctx;
    int m_stream_idx;
    AVStream * m_video_stream;
    AVCodec * m_decoder;
    AVPacket * m_packet;
    AVFrame * m_frame;
    uint8_t* m_frame_buffer;

public:
    VideoUtil();

    bool open(const char* filename);
    void close();

    bool readFrame();

    // TODO getters
    inline int getWidth() const {return (m_codec_ctx) ? m_codec_ctx->width : -1;};
    inline int getHeight() const {return (m_codec_ctx) ? m_codec_ctx->height : -1;};
    inline uint8_t* getFrameBuffer() const { return m_frame_buffer; };
    inline double getTimeStamp() const { return (m_frame && m_frame->pts) ? m_frame->pts * (double)m_video_stream->time_base.num / (double)m_video_stream->time_base.den : -1;};
};


#endif //VAPE_VIDEOUTIL_HPP
