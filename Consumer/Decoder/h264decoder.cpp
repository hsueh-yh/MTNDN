
#include "h264decoder.h"

h264decoder::h264decoder(void)
	: avcdll(NULL) 
	, pdec(NULL)
	, pdecContext(NULL)
	, pdecFrame(NULL)
	, pp_context(NULL)
	, pp_mode(NULL)
	, prodll(NULL)
	, utildll(NULL)
{
}

h264decoder::~h264decoder(void)
{
}

bool h264decoder::LoadDllFun()
{
	avcdll = new Tdll(L"avcodec-57.dll");
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

	utildll = new Tdll(L"avutil-55.dll");
	utildll->loadFunction((void**)&av_frame_alloc, "av_frame_alloc");
	utildll->loadFunction((void**)&av_free, "av_free");
	if (!utildll->ok)
	{
		cout << "load util dll error" << endl;
		return false;
	}

	prodll = new Tdll(L"postproc-54.dll");
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

	return true;
}

bool h264decoder::InitH264Deocder(int width, int height)
{
	if (!LoadDllFun())
		return false;
	
	//avcodec_init();
	cout << "avcodec_register_all..." << endl;
	avcodec_register_all();

	if (!InitPostproc(width, height))
		return false;

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

bool h264decoder::InitPostproc(int w, int h)
{
	cout << "InitPostproc" << endl;
	int i_flags = 0;
	i_flags = PP_CPU_CAPS_MMX | 
				PP_CPU_CAPS_MMX2 | 
				PP_FORMAT_420;
	
	pp_context = pp_get_context(w, h, i_flags);
	
	if (!pp_context)
		return false;
	
	pp_mode = pp_get_mode_by_name_and_quality("default", 6);
	
	if (!pp_mode)
		return false;
	
	return true;
}

bool h264decoder::H264Decode(unsigned char * inbuf, const int & inlen, unsigned char * outbuf, int & outlen)
{
	int got_frame;
	BYTE* showImage[3];
	int showheight[3], showLx[3];

	int len;
	avpkt.size = inlen;
	avpkt.data = inbuf;
	
	len = avcodec_decode_video2(pdecContext, pdecFrame, &got_frame, &avpkt);
	if (len < 0)
		return false;
	
	if (got_frame)
	{
		showImage[0] = outbuf;
		showImage[1] = showImage[0] + m_width*m_height;
		showImage[2] = showImage[1] + m_width*m_height / 4;
		showLx[0] = m_width; showLx[1] = m_width >> 1; showLx[2] = m_width >> 1;
		showheight[0] = m_height; showheight[1] = m_height >> 1; showheight[2] = m_height >> 1;
		pp_postprocess(pdecFrame->data, pdecFrame->linesize, showImage, showLx, m_width, m_height, pdecFrame->qscale_table,
			pdecFrame->qstride, pp_mode, pp_context, pdecFrame->pict_type);
		//GetImage(	pdecFrame->data,
		//			showImage,
		//			pdecFrame->linesize,
		//			showLx,
		//			showheight);
		outlen = m_width*m_height * 3 / 2;
	}
	else
	{
		outlen = 0;
	}

	return true;
}

void h264decoder::StopH264Decoder()
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

void h264decoder::ClosePostproc()
{
	if (pp_mode) {
		pp_free_mode(pp_mode);
		pp_mode = 0;
	}
	if (pp_context) {
		pp_free_context(pp_context);
		pp_context = 0;
	}
	if (prodll) {
		delete prodll;
		prodll = 0;
	}
}

void h264decoder::ReleaseConnection()
{
	delete this;
}
