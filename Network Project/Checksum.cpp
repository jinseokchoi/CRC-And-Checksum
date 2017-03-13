#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 99999 // Data�κ��� ����� ū������ ����

typedef struct Ethernet {
	char DestinationAddr[13];
	char SourceAddr[13];
	char Type[5];
}Ethernet; 
// Ethernet�κ��� �����ϱ� ���� ����ü

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
// IP����κ��� �����ϱ� ���� ����ü

typedef struct Header {
	char SourcePortAddr[5];
	char DestinationPortAddr[5];
	char UDPTotalLength[5];
	char Checksum[5];
}Header;
// UDP���� ������� ���ϱ� ���� ����ü

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
// UDP�� ��ü���� ���� checksum�� ����ϱ� ���� ����� ����ü

void Stringcopy(char* Dest, char* Src, int start, int end) // ���ڿ����� ���ϴ� �ش� ��ġ��ŭ �����Ѵ�.
{
	for (int i = start; i < end; i++)
		Dest[i - start] = Src[i];
	Dest[4] = NULL;
}

unsigned short Check_sum(unsigned short *p, int CheckByte) // üũ�� �Լ� // CheckByte�� �� �˻��� ����Ʈ��
{
	long sum = 0; 
	// 4����Ʈ�� �ڷ��� long����
	// short������ 2����Ʈ�� ������ checksum���� 2����Ʈ�� �����ϹǷ� �� ���� �����ϱ� ���� long���� ����
	while (CheckByte >=1){
		if(CheckByte==1)
			sum += (*(unsigned short*)p++)&0xFF00; //1����Ʈ���� �̵��Ͽ� ���� sum���� ����
		else
			sum += *((unsigned short*)p)++; //sum���� ���� 
		CheckByte -= 2; // 2���� ��ŭ ����
	}
	
	
		sum = (sum >> 16) + (sum & 0xFFFF);
	// sum >> 16 �� ���� 4�ڸ� ��
	// sum & 0xFFFF�� 4�ڸ� ���ǰ�
	// �����÷ο찡 �� ĳ������ sum�� �־ �����ִ� while��

	return ~sum; // ������ ���ؼ� ����
}
int main(void)
{
	FILE* fp = fopen("udp3.txt", "r"); // ���� read
	Ethernet ethernet;
	IPHeader ipheader;
	UDPHeader udpHeader;
	Header header;
	// ����ü ���� ����
	
	int OptionAndPaddingSize; // �ɼ� �� �е������ �迭ũ��� �Ҵ��Ͽ� ����� �� ���
	char *buf; //Option + padding�κ��� �����Ҵ��� ���� ��������
	char Data[N]; // ����� ū ũ��(N)���� ����
	char temp[5]; // Stringcopy�Լ��� �̿��Ͽ� 2����Ʈ�� ���� �����ϱ� ���� ����
	unsigned short checksum;
 
	ethernet.DestinationAddr[12] = ethernet.SourceAddr[12] = ethernet.Type[4] = NULL; 
	// �迭�� ������ ���� �ʱ�ȭ�Ͽ� �����Ⱚ�� ������ �ʰ� ��
	
	fread(ethernet.DestinationAddr, sizeof(char), 12, fp);
	fread(ethernet.SourceAddr, sizeof(char), 12, fp);
	fread(ethernet.Type, sizeof(char), 4, fp);
	// ������ �ʿ��� ũ�⸸ŭ �о ethernet����ü�� �ִ� ������ ������ ����

	printf("------Ethernet-------\n");
	printf("Destination address : %s\n", ethernet.DestinationAddr);
	printf("Source address : %s\n", ethernet.SourceAddr);
	printf("Type : %s\n", ethernet.Type);
	// ethernet������ �� ���

	ipheader.VER[1] = ipheader.HLEN[1] = ipheader.ServiceType[2] = ipheader.TotalLength[4] = NULL;
	ipheader.Identification[4] = ipheader.FlagsAndOffset[4] = ipheader.TTL[2] = ipheader.Protocol[2] = NULL;
	ipheader.HeaderChecksum[4] = ipheader.SourceIPAddr[8] = ipheader.DestinationIPAddr[8] = NULL;
	// �迭�� ������ ���� �ʱ�ȭ�Ͽ� �����Ⱚ�� ������ �ʰ� ��

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
	// ������ �ʿ��� ũ�⸸ŭ �о ipheader����ü�� �ִ� ������ ������ ����
	
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
	// IP Header������ �� ���

	OptionAndPaddingSize = atoi(ipheader.VER)*atoi(ipheader.HLEN);
	// atoi�� ���ڸ� ���ڷ� ���� // ����ü ������ VER�� HLEN�� char������ �޾ұ� �����̴�.

	if (OptionAndPaddingSize > 20)
	{
		buf = (char*)malloc(sizeof(char)*OptionAndPaddingSize - 20); // �ɼ� �� �е�ũ�Ⱑ 20���� Ŭ��� �� ũ�⸸ŭ �����Ҵ��� ���ְ�
		fread(buf, sizeof(char), OptionAndPaddingSize - 20, fp); // �����Ҵ��� ũ�⸸ŭ�� ũ�⸦ �д´�
		buf[OptionAndPaddingSize - 20] = NULL; // �߰��� �ɼ� �� �е��κ��� buf �迭���� �ʱ�ȭ
		printf("Option + padding : %dbytes\n", OptionAndPaddingSize - 20); // �ɼ� �� �е� �κ��� ����Ʈũ�� ���
	}

	header.SourcePortAddr[4] = header.DestinationPortAddr[4] = header.UDPTotalLength[4] = header.Checksum[4] = NULL;
	// �迭�� ������ ���� �ʱ�ȭ�Ͽ� �����Ⱚ�� ������ �ʰ� ��

	fread(header.SourcePortAddr, sizeof(char), 4, fp);
	fread(header.DestinationPortAddr, sizeof(char), 4, fp);
	fread(header.UDPTotalLength, sizeof(char), 4, fp);
	fread(header.Checksum, sizeof(char), 4, fp);
	// ������ �ʿ��� ũ�⸸ŭ �о header����ü�� �ִ� ������ ������ ����

	printf("------UDP Header------\n");
	printf("Source port address : %s\n", header.SourcePortAddr);
	printf("Destination port address : %s\n", header.DestinationPortAddr);
	printf("UDP total length : %s\n", header.UDPTotalLength);
	printf("UDP checksum : %s\n", header.Checksum);
	// UDP������ �� ���

	if (strcmp(header.Checksum, "0000") != 0)
		strcpy(header.Checksum, "0000");
	// checksum���� 0000�� �ƴҰ�� 0000���� �ʱ�ȭ

	memset(Data, 0, sizeof(char) * N); // Data�� �����ּҷ� ������ �޸𸮸� ä���
	fgets(Data, N, fp);

	if (strlen(Data) / 2 % 2 == 1) // �������� ���̰� 16��Ʈ�� ������ ������� == 1����Ʈ(8��Ʈ)�� ���� ������ ���
	{
		Data[strlen(Data)] = '0';
		Data[strlen(Data)] = '0';
		// �������� �κ��� 16��Ʈ�� ���������� �ʴ� ��� 0���� �е�
		// �迭�� ������ ���� �ʱ�ȭ�Ͽ� �����Ⱚ�� ������ �ʰ� ��
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
	// Stringcopy�Լ��� ���� 2����Ʈ�� ����� ����. ���߿� checksum������ ���� UDPHeader�� 2����Ʈ�� ��������.
	// Checksum�Լ��� ���� long(4����Ʈ)�ڷ������� short(2����Ʈ)�ڷ������� ������ ���� �޾� checksum������ �ϱ� �����̴�.
	// strtol => ���ڿ��ǰ��� ������ ��ȯ(16���� �� ��ȯ�� ���� ���)
	// strtol => (��ȯ����� ����������, ��ȯ����� ����������(NULL), N����)
	// �ǻ�����κ�

	for (int i = 0; i < strlen(Data); i += 4)
	{
		Stringcopy(temp, Data, i, i + 4);
		udpHeader.Data[i / 4] = (unsigned short)strtol(temp, NULL, 16);
	}
	// UDPHeader Data�κ��� 2����Ʈ�� ����� strtol�� ���� ���ڿ��� ���� ������ ��ȯ(16���� �� ��ȯ�� ���� ���)
	// ��� + ������ �κ�

	udpHeader.Checksum = Check_sum((unsigned short*)&udpHeader, udpHeader.UDPTotalLength + 12); 
	// UDPTotalLength => ���+������ // 12 => �ǻ����
	//Check_sum(UDPHeader�� �� ��ü checksum, UDPTotalLength������ ����κа� �����ͺκ��� ���� ���� �ǻ���� 12����Ʈ�� �����־���)
	printf("checksum : %X\n", udpHeader.Checksum);
	printf("checksum error check : %04x\n", Check_sum((unsigned short*)&udpHeader, udpHeader.UDPTotalLength +12));
	// checksum���� �տ��� ���� checksum������ �־ �ٽ� checksum���
	printf("----------------------\n");
}