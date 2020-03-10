#include <cstdio>
#include <vector>
#include <queue>
#include <cstring>
#include <cstdlib>
using namespace std;
vector<char> in;
typedef struct _node {
	char symbol;
	int freg;
	struct _node* left;
	struct _node* right;
}node;//data structure
struct cmp{
bool operator()(node* t, node* u) {
	return t->freg > u->freg;
	}
};//������ ����� ����.
FILE* out;
priority_queue<node*,vector<node*>,cmp> pq;
char* outputname;
vector<int> v[300];//0~256������ ��Ÿ�� �� �ִ� ����Ʈ�� � ����Ʈ�� ������ �ڵ忡�� � ��Ʈ��Ʈ���� �����Ǵ°�

int frequency[600];//���� �� ���� ��Ÿ����.

int mul, bits_to_byte;//��Ʈ�� �����Ͽ� ���Ͽ� �ֱ� ���� �ʿ��� �������̴�.
void make_tree(node* root) {//Ʈ���� ���Ͽ� �ֱ� ���� ���� �Լ��̴�.
	int ret = 0;
	if (root->left == NULL && root->right == NULL) {//��������� ��
		ret = mul;//��������϶����� 1�̱� ������ �̶����� bits_to_byte�� mul�� ���Ѵ�.
		bits_to_byte += ret;
		mul /= 2;
		if (mul == 0) {//8��° ��Ʈ�� ���� ���� ��
			fprintf(out, "%c", bits_to_byte);//���Ͽ� �����ش�.
			mul = 128;
			bits_to_byte = 0;
		}
		int t = root->symbol;//��������̱� ������ symbol ���� �־��ش�.
		if (t<0) t += 256;
		int tt = 128;//�տ� ��Ʈ���� ��󳻱� ���� �������־���.
 		for (int i = 0; i<8; i++) {
			int k = t / tt;
			bits_to_byte += k * mul;
			mul /= 2;
			t %= tt;
			tt /= 2;
			if (mul == 0) { 
				fprintf(out, "%c", bits_to_byte);
				mul = 128;
				bits_to_byte = 0;
			}
		}
	}
	else mul /= 2;
	if (mul == 0) {
		char c = bits_to_byte;
		fprintf(out, "%c", c);
		mul = 128;
		bits_to_byte = 0;
	}
	if (root->left) make_tree(root->left);//dfs�� ���� ��� �۾��� ���ش�.
	if (root->right) make_tree(root->right);
}
vector<int> temp;//stack�� ���� ������ �Ͽ���. ��� ����Ʈ���� ��Ʈ��Ʈ���� �����ϴ� v�� vector�� ������ ���� vector�� �����־���.
void dfs(node* root, int depth) {
	if (root->left == NULL && root->right == NULL) {//��������϶�
		int t = root->symbol;
		if(t<0) t+=256;
		v[t] = temp;//���ʹ� ���� �����̴�. 
	}
	else {
		temp.push_back(0);//������ 0, �������� 1
		dfs(root->left, depth + 1);
		temp.pop_back();
		temp.push_back(1);
		dfs(root->right, depth + 1);
		temp.pop_back();
	}
}
void encoding_huffman(const char* argv) {
	FILE* input = fopen(argv, "r");
	if(!input){
		printf("file open error!!");
		return;
	}//���� ������ ������ �ʴ´ٸ� ������ ����Ѵ�.
	int len = strlen(argv);
	outputname = (char*)calloc(sizeof(char), len + 4);
	for (int i = 0; i<len; i++) {
		outputname[i] = argv[i];
	}
	outputname[len] = '.';
	outputname[len + 1] = 'z';
	outputname[len + 2] = 'z';
	outputname[len + 3] = '\0';
	out = fopen(outputname, "w");//output file�� �����ش�.
	char c;
	//while (fscanf(input,"%c", &c) != EOF)
	while (1){
		c = fgetc(input);
		if (c==EOF) break;
		in.push_back(c);
		int t = c;
		if (t<0) t += 256;
		frequency[t]++;
	}//�Է¹��� �����͸� �޾ƿ´�.
	if(in.size()==0){
		return;
	}//���� �����Ͱ� �ƹ��͵� ���� ��� �ƹ��ϵ� ���� �ʴ� ���� �̵��̶� �Ǵ��Ͽ���.
	for (int i = 0; i < 256; i++) {
		if (frequency[i] != 0) {//�������� ��Ÿ������ �� i ��° ����Ʈ�� ���� ������ ����Ʈ�� �ѹ� �̻� �����Ͽ���.
			int t = i < 128 ? i : i - 256;
			node* temp = (node*)calloc(sizeof(node), 1);
			temp->left = NULL;
			temp->right = NULL;
			temp->freg = frequency[i];
			temp->symbol = t;
			pq.push(temp);//��带 ����� �װ��� pq�� �ִ´�.
		}
	}
	while (pq.size() > 1) {
		node* x = pq.top(); pq.pop();
		node* y = pq.top(); pq.pop();
		node* z = (node*)calloc(sizeof(node), 1);
		z->left = x;
		z->right = y;
		z->freg = x->freg + y->freg;
		z->symbol = 0;
		pq.push(z);
	}
	node* root = pq.top(); pq.pop();
	//huffman tree�� �����ǰ� root�� root ������ ����.
	bits_to_byte = 0;
	mul = 128;//�ʱ�ȭ
	if(root->right==NULL&&root->left==NULL){//���� ����Ÿ�� �ϳ��� ���� ��� ���ο� ����� ���ʿ� ���� �ִ� ��带 �ٿ��ָ� decoding �ϴ� ���������� ������ ���� �� �ִ�.
		node* newroot = (node*)calloc(sizeof(node),1);
		newroot->left = NULL;
		newroot->right = NULL;
		newroot->symbol = root->symbol;
		root->left = newroot;
		root->right = (node*)calloc(sizeof(node),1);
		root->right->symbol = 1;
		root->right->left=  NULL;
		root->right->right = NULL;
	}
	make_tree(root);//�����Ϳ� Ʈ���� �����ش�.
	dfs(root, 0);
	for (int i = 0; i<in.size(); i++) {
		char c = in[i];
		int t = c;
		if(t<0) t+=256;
		for (int i=0;i<v[t].size();i++) {
			int j = v[t][i];
			bits_to_byte += mul * j;
			mul /= 2;
			if (mul == 0) {
				mul = 128;
				fprintf(out, "%c", bits_to_byte);
				bits_to_byte = 0;
			}
		}
	}//�����Ϳ� ���� ������ �����Ѵ�.
	int remainder;
	int kkk = 1;
	for (remainder = 1; remainder <= 8; remainder++) {
		if (kkk == mul) break;
		kkk *= 2;
	}
	if (mul != 128) {
		fprintf(out, "%c", bits_to_byte);
		fprintf(out, "%c", remainder);
	}
	else fprintf(out,"%c",remainder);

	//������ ����Ʈ���� ���� �ִ� ��Ʈ ���� ����д�.
}
//int tempidx;
FILE* input;
int nowc;
int rc1, rc2;

char get_char() {//Ʈ���� �籸���ϴ� �������� ���� ��Ʈ���� 1�϶� ���� 8���� ����Ʈ�� �޾� �װ��� ���� ���Ѵ�.
	char ret = 0;
	int tt = 128;
	for (int i = 0; i < 8; i++) {
		int t = nowc / mul;
		ret += tt * t;
		nowc %= mul;
		mul /= 2; tt /= 2;
		if (mul == 0) {
			char c; fscanf(input, "%c", &c);
			int t = c;
			nowc = t < 0 ? t + 256 : t;
			mul = 128;
		}
	}
	return ret;
}

node* remake_tree() {//Ʈ���� �籸���Ѵ�.
	node* ret = (node*)calloc(1,sizeof(node));
	int t = nowc / mul;
	nowc %= mul;
	mul /= 2;
	if (mul == 0) {
		char c; fscanf(input, "%c", &c);
		int temp = c;
		nowc = temp < 0 ? temp + 256 : temp;
		mul = 128;
	}
	if (t == 0) {//���� ���� ��Ʈ�� 0�̶�� ����� ���ʿ��� ù��° ���, �����ʿ��� �ι�° ����� ret���� ���´�.
		ret->left = remake_tree();
		ret->right = remake_tree();
	}
	else {
		char c = get_char();
		ret->symbol = c;
		ret->right = NULL;
		ret->left = NULL;//c�� ������ �ϴ� ��带 �ϳ� ���� return���ش�.
	}
	return ret;
}
bool isend = false;
void decode(node* root) {//Ʈ���� ������ ������ �籸���ϴ� �ܰ��̴�.
	if (root->left == NULL && root->right == NULL) {
		fprintf(out, "%c", root->symbol);
		return;
	}//���� ������� symbol�� ������ش�.
	int t = nowc / mul;
	nowc %= mul;
	mul /= 2;
	if (mul == 0) {
		char c;
		int iseof = fscanf(input,"%c",&c);
		int temp = c;
		if(temp<0) temp+=256;
		nowc = rc1;
		rc1 = rc2;
		rc2 = temp;		
		if (iseof == EOF) {
			isend = true;//�̰��� �ڿ��� �ι�° ����Ʈ�� ���� �������ֱ� �����̴�.
		}
		mul = 128;
	}
	if (t == 0) {//0�̸� ���� 1�̸� ������
		decode(root->left);
	}
	else decode(root->right);
}

void decoding_huffman(const char* argv) {
	input = fopen(argv, "r");
	if(!input){
		printf("file open error!!");
		return;
	}
	//���� ������ ������ �ʴ´ٸ� �����޼����� ����.
	int len = strlen(argv);
	char* outputname = (char*)calloc(sizeof(char), len + 4);
	for (int i = 0; i < len; i++) outputname[i] = argv[i];
	outputname[len] = '.';
	outputname[len + 1] = 'y';
	outputname[len + 2] = 'y';
	outputname[len + 3] = '\0';
	out = fopen(outputname, "w");
	//output file open
	mul = 128;
	char c;
	int iseof = fscanf(input, "%c", &c);
	if(iseof==EOF) return;
	nowc = c;
	if (nowc < 0) nowc += 256;
	node* root = remake_tree();
	fscanf(input,"%c",&c);
	rc1 = c;
	if(rc1<0) rc1 += 256;
	iseof = fscanf(input,"%c",&c);
	rc2 = c;
	if(rc2<0) rc2 += 256;
	if(iseof==EOF) isend = true;
	while (1) {
		if(!isend) decode(root);
		if (isend == true) {
			int remainder = rc1;
			int kkk = 1 << ((remainder+7) % 8);
			if(kkk==128){
				decode(root);
			}
			while(mul!=kkk){//���� ���������� �ι�° ����� ��Ʈ�� ���� ������ ������ �д´�.
				decode(root);
			}
			break;
		}
	}
}

int main(int argc, char* argv[]) {
	if(argc!=3){
		printf("input error!!");
		return 0;
	}
	if(strcmp(argv[1],"-c")==0) encoding_huffman(argv[2]);
	else if(strcmp(argv[1],"-d")==0) decoding_huffman(argv[2]);
	else{
		printf("input error!!");
		return 0;
	}
	return 0;
}
