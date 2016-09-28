//#pragma once
#include "tdll.h"

#include <iostream>

using namespace std;

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/channel_layout.h"
#include "libavutil/common.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h" 
#include "libavutil/imgutils.h"
#include "libavutil/opt.h"
#include "libavutil/mathematics.h"
#include "libavutil/samplefmt.h"
#include "libpostproc/postprocess.h"
#include "libavutil/pixfmt.h"
};


#define BYTE unsigned char

class Decoder /*: public IH264Decoder*/
{
public:
	virtual bool InitDeocder(int width, int height);
	virtual bool decode(unsigned char * inbuf, const int & inlen, unsigned char * outbuf, int & outlen);
	virtual void StopDecoder();
	virtual void ReleaseConnection();

public:
	Decoder(void);
	virtual ~Decoder(void);
private:

	bool LoadDllFun();
	//void(*avcodec_init)(void);
	void(*avcodec_register_all)(void);
	void(*av_init_packet)(AVPacket *pkt);
	AVCodecContext* (*avcodec_alloc_context3)(const AVCodec *codec);
	AVFrame* (*av_frame_alloc)(void);
	int (*av_image_fill_arrays)(uint8_t *dst_data[4], int dst_linesize[4],
                         const uint8_t *src,
                         enum AVPixelFormat pix_fmt, int width, int height, int align);
	AVCodec *(*avcodec_find_decoder)(enum AVCodecID id);
	//int(*avcodec_decode_video)(AVCodecContext *avctx, AVFrame *picture, int *got_picture_ptr,
	//	uint8_t *buf, int buf_size);
	int(*avcodec_decode_video2)(AVCodecContext *avctx, AVFrame *picture,
								int *got_picture_ptr, AVPacket *avpkt);
	//int(*avcodec_open)(AVCodecContext *avctx, AVCodec *codec);
	int(*avcodec_open2)(AVCodecContext *avctx, const AVCodec *codec, AVDictionary **options);
	int(*avcodec_close)(AVCodecContext *avctx);
	void(*av_free)(void *ptr);

	bool InitPostproc(int w, int h);
	void ClosePostproc();
	//pp_context *(*pp_get_context)(int width, int height, int flags);
	//void(*pp_free_context)(pp_context *ppContext);
	//void(*pp_free_mode)(pp_mode *mode);
	pp_mode *(*pp_get_mode_by_name_and_quality)(char *name, int quality);
	/*
	void(*pp_postprocess)(uint8_t * src[3], int srcStride[3],
		uint8_t * dst[3], int dstStride[3],
		int horizontalSize, int verticalSize,
		QP_STORE_T *QP_store, int QP_stride,
		pp_mode *mode, pp_context *ppContext, int pict_type);
		*/
private:
	AVCodec			*pdec;
	AVCodecContext	*pdecContext;
	AVFrame			*pdecFrame;
	AVPacket		avpkt;
	int				m_width;
	int				m_height;

	Tdll *avcdll;
	Tdll *utildll;
	//Tdll* prodll;
	//pp_context *pp_context_;
	//pp_mode    *pp_mode_;
};
