#include <stdio.h>
#include <Windows.h>

int main(){
	int *frame_bit, *frame_bit_copy, *enter_bit;
	//���۵����ͺ���, ���۵����� ���� ������ ���� ����, �Էµ�����
	int *xor_input, *xor_input_copy;
	//xor������ �������Է°� ����, ������ ���갪(final remainder)�� �����Ͽ� ���������� Ȯ���ϱ� ���� ����
	int *input_data;
	//������ ���� ���� �̵��Ͽ� ������ �����ϱ� ���� ����
	int frame_bit_size=0, enter_bit_num=4;
	//���۵����� ũ�⸦ ���� ����, �Էµ������� ���� �˱� ���� ����

	enter_bit = (int *)malloc(sizeof(int) * enter_bit_num); //��Ʈ ���� ���� ���� �Ҵ�

	printf("enter the 4bits(binary number)\n"); //��Ʈ �Է�
	for(int i=0; i<enter_bit_num; i++){
		scanf("%d", &enter_bit[i]);	
		if(enter_bit[i] > 2){ //2������ �ƴ� ���� ���� ���� ����
			printf("�߸� �Է��ϼ̽��ϴ�.\n���α׷��� �����մϴ�\n");
			exit(0); //��������
		}
	}

	frame_bit_size = enter_bit_num + enter_bit_num-1; 
//���۵����� ������ = �Էµ������� �� + �Էµ������� �ְ�����-1
	frame_bit = (int *)malloc(sizeof(int) * frame_bit_size); 
//���۵����� ũ�⿡ ���� ���� �Ҵ�
	frame_bit_copy = (int *)malloc(sizeof(int) * frame_bit_size); 
//���۵����� ũ�⸸ŭ �����Ҵ�

	xor_input = (int *)malloc(sizeof(int) * enter_bit_num-1); 
//crc��ȣȭ�⿡�� xor������ ������ŭ �����Ҵ�
	xor_input_copy = (int *)malloc(sizeof(int) * enter_bit_num-1); //crc��ȣȭ�⿡�� xor������ ������ŭ �����Ҵ�
	//copy�� final remainder�� ����ϱ� ���� ����
	input_data = (int *)malloc(sizeof(int) * enter_bit_num-1); 
//crc��ȣȭ�⿡�� ������ ù°�ڸ� ���� ���� �޾� �̵��Ͽ� �����ϴ� ����

	printf("data word : ");
	for(int i=0; i<frame_bit_size; i++){//���۵������� ũ�⸸ŭ �ݺ����� ���� ���۵����͸� ������� ��
		frame_bit[i] = enter_bit[i]; //���۵����Ϳ� �Է��� ��Ʈ ����ŭ ����
		if(i == enter_bit_num) //i�� �Է��� ��Ʈ�� ��ŭ ������
			for(int j=0; j<enter_bit_num; j++){
				enter_bit[i+j+1] = 0;  
//�Է� ��Ʈ��ũ���� ������ ���۵����� �������� ������ 0���� �ʱ�ȭ
				frame_bit[i] = enter_bit[i+j+1]; 
//���۵����Ϳ� 0�� ���� == �Է��� ��Ʈ ���� �ְ����׸�ŭ 0�� ����
			}
			printf("%d ", frame_bit[i]); //���۵����� ���
	}
	printf("\n");

	for(int i=0; i<enter_bit_num; i++){
		xor_input[i] = 0; // ������ �ʱⰪ ����
		input_data[i] = 0; // ������ ���� �̵��Ͽ� ���� ������ �����ϴ� ����
	} //xor������ ó�� �������� 0�̱� ������ 0���� �ʱ�ȭ

	//�������� ������ �϶�, �Ǿտ����� n�ڸ���� ������
	for(int i=0, j=0; i<frame_bit_size; i++){ //���۵������� ũ�⸸ŭ �ùķ��̼� ����
	//input_data[j] �� �׻� 0�� �ԷµǱ� ������ 0���� �ʱ�ȭ ��Ų ������ ���� ����
		input_data[j+1] = xor_input[j]; 
//n�ڸ��� �������� xor������ �ϱ� ���� �ڸ� �̵�
		input_data[j+2] = xor_input[j]; 
//n�ڸ��� �������� xor������ �ϱ� ���� �ڸ� �̵�
		xor_input[j] = xor_input[j+1]^input_data[j]; 
//n�ڸ��� ������ ���� ���� ������ n+1�ڸ��� 0�� xor����
		xor_input[j+1] = xor_input[j+2]^input_data[j+1]; 
//n+1�ڸ��� ������ ���� ���� ������ n+2�ڸ��� n�ڸ� ������ ���� xor����
		xor_input[j+2] = frame_bit[i]^input_data[j+2]; 
//n+2�ڸ��� ������ ���� ���� ������ n�ڸ� ������ ���� �Է�dataword�� xor����

		if(i==0){ //Time1���� �ʱⰪ ������ 0���
			printf("Time %d : %d %d %d\n", i+1, i, i, i);
		}
		else{ //Time2���� ���۵������� ũ�����
			printf("Time %d : ", i+1);
			for(int k=0; k<enter_bit_num-1; k++){
				printf("%d ", xor_input[k]);
			} //�ְ������� ����ŭ xor������ ������ ���
			printf("\n");
		}

		if(i==frame_bit_size-1){ 
//i�� ���۵������� ũ�⸸ŭ �ùķ��̼��� ��� �������� ��,
			for(int l=0; l<enter_bit_num; l++){
				xor_input_copy[l] = xor_input[l]; 
			}	//xor������ ���� �������� ����
		}
	}

	printf("final remainder : ");
	for(int i=0; i<enter_bit_num-1; i++)
		printf("%d ", xor_input_copy[i]); 
//xor���� ���� final remainder ���
	printf("\n**************************\n");

	for(int i=0; i<enter_bit_num; i++){ //i�� �Էµ����͸�ŭ �ݺ��� ����
		frame_bit_copy[i]= enter_bit[i]; 
//���۵����Ϳ� ó�� �Է��ߴ� ��Ʈ�� ����
		frame_bit_copy[i+enter_bit_num] = xor_input_copy[i]; 
//���۵����� �ڿ� final remainder ���� ����
	}

	printf("code word : ");
	for(int i=0; i<frame_bit_size; i++)
		printf("%d ", frame_bit_copy[i]); 
//���� ���� Ȯ���� ���� ���۵����Ϳ� final remainder ���� ������ code word�� ���
	printf("\n");

	for(int i=0; i<enter_bit_num; i++){
		xor_input[i] = 0;  //������ �ʱⰪ ����
		input_data[i] = 0; // ������ ���� �̵��Ͽ� ���� ������ �����ϴ� ����
	} 

	for(int i=0, j=0; i<frame_bit_size; i++){ //���� ���� ���
		input_data[j+1] = xor_input[j];
		input_data[j+2] = xor_input[j];
		xor_input[j] = xor_input[j+1]^input_data[j];
		xor_input[j+1] = xor_input[j+2]^input_data[j+1];
		xor_input[j+2] = frame_bit_copy[i]^input_data[j+2];

		if(i==0){
			printf("Time %d : %d %d %d\n", i+1, i, i, i);
		}
		else{
			printf("Time %d : ", i+1);
			for(int k=0; k<enter_bit_num-1; k++){
				printf("%d ", xor_input[k]);
			} 
			printf("\n");
		}
	}
	return 0;
}