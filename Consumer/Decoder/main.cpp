
#include "decoder.h"
#include "libpostproc/postprocess.h"

#define INBUF_SIZE 100 * 1024;


static int FindStartCode(unsigned char *Buf, int zeros_in_startcode)
{
	int info;
	int i;

	info = 1;
	for (i = 0; i < zeros_in_startcode; i++)
	{
		if (Buf[i] != 0)
			info = 0;
	}

	if (Buf[i] != 1)
		info = 0;
	return info;
}

static bool Check_StartCode(unsigned char *Buf, int pos)
{
	int info3 = 0;

	info3 = FindStartCode(&Buf[pos - 4], 3);
	return info3 == 1;

}

static int getNextNal(FILE* inpf, unsigned char* Buf)
{
	int pos = 0;
	int StartCodeFound = 0;
	int info2 = 0;
	int info3 = 0;

	int nCount = 0;
	while (!feof(inpf) && ++nCount <= 4)
	{
		Buf[pos++] = fgetc(inpf);
	}

	if (!Check_StartCode(Buf, pos))
	{
		return 0;
	}


	while (!feof(inpf) && (Buf[pos++] = fgetc(inpf)) == 0);

	//find the next start code
	while (!StartCodeFound)
	{
		//end of file
		if (feof(inpf))
		{
			//			return -1;
			return pos - 1;
		}
		Buf[pos++] = fgetc(inpf);

		StartCodeFound = Check_StartCode(Buf, pos);
	}

	fseek(inpf, -4, SEEK_CUR);

	// return the end(length) of this NALU
	return pos - 4;
}

int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		printf("please input: PP_Demo.exe filename1[input] Width Height filename2[output]\n");
		//return 0;
	}

	argv[1] = "out.264";
	argv[2] = "640";
	argv[3] = "480";
	argv[4] = "outdec.yuv";

	//params set
	unsigned short usWidth = atoi(argv[2]);
	unsigned short usHeight = atoi(argv[3]);
	
	//create dec&pp
	Decoder *pdec = new Decoder;
	if (!pdec->InitDeocder(usWidth, usHeight))
	{
		return false;
	}


	unsigned char *p_In_Frame = new unsigned char[usWidth * usHeight * 3 / 2];
	unsigned char *p_Out_Frame = new unsigned char[usWidth * usHeight * 3 / 2];
	FILE* ifp = fopen( "out.264", "rb");
	FILE* ofp = fopen( "outdec.yuv", "wb");

	if (  ifp ==NULL || ofp==NULL  )
	{
		cout << "fopen error" << endl;
		getchar ();
		return 0;
	}
		
	bool b_continue = true;
	int nReadUnit = usWidth * usHeight * 3 / 2;

	int num = 0;
	while (!feof(ifp))
	{
		int nCount = getNextNal(ifp, p_In_Frame);

		if (nCount == 0)
		{
			continue;
		}
		
		fflush ( stdout );
		unsigned char *ptr = p_In_Frame;
		int n_Outlen = 0;

		unsigned char testbuf;
		fseek(ifp, 4, SEEK_CUR);
		testbuf = fgetc(ifp);
		printf( "ifp %X\n", ifp );
		printf("NALU1 Head:%X\n", testbuf);
		fseek(ifp, -5, SEEK_CUR);
		//cout << "NALU Head:" << testbuf << endl;

		unsigned char t1[13];
		unsigned char t2[9];
		int i =0;
		if (num == 0)
		{
			while (i<nCount){
				t1[i] = fgetc(ifp);
				printf(" %X ", t1[i]);
				i++;
			}
			cout << endl;
			fseek(ifp, -nCount, SEEK_CUR);
		}
		i = 0;
		
		if (num == 1)
		{
			while (i < nCount){
				t2[i] = fgetc(ifp);
				printf(" %X ", t2[i]);
				i++;
			}
			cout << endl;
			fseek(ifp, -nCount, SEEK_CUR);
		}
		
			


		cout << "Frame: No=" << ++num << " Length=" << nCount << endl;
		pdec->decode(ptr, nCount, p_Out_Frame, n_Outlen);

		if (n_Outlen > 0)
		{
			fwrite(p_Out_Frame, 1, n_Outlen, ofp);
		}
	}


	//realse
	delete[]p_In_Frame;
	delete[]p_Out_Frame;
	pdec->StopDecoder();
	pdec->ReleaseConnection();
	fclose(ifp);
	fclose(ofp);

	return 0;
}
