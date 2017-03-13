#include <stdio.h>
#include <Windows.h>

int main(){
	int *frame_bit, *frame_bit_copy, *enter_bit;
	//전송데이터변수, 전송데이터 오류 검출을 위한 변수, 입력데이터
	int *xor_input, *xor_input_copy;
	//xor연산의 나머지입력값 변수, 마지막 연산값(final remainder)을 복사하여 오류검출을 확인하기 위한 변수
	int *input_data;
	//나머지 값이 직접 이동하여 연산을 수행하기 위한 변수
	int frame_bit_size=0, enter_bit_num=4;
	//전송데이터 크기를 위한 변수, 입력데이터의 수를 알기 위한 변수

	enter_bit = (int *)malloc(sizeof(int) * enter_bit_num); //비트 수에 따른 동적 할당

	printf("enter the 4bits(binary number)\n"); //비트 입력
	for(int i=0; i<enter_bit_num; i++){
		scanf("%d", &enter_bit[i]);	
		if(enter_bit[i] > 2){ //2진수가 아닌 수에 대한 에러 검출
			printf("잘못 입력하셨습니다.\n프로그램을 종료합니다\n");
			exit(0); //강제종료
		}
	}

	frame_bit_size = enter_bit_num + enter_bit_num-1; 
//전송데이터 사이즈 = 입력데이터의 수 + 입력데이터의 최고차항-1
	frame_bit = (int *)malloc(sizeof(int) * frame_bit_size); 
//전송데이터 크기에 따른 동적 할당
	frame_bit_copy = (int *)malloc(sizeof(int) * frame_bit_size); 
//전송데이터 크기만큼 동적할당

	xor_input = (int *)malloc(sizeof(int) * enter_bit_num-1); 
//crc부호화기에서 xor연산의 갯수만큼 동적할당
	xor_input_copy = (int *)malloc(sizeof(int) * enter_bit_num-1); //crc부호화기에서 xor연산의 갯수만큼 동적할당
	//copy는 final remainder를 출력하기 위한 변수
	input_data = (int *)malloc(sizeof(int) * enter_bit_num-1); 
//crc부호화기에서 나머지 첫째자리 값을 직접 받아 이동하여 연산하는 변수

	printf("data word : ");
	for(int i=0; i<frame_bit_size; i++){//전송데이터의 크기만큼 반복문을 돌려 전송데이터를 만들려고 함
		frame_bit[i] = enter_bit[i]; //전송데이터에 입력한 비트 수만큼 삽입
		if(i == enter_bit_num) //i가 입력한 비트수 만큼 같으면
			for(int j=0; j<enter_bit_num; j++){
				enter_bit[i+j+1] = 0;  
//입력 비트수크기의 끝부터 전송데이터 사이즈의 끝까지 0으로 초기화
				frame_bit[i] = enter_bit[i+j+1]; 
//전송데이터에 0을 삽입 == 입력한 비트 수의 최고차항만큼 0을 삽입
			}
			printf("%d ", frame_bit[i]); //전송데이터 출력
	}
	printf("\n");

	for(int i=0; i<enter_bit_num; i++){
		xor_input[i] = 0; // 나머지 초기값 변수
		input_data[i] = 0; // 나머지 값이 이동하여 직접 연산을 수행하는 변수
	} //xor연산의 처음 나머지는 0이기 때문에 0으로 초기화

	//나머지가 ㅁㅁㅁ 일때, 맨앞에부터 n자리라고 정의함
	for(int i=0, j=0; i<frame_bit_size; i++){ //전송데이터의 크기만큼 시뮬레이션 수행
	//input_data[j] 는 항상 0이 입력되기 때문에 0으로 초기화 시킨 값으로 연산 수행
		input_data[j+1] = xor_input[j]; 
//n자리의 나머지를 xor연산을 하기 위한 자리 이동
		input_data[j+2] = xor_input[j]; 
//n자리의 나머지를 xor연산을 하기 위한 자리 이동
		xor_input[j] = xor_input[j+1]^input_data[j]; 
//n자리의 나머지 값은 이전 연산의 n+1자리와 0의 xor연산
		xor_input[j+1] = xor_input[j+2]^input_data[j+1]; 
//n+1자리의 나머지 값은 이전 연산의 n+2자리와 n자리 나머지 값의 xor연산
		xor_input[j+2] = frame_bit[i]^input_data[j+2]; 
//n+2자리의 나머지 값은 이전 연산의 n자리 나머지 값과 입력dataword의 xor연산

		if(i==0){ //Time1에서 초기값 나머지 0출력
			printf("Time %d : %d %d %d\n", i+1, i, i, i);
		}
		else{ //Time2부터 전송데이터의 크기까지
			printf("Time %d : ", i+1);
			for(int k=0; k<enter_bit_num-1; k++){
				printf("%d ", xor_input[k]);
			} //최고차항의 수만큼 xor연산의 나머지 출력
			printf("\n");
		}

		if(i==frame_bit_size-1){ 
//i가 전송데이터의 크기만큼 시뮬레이션을 모두 수행했을 때,
			for(int l=0; l<enter_bit_num; l++){
				xor_input_copy[l] = xor_input[l]; 
			}	//xor연산한 값의 나머지를 복사
		}
	}

	printf("final remainder : ");
	for(int i=0; i<enter_bit_num-1; i++)
		printf("%d ", xor_input_copy[i]); 
//xor연산 값의 final remainder 출력
	printf("\n**************************\n");

	for(int i=0; i<enter_bit_num; i++){ //i가 입력데이터만큼 반복문 수행
		frame_bit_copy[i]= enter_bit[i]; 
//전송데이터에 처음 입력했던 비트를 복사
		frame_bit_copy[i+enter_bit_num] = xor_input_copy[i]; 
//전송데이터 뒤에 final remainder 값을 복사
	}

	printf("code word : ");
	for(int i=0; i<frame_bit_size; i++)
		printf("%d ", frame_bit_copy[i]); 
//오류 검출 확인을 위한 전송데이터에 final remainder 값을 복사한 code word를 출력
	printf("\n");

	for(int i=0; i<enter_bit_num; i++){
		xor_input[i] = 0;  //나머지 초기값 변수
		input_data[i] = 0; // 나머지 값이 이동하여 직접 연산을 수행하는 변수
	} 

	for(int i=0, j=0; i<frame_bit_size; i++){ //위와 같은 방법
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