
#include "decoder.h"

Decoder::Decoder(void)
	: avcdll(NULL) 
	, pdec(NULL)
	, pdecContext(NULL)
	, pdecFrame(NULL)
	//, pp_context_(NULL)
	//, pp_mode_(NULL)
	//, prodll(NULL)
	, utildll(NULL)
{
}

Decoder::~Decoder(void)
{
}

bool Decoder::LoadDllFun()
{
	const char* avcode = "/usr/local/lib/libavcodec.so";
	const char* util = "/usr/local/lib/libavutil.so";
	const char* postpro = "/usr/local/lib/libpostproc.so";
	///*
	avcdll = new Tdll(avcode);
	avcdll->loadFunction((void**)&avcodec_register_all, "avcodec_register_all");
	avcdll->loadFunction((void**)&av_init_packet, "av_init_packet");
	avcdll->loadFunction((void**)&avcodec_alloc_context3, "avcodec_alloc_context3");
	avcdll->loadFunction((void**)&avcodec_find_decoder, "avcodec_find_decoder");
	avcdll->loadFunction((void**)&avcodec_open2, "avcodec_open2");
	avcdll->loadFunction((void**)&avcodec_decode_video2, "avcodec_decode_video2");
	avcdll->loadFunction((void**)&avcodec_close, "avcodec_close");
	//avcdll->loadFunction((void**)&av_free, "av_free");
	if (!avcdll->ok)
	{
		cout << "load avcodec dll error" << endl;
		return false;
	}

	utildll = new Tdll(util);
	utildll->loadFunction((void**)&av_frame_alloc, "av_frame_alloc");
	utildll->loadFunction((void**)&av_image_fill_arrays, "av_image_fill_arrays");
	utildll->loadFunction((void**)&av_free, "av_free");
	if (!utildll->ok)
	{
		cout << "load util dll error" << endl;
		return false;
	}
/*
	prodll = new Tdll(postpro);
	prodll->loadFunction((void**)&pp_get_context, "pp_get_context");
	prodll->loadFunction((void**)&pp_free_context, "pp_free_context");
	prodll->loadFunction((void**)&pp_free_mode, "pp_free_mode");
	prodll->loadFunction((void**)&pp_get_mode_by_name_and_quality, "pp_get_mode_by_name_and_quality");
	prodll->loadFunction((void**)&pp_postprocess, "pp_postprocess");
	if (!prodll->ok)
	{
		cout << "load prodll error" << endl;
		return false;
	}
*/
	return true;
}

bool Decoder::InitDeocder(int width, int height)
{
	if (!LoadDllFun())
		return false;
	
	//avcodec_init();
	cout << "avcodec_register_all..." << endl;
	avcodec_register_all();
	cout << "done." << endl;
	//if (!InitPostproc(width, height))
	//	return false;

	cout << "av_init_packet..." << endl;
	av_init_packet(&avpkt);

	m_width = width;
	m_height = height;
	cout << "avcodec_find_decoder..." << endl;
	pdec = avcodec_find_decoder(AV_CODEC_ID_H264);
	
	if (pdec == NULL)
		return false;

	pdecContext = avcodec_alloc_context3(pdec);
	pdecFrame = av_frame_alloc();

	pdecContext->width = width;
	pdecContext->height = height;
	pdecContext->pix_fmt = AV_PIX_FMT_YUV420P;
	
	/* open it */
	cout << "avcodec_open2" << endl;
	if (avcodec_open2(pdecContext, pdec, NULL) < 0)
	{
		cout << "avcodec_open2 error!" << endl;
		return false;
	}
	return true;
}

bool Decoder::InitPostproc(int w, int h)
{
	/*
	cout << "InitPostproc" << endl;
	int i_flags = 0;
	i_flags = PP_CPU_CAPS_MMX | 
				PP_CPU_CAPS_MMX2 | 
				PP_FORMAT_420;
	
	pp_context_ = pp_get_context(w, h, i_flags);
	
	if (!pp_context_)
		return false;
	
	pp_mode_ = pp_get_mode_by_name_and_quality("default", 6);
	
	if (!pp_mode_)
		return false;
	*/
	return true;
}

bool Decoder::decode(unsigned char * inbuf, const int & inlen, unsigned char * outbuf, int & outlen)
{
	int got_frame;
	BYTE* showImage[3];
	int showheight[3], showLx[3];

	int len;
	avpkt.size = inlen;
	avpkt.data = inbuf;

	av_image_fill_arrays(pdecFrame->data, pdecFrame->linesize,
                             outbuf, AV_PIX_FMT_YUV420P,
                             pdecContext->width, pdecContext->height,1);
	
	len = avcodec_decode_video2(pdecContext, pdecFrame, &got_frame, &avpkt);
	if (len < 0)
		return false;
	
	if (got_frame)
	{
		outlen = m_width*m_height * 3 / 2;
	}
	else
	{
		outlen = 0;
	}

	return true;
}

void Decoder::StopDecoder()
{
	if (pdecContext != NULL)
	{
		avcodec_close(pdecContext);
		av_free(pdecContext);
		pdecContext = NULL;
		if (pdecFrame) {
			av_free(pdecFrame);
			pdecFrame = NULL;
		}
	}
	if (avcdll) {
		delete avcdll;
		avcdll = 0;
	}

	ClosePostproc();
}

void Decoder::ClosePostproc()
{
	/*
	if (pp_mode_) {
		pp_free_mode(pp_mode_);
		pp_mode_ = 0;
	}
	if (pp_context_) {
		pp_free_context(pp_context_);
		pp_context_ = 0;
	}
	if (prodll) {
		delete prodll;
		prodll = 0;
	}
	*/
}

void Decoder::ReleaseConnection()
{
	delete this;
}
