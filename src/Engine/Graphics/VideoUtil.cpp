//
// Created by Cody on 11/17/2019.
//

#include <iostream>
#include <fstream>
#include <memory>
#include "VideoUtil.hpp"

VideoUtil::VideoUtil() {
    m_file_stream = nullptr;
    m_format_ctx = nullptr;
    m_codec_ctx = nullptr;
    m_sws_ctx = nullptr;
    m_stream_idx = -1;
    m_video_stream = nullptr;
    m_decoder = nullptr;
    m_packet = nullptr;
    m_frame = nullptr;
}

static int readFunction(void* opaque, uint8_t* buf, int buf_size) {
    auto& me = *reinterpret_cast<std::istream*>(opaque);
    me.read(reinterpret_cast<char*>(buf), buf_size);

    if (me.good())
        return (int)me.gcount();
    else
    {
        if (me.eof())
            return AVERROR_EOF;
        else
            return AVERROR_EXTERNAL;
    }
}

static int64_t seekFunction(void* ptr, int64_t pos, int whence) {
    auto& me = *reinterpret_cast< std::istream*>(ptr);
    switch (whence)
    {
        case AVSEEK_SIZE:
            return AVERROR_EXTERNAL;
        case SEEK_SET:
            me.seekg(pos, std::ios_base::beg);
            return me.good() ? (int64_t)me.tellg() : AVERROR_EXTERNAL;
        case SEEK_CUR:
            me.seekg(pos, std::ios_base::cur);
            return me.good() ? (int64_t)me.tellg() : AVERROR_EXTERNAL;
        case SEEK_END:
            me.seekg(pos, std::ios_base::end);
            return me.good() ? (int64_t)me.tellg() : AVERROR_EXTERNAL;
        default:
            return AVERROR_EXTERNAL;
    }
}

bool VideoUtil::open(const char *filename) {
    // TODO cleanup on fails

    av_log_set_level(AV_LOG_DEBUG); // TODO remove
    if (!PhysFS::exists(filename))
    {
        std::cout << "Unable to find " << filename << std::endl;
        return false; //file doesn't exist
    }

    // A IStream - you choose where it comes from
    m_file_stream = new PhysFS::ifstream(filename);

    // Alloc a buffer for the stream
    const int ioBufferSize = 8192;
    //const std::shared_ptr<unsigned char> buffer(reinterpret_cast<unsigned char*>(av_malloc(ioBufferSize + AV_INPUT_BUFFER_PADDING_SIZE)), &av_free);
    m_buffer = reinterpret_cast<unsigned char*>(av_malloc(ioBufferSize + AV_INPUT_BUFFER_PADDING_SIZE));

    if (!m_buffer){
        std::cout << "Could not allocate buffer.\n";
    }


    // Get a AVContext stream
    AVIOContext* ioContext = avio_alloc_context(
            m_buffer,    // Buffer
            ioBufferSize,  // Buffer size
            0,                   // Buffer is only readable - set to 1 for read/write
            reinterpret_cast<void*>(m_file_stream),      // User (your) specified data
            &readFunction,      // Function - Reading Packets (see example)
            nullptr,                   // Function - Write Packets
            &seekFunction       // Function - Seek to position in stream (see example)
    );
    if(ioContext == nullptr){
        fprintf(stderr, "Failed to allocate context!\n");
        close();
        return false;
    }

    // Allocate a AVContext
    m_format_ctx = avformat_alloc_context();

    // Set up the Format Context
    m_format_ctx->pb = ioContext;
    m_format_ctx->flags |= AVFMT_FLAG_CUSTOM_IO; // we set up our own IO

    // Open video with avformat
    if (avformat_open_input(&m_format_ctx, filename, nullptr, nullptr) < 0) {
        fprintf(stderr, "Failed to open video file!\n");
        close();
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
        av_free(m_format_ctx->pb);
        avformat_free_context(m_format_ctx);
    }
    if (m_buffer) {
        av_free(m_buffer);
    }
    delete m_file_stream;
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

    return false;
}

