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
    #include <sys/time.h>
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

public:
    VideoUtil();

    bool open(const char* filename);
    void close();

    bool readFrame();
    
    // TODO getters
};


#endif //VAPE_VIDEOUTIL_HPP
