#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 99999 // Data부분을 충분히 큰값으로 설정

typedef struct Ethernet {
	char DestinationAddr[13];
	char SourceAddr[13];
	char Type[5];
}Ethernet; 
// Ethernet부분을 구별하기 위한 구조체

typedef struct IPHeader {
	char VER[2];
	char HLEN[2];
	char ServiceType[3];
	char TotalLength[5];
	char Identification[5];
	char FlagsAndOffset[5];
	char TTL[3];
	char Protocol[3];
	char HeaderChecksum[5];
	char SourceIPAddr[9];
	char DestinationIPAddr[9];
}IPHeader;
// IP헤더부분을 구별하기 위한 구조체

typedef struct Header {
	char SourcePortAddr[5];
	char DestinationPortAddr[5];
	char UDPTotalLength[5];
	char Checksum[5];
}Header;
// UDP내의 헤더값을 구하기 위한 구조체

typedef struct UDPHeader {
	unsigned short SourceIPAddr[2];
	unsigned short DestinationIPAddr[2];
	unsigned char Protocol;
	unsigned char All0s;
	unsigned short UDPTotalLength;
	unsigned short SourcePortAddr;
	unsigned short DestinationPortAddr;
	unsigned short UDPTotalLength2;
	unsigned short Checksum;
	unsigned short Data[30];
}UDPHeader;
// UDP의 전체값을 구해 checksum에 사용하기 위해 선언된 구조체

void Stringcopy(char* Dest, char* Src, int start, int end) // 문자열에서 원하는 해당 위치만큼 복사한다.
{
	for (int i = start; i < end; i++)
		Dest[i - start] = Src[i];
	Dest[4] = NULL;
}

unsigned short Check_sum(unsigned short *p, int CheckByte) // 체크섬 함수 // CheckByte는 총 검사할 바이트수
{
	long sum = 0; 
	// 4바이트의 자료형 long선언
	// short변수는 2바이트기 때문에 checksum값을 2바이트씩 연산하므로 그 값을 저장하기 위해 long변수 선언
	while (CheckByte >=1){
		if(CheckByte==1)
			sum += (*(unsigned short*)p++)&0xFF00; //1바이트값을 이동하여 연산 sum값에 더함
		else
			sum += *((unsigned short*)p)++; //sum값에 더함 
		CheckByte -= 2; // 2개씩 만큼 감소
	}
	
	
		sum = (sum >> 16) + (sum & 0xFFFF);
	// sum >> 16 은 앞의 4자리 값
	// sum & 0xFFFF는 4자리 뒤의값
	// 오버플로우가 난 캐리값을 sum에 넣어서 더해주는 while문

	return ~sum; // 보수를 취해서 리턴
}
int main(void)
{
	FILE* fp = fopen("udp3.txt", "r"); // 파일 read
	Ethernet ethernet;
	IPHeader ipheader;
	UDPHeader udpHeader;
	Header header;
	// 구조체 변수 선언
	
	int OptionAndPaddingSize; // 옵션 및 패딩사이즈를 배열크기로 할당하여 계산할 때 사용
	char *buf; //Option + padding부분의 동적할당을 위한 변수선언
	char Data[N]; // 충분히 큰 크기(N)으로 설정
	char temp[5]; // Stringcopy함수를 이용하여 2바이트씩 끊어 복사하기 위해 선언
	unsigned short checksum;
 
	ethernet.DestinationAddr[12] = ethernet.SourceAddr[12] = ethernet.Type[4] = NULL; 
	// 배열의 마지막 값을 초기화하여 쓰레기값이 나오지 않게 함
	
	fread(ethernet.DestinationAddr, sizeof(char), 12, fp);
	fread(ethernet.SourceAddr, sizeof(char), 12, fp);
	fread(ethernet.Type, sizeof(char), 4, fp);
	// 파일을 필요한 크기만큼 읽어서 ethernet구조체에 있는 각각의 변수에 저장

	printf("------Ethernet-------\n");
	printf("Destination address : %s\n", ethernet.DestinationAddr);
	printf("Source address : %s\n", ethernet.SourceAddr);
	printf("Type : %s\n", ethernet.Type);
	// ethernet구조의 값 출력

	ipheader.VER[1] = ipheader.HLEN[1] = ipheader.ServiceType[2] = ipheader.TotalLength[4] = NULL;
	ipheader.Identification[4] = ipheader.FlagsAndOffset[4] = ipheader.TTL[2] = ipheader.Protocol[2] = NULL;
	ipheader.HeaderChecksum[4] = ipheader.SourceIPAddr[8] = ipheader.DestinationIPAddr[8] = NULL;
	// 배열의 마지막 값을 초기화하여 쓰레기값이 나오지 않게 함

	fread(ipheader.VER, sizeof(char), 1, fp);
	fread(ipheader.HLEN, sizeof(char), 1, fp);
	fread(ipheader.ServiceType, sizeof(char), 2, fp);
	fread(ipheader.TotalLength, sizeof(char), 4, fp);
	fread(ipheader.Identification, sizeof(char), 4, fp);
	fread(ipheader.FlagsAndOffset, sizeof(char), 4, fp);
	fread(ipheader.TTL, sizeof(char), 2, fp);
	fread(ipheader.Protocol, sizeof(char), 2, fp);
	fread(ipheader.HeaderChecksum, sizeof(char), 4, fp);
	fread(ipheader.SourceIPAddr, sizeof(char), 8, fp);
	fread(ipheader.DestinationIPAddr, sizeof(char), 8, fp);
	// 파일을 필요한 크기만큼 읽어서 ipheader구조체에 있는 각각의 변수에 저장
	
	printf("------IP Header------\n");
	printf("VER : V%s\n", ipheader.VER);
	printf("HLEN : %s\n", ipheader.HLEN);
	printf("Service type : %s\n", ipheader.ServiceType);
	printf("Total length : %s\n", ipheader.TotalLength);
	printf("Identification : %s\n", ipheader.Identification);
	printf("Flags and Offset : %s\n", ipheader.FlagsAndOffset);
	printf("TTL : %s\n", ipheader.TTL);
	printf("Protocol : %s\n", ipheader.Protocol);
	printf("Header checksum : %s\n", ipheader.HeaderChecksum);
	printf("Source IP address : %s\n", ipheader.SourceIPAddr);
	printf("Destinatoin IP address : %s\n", ipheader.DestinationIPAddr);
	// IP Header구조의 값 출력

	OptionAndPaddingSize = atoi(ipheader.VER)*atoi(ipheader.HLEN);
	// atoi는 문자를 숫자로 변경 // 구조체 내에서 VER과 HLEN을 char형으로 받았기 때문이다.

	if (OptionAndPaddingSize > 20)
	{
		buf = (char*)malloc(sizeof(char)*OptionAndPaddingSize - 20); // 옵션 및 패딩크기가 20보다 클경우 그 크기만큼 동적할당을 해주고
		fread(buf, sizeof(char), OptionAndPaddingSize - 20, fp); // 동적할당한 크기만큼의 크기를 읽는다
		buf[OptionAndPaddingSize - 20] = NULL; // 추가된 옵션 및 패딩부분의 buf 배열값을 초기화
		printf("Option + padding : %dbytes\n", OptionAndPaddingSize - 20); // 옵션 및 패딩 부분의 바이트크기 출력
	}

	header.SourcePortAddr[4] = header.DestinationPortAddr[4] = header.UDPTotalLength[4] = header.Checksum[4] = NULL;
	// 배열의 마지막 값을 초기화하여 쓰레기값이 나오지 않게 함

	fread(header.SourcePortAddr, sizeof(char), 4, fp);
	fread(header.DestinationPortAddr, sizeof(char), 4, fp);
	fread(header.UDPTotalLength, sizeof(char), 4, fp);
	fread(header.Checksum, sizeof(char), 4, fp);
	// 파일을 필요한 크기만큼 읽어서 header구조체에 있는 각각의 변수에 저장

	printf("------UDP Header------\n");
	printf("Source port address : %s\n", header.SourcePortAddr);
	printf("Destination port address : %s\n", header.DestinationPortAddr);
	printf("UDP total length : %s\n", header.UDPTotalLength);
	printf("UDP checksum : %s\n", header.Checksum);
	// UDP구조의 값 출력

	if (strcmp(header.Checksum, "0000") != 0)
		strcpy(header.Checksum, "0000");
	// checksum값이 0000이 아닐경우 0000으로 초기화

	memset(Data, 0, sizeof(char) * N); // Data을 시작주소로 선언해 메모리를 채운다
	fgets(Data, N, fp);

	if (strlen(Data) / 2 % 2 == 1) // 데이터의 길이가 16비트로 끝나지 않을경우 == 1바이트(8비트)의 값이 남았을 경우
	{
		Data[strlen(Data)] = '0';
		Data[strlen(Data)] = '0';
		// 데이터의 부분이 16비트로 나누어지지 않는 경우 0으로 패딩
		// 배열의 마지막 값을 초기화하여 쓰레기값이 나오지 않게 함
	}
	printf("Data : %s\n", Data);

	Stringcopy(temp, ipheader.SourceIPAddr, 0, 4);
	udpHeader.SourceIPAddr[0] = (unsigned short)strtol(temp, NULL, 16);
	Stringcopy(temp, ipheader.SourceIPAddr, 4, 8);
	udpHeader.SourceIPAddr[1] = (unsigned short)strtol(temp, NULL, 16);
	Stringcopy(temp, ipheader.DestinationIPAddr, 0, 4);
	udpHeader.DestinationIPAddr[0] = (unsigned short)strtol(temp, NULL, 16);
	Stringcopy(temp, ipheader.DestinationIPAddr, 4, 8);
	udpHeader.DestinationIPAddr[1] = (unsigned short)strtol(temp, NULL, 16);
	
	udpHeader.All0s = (char)strtol("00", NULL, 16);	
	udpHeader.Protocol = (char)strtol("11", NULL, 16);
	udpHeader.UDPTotalLength = (unsigned short)strtol(header.UDPTotalLength, NULL, 16);
	udpHeader.SourcePortAddr = (unsigned short)strtol(header.SourcePortAddr, NULL, 16);
	udpHeader.DestinationPortAddr = (unsigned short)strtol(header.DestinationPortAddr, NULL, 16);
	udpHeader.UDPTotalLength2 = udpHeader.UDPTotalLength;
	udpHeader.Checksum = (unsigned short)strtol(header.Checksum, NULL, 16);
	// Stringcopy함수를 통해 2바이트씩 나누어서 저장. 나중에 checksum연산을 위해 UDPHeader를 2바이트씩 나누었다.
	// Checksum함수를 보면 long(4바이트)자료형으로 short(2바이트)자료형으로 더해진 값을 받아 checksum연산을 하기 때문이다.
	// strtol => 문자열의값을 정수로 변환(16진수 값 변환을 위해 사용)
	// strtol => (변환대상의 시작포인터, 변환대상의 종료포인터(NULL), N진수)
	// 의사헤더부분

	for (int i = 0; i < strlen(Data); i += 4)
	{
		Stringcopy(temp, Data, i, i + 4);
		udpHeader.Data[i / 4] = (unsigned short)strtol(temp, NULL, 16);
	}
	// UDPHeader Data부분을 2바이트씩 나누어서 strtol을 통해 문자열의 값을 정수로 변환(16진수 값 변환을 위해 사용)
	// 헤더 + 데이터 부분

	udpHeader.Checksum = Check_sum((unsigned short*)&udpHeader, udpHeader.UDPTotalLength + 12); 
	// UDPTotalLength => 헤더+데이터 // 12 => 의사헤더
	//Check_sum(UDPHeader의 값 전체 checksum, UDPTotalLength값으로 헤더부분과 데이터부분을 구한 값에 의사헤더 12바이트를 더해주었다)
	printf("checksum : %X\n", udpHeader.Checksum);
	printf("checksum error check : %04x\n", Check_sum((unsigned short*)&udpHeader, udpHeader.UDPTotalLength +12));
	// checksum값을 앞에서 나온 checksum값으로 넣어서 다시 checksum계산
	printf("----------------------\n");
}