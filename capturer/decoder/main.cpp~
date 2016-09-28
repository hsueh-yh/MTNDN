
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
	cout << "111111111" << endl;

	while (!feof(inpf) && (Buf[pos++] = fgetc(inpf)) == 0);

	//find the next start code
	while (!StartCodeFound)
	{
		//end of file
		if (feof(inpf))
		{
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

	char outputFile[20] = "output.264";
	char inputFile[20] = "outdec.yuv";
	int width = 640;
	int height = 480;

	//params set
	//unsigned short usWidth = atoi(argv[2]);
	//unsigned short usHeight = atoi(argv[3]);
	unsigned short usWidth = width;
	unsigned short usHeight = height;
	
	//create dec&pp
	Decoder *pdec = new Decoder;
	if (!pdec->InitDeocder(usWidth, usHeight))
	{
		return false;
	}


	unsigned char *p_In_Frame = new unsigned char[usWidth * usHeight * 3 / 2];
	unsigned char *p_Out_Frame = new unsigned char[usWidth * usHeight * 3 / 2];

	unsigned char *ptr = NULL;

	FILE* in_file = fopen( inputFile, "rb");
	FILE* out_file = fopen( outputFile, "wb");

	if (  in_file ==NULL || out_file==NULL  )
	{
		cout << "open output.264 error" << endl;
		return 0;
	}

	int nReadUnit = usWidth * usHeight * 3 / 2;

	cout << "******************************************************" << endl;

	int num = 0, nCount = 0, n_Outlen = 0;
	while (!feof(in_file))
	{
		nCount = getNextNal(in_file, p_In_Frame);

		if (nCount == 0)
		{
			cout << "######## no NALU ######## " << nCount << endl;
			continue;
		}

		fflush ( stdout );
		ptr = p_In_Frame;
		n_Outlen = 0;

		unsigned char testbuf;
		fseek(in_file, 4, SEEK_CUR);
		testbuf = fgetc(in_file);
		//printf( "in_file %X\n", SEEK_CUR );
		printf("NALU Head:%X\n", testbuf);
		fseek(in_file, -5, SEEK_CUR);
		//cout << "NALU Head:" << testbuf << endl;

		unsigned char t1[13];
		unsigned char t2[9];
		int i =0;
		if (num == 0)
		{
			while (i<nCount){
				t1[i] = fgetc(in_file);
				printf(" %X ", t1[i]);
				i++;
			}
			cout << endl;
			fseek(in_file, -nCount, SEEK_CUR);
		}
		i = 0;
		
		if (num == -1)
		{
			while (i < nCount){
				t2[i] = fgetc(in_file);
				printf(" %X ", t2[i]);
				i++;
			}
			cout << endl;
			fseek(in_file, -nCount, SEEK_CUR);
		}
		
			


		cout << "Frame: No=" << ++num << " Length=" << nCount << endl;
		pdec->decode(ptr, nCount, p_Out_Frame, n_Outlen);

		if (n_Outlen > 0)
		{
			fwrite(p_Out_Frame, 1, n_Outlen, out_file);
		}
		cout << endl;
	}

	cout << "Frame: No=" << ++num << " Length=" << nCount << endl;
	pdec->decode(NULL, 0, p_Out_Frame, n_Outlen);

	if (n_Outlen > 0)
	{
		fwrite(p_Out_Frame, 1, n_Outlen, out_file);
	}
	cout << endl;

	cout << "******************************************************" << endl << endl;


	//realse
	delete[]p_In_Frame;
	delete[]p_Out_Frame;
	pdec->StopDecoder();
	pdec->ReleaseConnection();
	fclose(in_file);
	fclose(out_file);

	return 0;
}
