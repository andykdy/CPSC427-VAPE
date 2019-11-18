//
// Created by Cody on 11/17/2019.
//

#include "VideoUtil.hpp"

VideoUtil::VideoUtil() {
    m_format_ctx = nullptr;
    m_codec_ctx = nullptr;
    m_sws_ctx = nullptr;
    m_stream_idx = -1;
    m_video_stream = nullptr;
    m_decoder = nullptr;
    m_packet = nullptr;
    m_frame = nullptr;
}

bool VideoUtil::open(const char *filename) {
    // TODO cleanup on fails

    // Open video with avformat
    if (avformat_open_input(&m_format_ctx, filename, nullptr, nullptr) < 0) {
        fprintf(stderr, "Failed to open video file!\n");
        return false;
    }

    // Find stream info
    if (avformat_find_stream_info(m_format_ctx, nullptr) < 0) {
        fprintf(stderr, "Failed to read stream info!\n");
        close();
        return false;
    }

    // Find the best video stream
    m_stream_idx = av_find_best_stream(m_format_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (m_stream_idx < 0)
    {
        fprintf(stderr, "Could not find video stream in input file\n");
        close();
        return false;
    }
    m_video_stream = m_format_ctx->streams[m_stream_idx];

    // Find decoder for stream codec
    m_decoder = avcodec_find_decoder(m_video_stream->codecpar->codec_id);
    if (m_decoder == nullptr) {
        fprintf(stderr, "Could not find decoder\n");
        close();
        return false;
    }

    // Set up codec context for the decoder
    m_codec_ctx = avcodec_alloc_context3(m_decoder);
    if (!m_codec_ctx) {
        fprintf(stderr, "Could not create the codec context\n");
        close();
        return false;
    }
    if (avcodec_parameters_to_context(m_codec_ctx, m_video_stream->codecpar) < 0) {
        fprintf(stderr, "Could not initialize the codec context\n");
        close();
        return false;
    }
    if (avcodec_open2(m_codec_ctx, m_decoder, nullptr) < 0) {
        fprintf(stderr, "Could not open codec\n");
        close();
        return false;
    }

    // Allocate video frame and packet
    m_frame = av_frame_alloc();
    if (!m_frame) {
        fprintf(stderr, "Couldn't allocate AVFrame\n");
        return false;
    }
    m_packet = av_packet_alloc();
    if (!m_packet) {
        fprintf(stderr, "Couldn't allocate AVPacket\n");
        return false;
    }

    m_frame_buffer = new uint8_t[getWidth() * getHeight() * 4];

    return true;
}

void VideoUtil::close() {
    if (m_sws_ctx) {
        sws_freeContext(m_sws_ctx);
    }
    if (m_frame) av_frame_free(&m_frame);
    if (m_packet) av_packet_free(&m_packet);
    if (m_codec_ctx) {
        avcodec_close(m_codec_ctx);
        avcodec_free_context(&m_codec_ctx);
    }
    if (m_format_ctx) {
        avformat_close_input(&m_format_ctx);
        avformat_free_context(m_format_ctx);
    }
}

bool VideoUtil::readFrame() {
    while (av_read_frame(m_format_ctx, m_packet) >= 0) {
        if (m_packet->stream_index != m_stream_idx) {
            av_packet_unref(m_packet);
            continue;
        }

        // https://ffmpeg.org/doxygen/trunk/group__lavc__encdec.html
        // Send the packet to the decoder
        if (avcodec_send_packet(m_codec_ctx, m_packet) < 0) {
            fprintf(stderr, "Failed to decode packet\n");
            return false;
        }

        // Receive frame from the decoder
        int res = avcodec_receive_frame(m_codec_ctx, m_frame);
        if (res == AVERROR(EAGAIN) || res == AVERROR_EOF) {
            av_packet_unref(m_packet);
            continue;
        } else if (res < 0) {
            fprintf(stderr, "Failed to decode packet\n");
            return false;
        }

        av_packet_unref(m_packet);
        break;
    }

    // Set up sws scaler
    if (!m_sws_ctx) {
        m_sws_ctx = sws_getContext(getWidth(), getHeight(), m_codec_ctx->pix_fmt,
                                        getWidth(), getHeight(), AV_PIX_FMT_RGB0,
                                        SWS_BILINEAR, NULL, NULL, NULL);
    }
    if (!m_sws_ctx) {
        fprintf(stderr, "Failed to initialize SWS \n");
        return false;
    }

    uint8_t* dest[4] = { m_frame_buffer, nullptr, nullptr, nullptr };
    int dest_linesize[4] = { getWidth() * 4, 0, 0, 0 };
    sws_scale(m_sws_ctx, m_frame->data, m_frame->linesize, 0, m_frame->height, dest, dest_linesize);

    return true;
}

