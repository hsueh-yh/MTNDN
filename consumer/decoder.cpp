
#include "decoder.h"

Decoder::Decoder(void)
	: avcdll(NULL) 
	, pdec(NULL)
	, pdecContext(NULL)
	, pdecFrame(NULL)
	, pp_context_(NULL)
	, pp_mode_(NULL)
	, prodll(NULL)
	, utildll(NULL)
{
}

Decoder::~Decoder(void)
{
}

bool Decoder::LoadDllFun()
{
	const char* avcode = "./ffmpeg/lib/libavcodec.so";
	const char* util = "./ffmpeg/lib/libavutil.so";
	const char* postpro = "./ffmpeg/lib/libpostproc.so";
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
	utildll->loadFunction((void**)&av_free, "av_free");
	if (!utildll->ok)
	{
		cout << "load util dll error" << endl;
		return false;
	}
///*
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
//*/
	return true;
}

bool Decoder::InitDeocder(int width, int height, uint8_t *sps, int spslen, uint8_t *pps, int ppslen )
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

	pdecContext->extradata = new uint8_t[spslen+ppslen];
	pdecContext->extradata_size = spslen+ppslen;

	memcpy(pdecContext->extradata, sps, spslen);
	memcpy(pdecContext->extradata+spslen, pps, ppslen);

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
	///*
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
	//*/
	return true;
}

bool Decoder::decode( unsigned char * inbuf, const int & inlen, unsigned char * outbuf, int & outlen)
{
	int got_frame;
	BYTE* showImage[3];
	int showheight[3], showLx[3];

	int len;
	avpkt.size = inlen;
	avpkt.data = inbuf;
	///cout << "avpkt:" << inlen << " " << strlen((char*)avpkt.data) << endl;

	cout << endl;
	for( int i = 0; i <20; i++ )
			printf("%X ",inbuf[i]);
	std::cout << std::endl << std::endl;

	cout << pdecContext->extradata_size << endl;
//	cout <<endl<<endl<<endl;
//	for ( int i = 0; i < pdecContext->extradata_size; i++ )
//		printf("%X ", pdecContext->extradata[i]);
//	cout <<endl<<endl<<endl;

	len = avcodec_decode_video2(pdecContext, pdecFrame, &got_frame, &avpkt);
	if (len < 0)
	{
		cout << "decode error ";
		return false;
	}
	cout << "got_frame:" << got_frame << endl;
	if (got_frame)
	{
		showImage[0] = outbuf;
		showImage[1] = showImage[0] + m_width*m_height;
		showImage[2] = showImage[1] + m_width*m_height / 4;
		showLx[0] = m_width; showLx[1] = m_width >> 1; showLx[2] = m_width >> 1;
		showheight[0] = m_height; showheight[1] = m_height >> 1; showheight[2] = m_height >> 1;
		cout << "pp_postprocess" <<endl;
//		pp_postprocess(pdecFrame->data, pdecFrame->linesize, showImage, showLx, m_width, m_height, pdecFrame->qscale_table,
//			pdecFrame->qstride, pp_mode_, pp_context_, pdecFrame->pict_type);
		//GetImage(	pdecFrame->data,
		//			showImage,
		//			pdecFrame->linesize,
		//			showLx,
		//			showheight);
		cout << "pp_postprocess done" <<endl;
		outlen = m_width*m_height * 3 / 2;

		//cout << "pdecFrame:" << pdecFrame->linesize << " " << pdecFrame->width << "*" << pdecFrame->height << endl;

		//memcpy(outbuf,pdecFrame->data,outlen);
		//cout << "done" << endl;
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
	///*
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
	//*/
}

void Decoder::ReleaseConnection()
{
	delete this;
}
